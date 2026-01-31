// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/GoldSystem.h"

#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Subsystem/SkillSpawnSubsystem.h"

void UGoldSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SkillsManagerSubsystem = GetGameInstance()->GetSubsystem<USkillsManagerSubsystem>();
	if (!SkillsManagerSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("GoldSystem: SkillsManagerSubsystem not found!"));
	}
}

void UGoldSystem::Settlement()
{
	SetGold_OfAll(GoldInInventory + Gold_InGame);
	SetGold_InGame(0.f);

	GetGameInstance()->GetSubsystem<USkillSpawnSubsystem>()->ClearAllNodeInformation();
}

void UGoldSystem::AddNewNodeGold_InGame(USkillNode* NewNode)
{
	float NodeGold = 0.f;
	
	// TODO: 计算节点的金币收益

	AddGold_InGame(NodeGold);
}

void UGoldSystem::AddGold_InGame(float AddGold)
{
	SetGold_InGame(Gold_InGame + AddGold);
}

void UGoldSystem::SubtractGold_InGame(float SubtractGold)
{
	SetGold_InGame(FMath::Max(0.f, Gold_InGame - SubtractGold));
}

float UGoldSystem::GetGold_InGame()
{
	return Gold_InGame;
}

void UGoldSystem::SetGold_InGame(float NewGold)
{
	Gold_InGame = NewGold;
}

float UGoldSystem::GetGold_OfAll()
{
	return GoldInInventory;
}

void UGoldSystem::SetGold_OfAll(float NewGold)
{
	GoldInInventory = NewGold;
}
