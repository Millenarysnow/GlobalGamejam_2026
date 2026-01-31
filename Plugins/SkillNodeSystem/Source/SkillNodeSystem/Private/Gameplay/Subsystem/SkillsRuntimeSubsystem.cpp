// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/SkillsRuntimeSubsystem.h"

#include "Gameplay/Skills/SkillNode.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"

void USkillsRuntimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	for (int i = 0; i < 6; i++)
	{
		StartSkillNodeByMap.Add(i, FStartSkillNodeMap());
	}
}

void USkillsRuntimeSubsystem::LaunchAllStartNode()
{
	GetSkillsManagerSubsystem()->SetCanRun(true);
}

void USkillsRuntimeSubsystem::StopAllStartNode()
{
	GetSkillsManagerSubsystem()->SetCanRun(false);
}

void USkillsRuntimeSubsystem::TriggerStartNode(EStartNodeType StartNodeType)
{
	UGameInstance* const& GameInstance = GetGameInstance();
	checkf(GameInstance != nullptr, TEXT("Millenarysnow : GameInstance in SkillRuntimeSubsystem is nullptr."));
	
	FStartSkillNodeMap& StartSkillNodeMap = StartSkillNodeByMap[static_cast<int>(StartNodeType)];
	for (auto& Element : StartSkillNodeMap.CanTrigger)
	{
		if (Element.Value)
		{
			USkillNode* Node = Element.Key;
			FTimerHandle& TimerHandle = StartSkillNodeMap.NodeMap[Node];

			Node->Start();
			
			GameInstance->GetTimerManager().SetTimer(
			TimerHandle,
			[this, Node]()
			{
				SetCooldownTrue(Node);
			},
			Node->GetDelayTime() / 1000.0f,
			false
			);

			Element.Value = false;
		}
	}
}

void USkillsRuntimeSubsystem::ClearNode()
{
	StartSkillNodeByMap.Empty();
}

USkillsRuntimeSubsystem* USkillsRuntimeSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<USkillsRuntimeSubsystem>() : nullptr;
}

void USkillsRuntimeSubsystem::SetCooldownTrue(USkillNode* Node)
{
	if (!Node) return;
	
	if (StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].CanTrigger.Contains(Node))
	{
		StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].CanTrigger[Node] = true;
	}
}

void USkillsRuntimeSubsystem::AddStartSkillNode(USkillNode* Node)
{
	if (Node->GetNodeType() != ESkillNodeType::StartNode) return ;

	StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].NodeMap.Add(Node, FTimerHandle());
	StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].CanTrigger.Add(Node, true);
}

void USkillsRuntimeSubsystem::RemoveStartSkillNode(USkillNode* Node)
{
	if (Node->GetNodeType() != ESkillNodeType::StartNode) return ;
	
	UGameInstance* const& GameInstance = GetGameInstance();
	checkf(GameInstance != nullptr, TEXT("Millenarysnow : GameInstance in SkillRuntimeSubsystem is nullptr."));
	
	FTimerHandle* TimerHandlePtr = StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].NodeMap.Find(Node);
	if (TimerHandlePtr && TimerHandlePtr->IsValid())
	{
		GameInstance->GetTimerManager().ClearTimer(*TimerHandlePtr);
	}
	
	StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].NodeMap.Remove(Node);
	StartSkillNodeByMap[static_cast<int>(Node->GetNodeInfo().StartNodeType)].CanTrigger.Remove(Node);
}

USkillsManagerSubsystem* USkillsRuntimeSubsystem::GetSkillsManagerSubsystem()
{
	if (!SkillsManagerSubsystem)
	{
		SkillsManagerSubsystem = GetGameInstance()->GetSubsystem<USkillsManagerSubsystem>();
		checkf(SkillsManagerSubsystem, TEXT("Millenarysnow : SkillsManagerSubsystem in SkillRuntimeSubsystem is nullptr."));
	}
	return SkillsManagerSubsystem;
}


