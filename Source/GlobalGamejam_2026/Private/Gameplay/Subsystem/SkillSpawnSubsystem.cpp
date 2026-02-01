// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/SkillSpawnSubsystem.h"

#include "Gameplay/Skills/SkillChildType.h"
#include "Gameplay/Skills/ChildNodes/BuffNode.h"
#include "Gameplay/Skills/ChildNodes/GenerateNode.h"
#include "Gameplay/Skills/ChildNodes/StartNode.h"
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

void USkillSpawnSubsystem::Test1()
{
	FSkillNodeInfo StartNodeInfo;
	StartNodeInfo.NodeType = ESkillNodeType::StartNode;
	StartNodeInfo.SwitchTargetType = ESwitchTargetType::Unchanged;
	StartNodeInfo.DelayTime = 10.f;
	StartNodeInfo.OutPinCount = 1;
	StartNodeInfo.StartNodeType = EStartNodeType::LeftButton;
	
	NewNode(StartNodeInfo, UStartNode::StaticClass());

	FSkillNodeInfo GenerateNodeInfo; // = FGenerateNodeInfo(EGeneratedType::StandardBullet, 20.f, 100.f);
	GenerateNodeInfo.NodeType = ESkillNodeType::GenerateNode;
	GenerateNodeInfo.SwitchTargetType = ESwitchTargetType::Changed;
	GenerateNodeInfo.DelayTime = 100.f;
	GenerateNodeInfo.OutPinCount = 2;
	GenerateNodeInfo.GeneratedType = EGeneratedType::StandardBullet;
	GenerateNodeInfo.Damage = 20.f;
	GenerateNodeInfo.Radius = 100.f;

	NewNode(GenerateNodeInfo, UGenerateNode::StaticClass());

	FSkillNodeInfo BuffNodeInfo; // = FBuffNodeInfo(EBuffType::Accelerate, 5.f, 0.5f);
	BuffNodeInfo.NodeType = ESkillNodeType::BuffNode;
	BuffNodeInfo.SwitchTargetType = ESwitchTargetType::Unchanged;
	BuffNodeInfo.DelayTime = 100.f;
	BuffNodeInfo.OutPinCount = 2;
	BuffNodeInfo.BuffType = EBuffType::Accelerate;
	BuffNodeInfo.BuffDurationTime = 5.f;
	BuffNodeInfo.BuffValue = 0.5f;
	
	NewNode(BuffNodeInfo, UBuffNode::StaticClass());
}

void USkillSpawnSubsystem::Test2()
{
}

void USkillSpawnSubsystem::Test3()
{
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
