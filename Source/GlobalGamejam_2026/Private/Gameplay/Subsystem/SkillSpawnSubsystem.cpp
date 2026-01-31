// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/SkillSpawnSubsystem.h"

#include "Gameplay/Subsystem/GoldSystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"

void USkillSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SkillsManagerSubsystem = GetGameInstance()->GetSubsystem<USkillsManagerSubsystem>();
	if (!SkillsManagerSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillSpawnSubsystem: SkillsManagerSubsystem not found!"));
	}

	GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (!GoldSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillSpawnSubsystem: GoldSystem not found!"));
	}

	SkillUISubsystem = GetGameInstance()->GetSubsystem<USkillUISubsystem>();
	if (!SkillUISubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillSpawnSubsystem: SkillUISubsystem not found!"));
	}
}

void USkillSpawnSubsystem::NewNode(const FSkillNodeInfo& NodeInfo, TSubclassOf<USkillNode> SkillNodeClass)
{
	USkillNode* Node = SkillsManagerSubsystem->NewSkillNode(NodeInfo, SkillNodeClass);

	GoldSystem->AddNewNodeGold_InGame(Node);
}

void USkillSpawnSubsystem::ClearAllNodeInformation()
{
	// 清理可视化层

	SkillUISubsystem->ClearNode();

	// 清理逻辑层

	SkillsManagerSubsystem->ClearAllNode();
}
