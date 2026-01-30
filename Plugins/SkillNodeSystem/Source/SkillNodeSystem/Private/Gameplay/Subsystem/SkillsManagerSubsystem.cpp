// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"

#include "Gameplay/Skills/SkillNode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include <random>

#include "Gameplay/Skills/SkillChildType.h"
#include "Gameplay/Subsystem/SkillsRuntimeSubsystem.h"

const float USkillsManagerSubsystem::AdditionalParamForAttack = 10.0f;

FStartNodeInfo USkillsManagerSubsystem::TestMake()
{
	return FStartNodeInfo(EStartNodeType::LeftButton);
}

FGenerateNodeInfo USkillsManagerSubsystem::TestMake2()
{
	return FGenerateNodeInfo(EGeneratedType::StandardBullet, 50.f, 200.f);
}

void USkillsManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	std::random_device _seed;
	HashGenerateEngine.seed(_seed());
}

USkillNode* USkillsManagerSubsystem::NewSkillNode(const FSkillNodeInfo& NodeInfo, TSubclassOf<USkillNode> SkillNodeClass)
{
	std::uniform_int_distribution<> distribution(0, 0x3fffffff);
	int32 HashID = distribution(HashGenerateEngine);
    
	USkillNode* Node = NewObject<USkillNode>(this, SkillNodeClass);
	Node->Initialize(HashID, NodeInfo);
    
	HashToNode.Add(HashID, Node);

	UE_LOG(LogSkillNode, Warning, TEXT("NewNode : Type : %s, HashID : %d"), *UEnum::GetValueAsString(NodeInfo.NodeType), HashID);
    
	if (NodeInfo.NodeType == ESkillNodeType::StartNode)
	{
		GetSkillsRuntimeSubsystem()->AddStartSkillNode(Node);
	}
	
	return Node;
}

void USkillsManagerSubsystem::DeleteSkillNode(USkillNode* Node, OnBranchNode Branch)
{
	if (!Node) return ;
	
	HashToNode.Remove(Node->GetHashID());

	// 重置所有子节点的状态
	Node->ForEachChild([this](USkillNode* Child)
	{
		if (Child)
		{
			Child->SetParentNode(nullptr);
			Child->SetAccumulativeInfo(FAccumulativeInfo::ZeroValue);
			DfsUpdateForwardParam(Child);
		}
	});

	// 清空子节点列表
	Node->ForceRemoveAllChild();

	if (Node->GetParentNode())
		DisconnectNode(Node->GetParentNode(), Node, Branch);

	if (Node->GetNodeType() == ESkillNodeType::StartNode)
		GetSkillsRuntimeSubsystem()->RemoveStartSkillNode(Node);
	
	Node->MarkAsGarbage();
}

bool USkillsManagerSubsystem::ConnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch)
{
	if (!ParentNode || !ChildNode) return false;
	if (ParentNode->GetHashID() == ChildNode->GetHashID()) return false;

	// 如果子节点已经有父节点，断开之前连接
	if (ChildNode->GetParentNode())
	{
		DisconnectNode(ChildNode->GetParentNode(), ChildNode, OnBranchNode::No);
	}
	
	if (!ParentNode->AddChildNode(ChildNode, Branch)) return false;
	
	ChildNode->SetParentNode(ParentNode);
	
	if (ParentNode->GetNodeType() == ESkillNodeType::ParamNode)
		DfsUpdateForwardParam(ChildNode);
	else
		UpdateTimeDelay(ParentNode);
	return true;
}

void USkillsManagerSubsystem::DisconnectNode(USkillNode* ParentNode, USkillNode* ChildNode, OnBranchNode Branch)
{
	if (!ParentNode || !ChildNode) return;
	if (ChildNode->GetParentNode()->GetHashID() != ParentNode->GetHashID()) return;
	
	ParentNode->RemoveChildNode(ChildNode, Branch);
	UpdateTimeDelay(ParentNode);
	ChildNode->SetParentNode(nullptr);
	ChildNode->SetAccumulativeInfo(FAccumulativeInfo::ZeroValue);
	DfsUpdateForwardParam(ChildNode);
}

USkillNode* USkillsManagerSubsystem::GetSkillNodeByHash(const int32 _HashID) const
{
	return HashToNode.Contains(_HashID) ? HashToNode[_HashID] : nullptr;
}

USkillsManagerSubsystem* USkillsManagerSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<USkillsManagerSubsystem>() : nullptr;
}

void USkillsManagerSubsystem::SetCanRun(bool Value)
{
	bCanRun = Value;
}

bool USkillsManagerSubsystem::GetCanRun()
{
	return bCanRun;
}

TMap<int32, USkillNode*>& USkillsManagerSubsystem::GetAllNodes()
{
	return HashToNode;
}

USkillsRuntimeSubsystem* USkillsManagerSubsystem::GetSkillsRuntimeSubsystem()
{
	if (!SkillsRuntimeSubsystem)
	{
		SkillsRuntimeSubsystem = GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>();
		checkf(SkillsRuntimeSubsystem, TEXT("Millenarysnow : SkillManagerSubsystem cant get SkillRuntimeSubsystem"));
	}
	return SkillsRuntimeSubsystem;
}

void USkillsManagerSubsystem::DfsUpdateForwardParam(USkillNode* _Node)
{
	if (!_Node) return;

	// 如果父节点是参数节点，就把信息传下来
	FAccumulativeInfo CurrentInfo = FAccumulativeInfo::ZeroValue;
	if (_Node->GetParentNode() && _Node->GetParentNode()->GetNodeType() == ESkillNodeType::ParamNode)
	{
		CurrentInfo = _Node->GetParentNode()->GetAccumulativeInfo();
	}

	// 如果当前是参数节点，将自身的配置值添加到累加信息中
	if (_Node->GetNodeType() == ESkillNodeType::ParamNode)
	{
		const FSkillNodeInfo& Info = _Node->GetNodeInfo();
		switch (Info.ParamType)
		{
		case EParamType::Damage:
			CurrentInfo.AccDamageValue += Info.ParamFloatValue;
			break;
		case EParamType::EffectiveTime:
			CurrentInfo.AccEffectiveTimeValue += Info.ParamFloatValue;
			break;
		case EParamType::ReduceDelayTime:
			CurrentInfo.AccReduceDelayTimeValue += Info.ParamFloatValue;
			break;
		case EParamType::Radius:
			CurrentInfo.RadiusValue += Info.ParamFloatValue;
			break;
		default:
			break;
		}
	}

	// 更新当前节点的累加信息
	_Node->SetAccumulativeInfo(CurrentInfo);

	if (_Node->GetNodeType() == ESkillNodeType::ParamNode)
	{
		// 如果是参数节点，继续下传
		_Node->ForEachChild([this](USkillNode* Child)
		{
			DfsUpdateForwardParam(Child);
		});
	}
	else
	{
		// 如果是非参数节点，向上更新延迟
		UpdateTimeDelay(_Node);	
	}
}

void USkillsManagerSubsystem::UpdateTimeDelay(USkillNode* _UpdateStartNode)
{
	USkillNode* CurrentNode = _UpdateStartNode;
	while (CurrentNode)
	{
		CurrentNode->CalculateDelayTime();
		CurrentNode = CurrentNode->GetParentNode();
	}
}
