// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"

#include "Gameplay/Skills/SkillNode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include <random>

#include "Gameplay/Subsystem/SkillsRuntimeSubsystem.h"

const float USkillsManagerSubsystem::AdditionalParamForAttack = 10.0f;

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
	HashToNode.Remove(Node->GetHashID());

	USkillNode* TempChildNode = Node->GetFirstChildNode();
	
	Node->ForceRemoveAllChild();

	if (Node->GetNodeType() == ESkillNodeType::LoopEndNode)
	{
		ConnectNode(Node->GetParentNode(), TempChildNode);
	}
	
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
	FAccumulativeInfo AccumulativeInfo = FAccumulativeInfo::ZeroValue;
	if (_Node->GetParentNode() && _Node->GetParentNode()->GetNodeType() == ESkillNodeType::ParamNode)
		AccumulativeInfo = _Node->GetParentNode()->GetAccumulativeInfo();

	_Node->SetAccumulativeInfo(_Node->GetAccumulativeInfo() + AccumulativeInfo);

	// 如果已经找到第一个普通节点，处理并返回
	if (_Node->GetNodeType() != ESkillNodeType::ParamNode)
	{
		UpdateTimeDelay(_Node);
		return;
	}
	
	DfsUpdateForwardParam(_Node->GetFirstChildNode());
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
