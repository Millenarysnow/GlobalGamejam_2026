// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Skills/SkillNode.h"

#include "Gameplay/Component/ExecuteSkillComponent.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Skills/SkillExecutionContext.h"

#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogSkillNode);

const FName USkillNode::Tag_SkillComponent = "SkillComponent";

void USkillNode::Initialize(const int32 _HashID, const FSkillNodeInfo& _NodeInfo)
{
	HashID = _HashID;

	NodeInfo = _NodeInfo;

	SetDefaultValue();

	SkillsManagerSubsystem = USkillsManagerSubsystem::Get(this);

	checkf(SkillsManagerSubsystem, TEXT("Millenarysnow : SkillNode cant get SkillManagerSubsystem"));
}

void USkillNode::SetDefaultValue()
{
	AllDelayTime = NodeInfo.DelayTime;
}

bool USkillNode::AddChildNode(USkillNode* Node, OnBranchNode Branch)
{
	if (ChildNodes.Num() >= NodeInfo.OutPinCount) return false;
	
	if (Branch != OnBranchNode::No)
	{
		if (NodeInfo.NodeType != ESkillNodeType::BranchNode) return false;
		
		if (Branch == OnBranchNode::TrU)
		{
			if (!BranchTrueNode) BranchTrueNode = Node;
			else return false;
		}
		else
		{
			if (!BranchFalseNode) BranchFalseNode = Node;
			else return false;			
		}
	}
	
	ChildNodes.Add(Node);

	AllDelayTime = std::max(AllDelayTime, Node->GetDelayTime());
	Node->SetParentNode(this);

	return true;
}

void USkillNode::RemoveChildNode(USkillNode* Node, OnBranchNode Branch)
{
	if (!Node) return ;

	// 从子节点列表中移除
	for (int i = 0; i < ChildNodes.Num(); i++)
	{
		if (ChildNodes[i]->GetHashID() == Node->GetHashID())
		{
			ChildNodes.RemoveAt(i);
			Node->SetParentNode(nullptr);
			break;
		}
	}

	if (BranchTrueNode && BranchTrueNode->GetHashID() == Node->GetHashID())
		BranchTrueNode = nullptr;
	if (BranchFalseNode && BranchFalseNode->GetHashID() == Node->GetHashID())
		BranchFalseNode = nullptr;
	
	CalculateDelayTime();
}

void USkillNode::Delivery(TSharedPtr<FSkillExecutionContext> Context)
{
	AActor* TargetActor = Context->GetTarget();
	if (!TargetActor) return;

	if (!HasChild()) // 叶节点
	{
		USkillNode* NextNode = nullptr;
			
		while (!Context->IsStackEmpty())
		{
			FSkillStackFrame& StackFrame = Context->TopStackRef();
			StackFrame.CurrentCount ++;
			if (StackFrame.CurrentCount >= StackFrame.MaxCount)
				Context->PopStack();
			else
			{
				NextNode = StackFrame.LoopBodyStart;
				break;
			}
		}

		if (NextNode)
			NextNode->Trigger(Context);
		else
			return ;
	}

	// 如果只有一个子节点且不需要切换目标，则直接传递不拷贝
	bool bShouldCopy = ChildNodes.Num() > 1;
	
	if (NodeInfo.NodeType != ESkillNodeType::BranchNode)
	{
		for (const auto& Element : ChildNodes)
		{
			if (bShouldCopy)
			{
				TSharedPtr<FSkillExecutionContext> NextContext = MakeShared<FSkillExecutionContext>(*Context);
				Element->Trigger(NextContext);
			}
			else
			{
				Element->Trigger(Context);
			}
		}
	}
	else
	{
		bool bResult = Branch(TargetActor);

		USkillNode* NextNode = bResult ? BranchTrueNode : BranchFalseNode;

		if (NextNode)
		{
			/**
			 * 这里传拷贝是因为考虑到后续扩展，可能有并行需求
			 */
			TSharedPtr<FSkillExecutionContext> NextContext = MakeShared<FSkillExecutionContext>(*Context);
			NextNode->Trigger(NextContext);
		}
	}
}

