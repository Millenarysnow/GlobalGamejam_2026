#include "Actor/RandomEventInteractActor.h"
#include "Gameplay/Subsystem/SkillSpawnSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Gameplay/Skills/SkillNode.h" 
#include "Gameplay/Skills/ChildNodes/BuffNode.h"
#include "Gameplay/Skills/ChildNodes/GenerateNode.h"
#include "Gameplay/Skills/ChildNodes/StartNode.h"

void ARandomEventInteractActor::OnInteract_Implementation(AHero* Interactor)
{
	Super::OnInteract_Implementation(Interactor);

	bool bSpawnMonster = FMath::FRand() < SpawnMonsterProbability;

	if (bSpawnMonster)
	{
		// --- 事件 A: 消失并生成怪物 ---
		if (MonsterClasses.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, MonsterClasses.Num() - 1);
			TSubclassOf<ACharacter> MonsterClass = MonsterClasses[Index];

			if (MonsterClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				
				GetWorld()->SpawnActor<ACharacter>(MonsterClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			}
		}
		
		Destroy(); // 自身消失
	}
	else
	{
		// --- 事件 B: 给予玩家随机节点 ---
		if (UGameInstance* GI = GetGameInstance())
		{
			if (USkillSpawnSubsystem* SkillSys = GI->GetSubsystem<USkillSpawnSubsystem>())
			{
				// 生成随机节点信息
				FSkillNodeInfo NewNodeInfo = GenerateRandomNodeInfo();
				
				// 确保有节点类
				TSubclassOf<USkillNode> TargetClass = SkillNodeClass;
				switch (NewNodeInfo.NodeType)
				{
				case ESkillNodeType::GenerateNode:
					TargetClass = UGenerateNode::StaticClass(); 
					break;
				case ESkillNodeType::BuffNode:
					TargetClass = UBuffNode::StaticClass(); 
					break;
				case ESkillNodeType::StartNode:
					TargetClass = UStartNode::StaticClass();
					break;
				default:
					break;
				}
				
				SkillSys->NewNode(NewNodeInfo, TargetClass);
			}
		}
		
		Destroy(); // 获得奖励后消失
	}
}

FSkillNodeInfo ARandomEventInteractActor::GenerateRandomNodeInfo()
{
	FSkillNodeInfo Info;
	
	// 随机决定节点大类 (生成、Buff、参数)
	// 0: Generate, 1: Buff, 2: Param
	int32 RandomType = FMath::RandRange(0, 2);

	switch (RandomType)
	{
	case 0: // Generate Node
		Info.NodeType = ESkillNodeType::GenerateNode;
		// 随机生成物类型 (StandardBullet 或 Explosion)
		Info.GeneratedType = FMath::RandBool() ? EGeneratedType::StandardBullet : EGeneratedType::Explosion;
		Info.Damage = FMath::RandRange(10.f, 30.f);
		Info.Radius = FMath::RandRange(50.f, 150.f);
		break;

	case 1: // Buff Node
		Info.NodeType = ESkillNodeType::BuffNode;
		// 随机 Buff 类型
		Info.BuffType = FMath::RandBool() ? EBuffType::Accelerate : EBuffType::Strength;
		Info.BuffDurationTime = FMath::RandRange(3.f, 8.f);
		Info.BuffValue = FMath::RandRange(1.1f, 1.5f); // 10% - 50% 提升
		break;

	case 2: // Param Node
		Info.NodeType = ESkillNodeType::ParamNode;
		// 随机参数类型
		Info.ParamType = FMath::RandBool() ? EParamType::Damage : EParamType::EffectiveTime;
		if (Info.ParamType == EParamType::Damage)
		{
			Info.ParamFloatValue = FMath::RandRange(5.f, 20.f);
		}
		else
		{
			Info.ParamFloatValue = FMath::RandRange(0.5f, 2.0f);
		}
		break;
	}

	// 通用设置
	Info.DelayTime = FMath::RandRange(0.1f, 0.5f);
	Info.OutPinCount = 1; // 默认给一个输出引脚

	return Info;
}