void USkillNode::Trigger(TSharedPtr<FSkillExecutionContext> Context)
{
	if (!SkillsManagerSubsystem->GetCanRun()) return;
	
	this->Ability(Context);
	
	if (NodeInfo.NodeType == ESkillNodeType::LoopStartNode && HasChild())
	{
		FSkillStackFrame StackFrame;
		StackFrame.LoopBodyStart = this->GetFirstChildNode();
		StackFrame.MaxCount = NodeInfo.LoopCount;

		Context->PushStack(StackFrame);
	}

	if (NodeInfo.SwitchTargetType == ESwitchTargetType::Unchanged)
		Delivery(Context);
}

bool USkillNode::Branch(const AActor* Target)
{
	return true;
}

int32 USkillNode::CalculateDelayTime()
{
	/**
	 * 参数节点本身不添加累加延迟
	 * 如果不是参数节点，就施加累加减少的延迟
	 */

	// 计算自身延迟
	float ReduceDelayTimeValue = std::max(1.0f - AccumulativeInfo.AccReduceDelayTimeValue, 0.0f);
	int32 SelfDelayTime = (GetNodeType() == ESkillNodeType::ParamNode) ? 0 : ReduceDelayTimeValue * NodeInfo.DelayTime;

	// 计算子节点最大延迟
	int32 MaxChildDelay = 0;
	for (const auto& Element : ChildNodes)
	{
		MaxChildDelay = std::max(MaxChildDelay, Element->GetDelayTime());
	}

	// 计算总延迟
	int32 TotalDelay = 0;
	if (GetNodeType() == ESkillNodeType::LoopStartNode)
	{
		// 循环节点需要把子节点延迟乘以循环次数
		TotalDelay = (SelfDelayTime + MaxChildDelay) * NodeInfo.LoopCount;
	}
	else
	{
		TotalDelay = MaxChildDelay + SelfDelayTime;
	}

	AllDelayTime = TotalDelay;
	return AllDelayTime;
}

bool USkillNode::HasChild() const
{
	return ChildNodes.Num() > 0;
}

void USkillNode::SetParentNode(USkillNode* Node)
{
	ParentNode = Node;
}

USkillNode* USkillNode::GetParentNode() const
{
	return ParentNode;
}

int32 USkillNode::GetHashID() const
{
	return HashID;
}

int32 USkillNode::GetDelayTime() const
{
	return AllDelayTime;
}

ESkillNodeType USkillNode::GetNodeType() const
{
	return NodeInfo.NodeType;
}

void USkillNode::ForceRemoveAllChild()
{
	for (auto& Element : ChildNodes)
	{
		if (Element)
			Element->SetParentNode(nullptr);
	}
	ChildNodes.Empty();

	BranchTrueNode = nullptr;
	BranchFalseNode = nullptr;
}

USkillNode* USkillNode::GetFirstChildNode()
{
	return ChildNodes.Num() > 0 ? ChildNodes[0] : nullptr;
}

int32 USkillNode::GetChildCount() const
{
	return ChildNodes.Num();
}

FAccumulativeInfo USkillNode::GetAccumulativeInfo() const
{
	return AccumulativeInfo;
}

void USkillNode::SetAccumulativeInfo(const FAccumulativeInfo& Info)
{
	AccumulativeInfo = Info;
}

void USkillNode::SetBranchTrueNode(USkillNode* Node)
{
	BranchTrueNode = Node;
}

void USkillNode::SetBranchFalseNode(USkillNode* Node)
{
	BranchFalseNode = Node;
}

USkillNode* USkillNode::GetBranchTrueNode()
{
	return BranchTrueNode;
}

USkillNode* USkillNode::GetBranchFalseNode()
{
	return BranchFalseNode;
}

const FSkillNodeInfo& USkillNode::GetNodeInfo() const
{
	return NodeInfo;
}

UExecuteSkillComponent* USkillNode::GetTargetExecuteSkillComponent(const AActor* Target) const
{
	if (!Target) return nullptr;
	
	UExecuteSkillComponent* SkillComponent = Target->FindComponentByTag<UExecuteSkillComponent>(USkillNode::Tag_SkillComponent);
	checkf(SkillComponent, TEXT("Millenarysnow : Skill Target not has a Skill Component."));

	return SkillComponent;
}
