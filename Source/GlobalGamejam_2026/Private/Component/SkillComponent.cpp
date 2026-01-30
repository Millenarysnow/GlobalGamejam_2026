// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"

#include "Component/CharacterAttributeComponent.h"


USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USkillComponent::IfBloodLow()
{
	return true;
}

bool USkillComponent::IfInAir() const
{
	return false;
}

void USkillComponent::EffectBuff(FNodeBuffValueFinal BuffValueFinal)
{
	// 1. 获取 Owner 身上的属性组件
	AActor* Owner = GetOwner();
	if (!Owner) return;

	UCharacterAttributeComponent* AttrComp = Owner->FindComponentByClass<UCharacterAttributeComponent>();
	if (!AttrComp)
	{
		// 如果你的设计中 CharacterBase 必定有该组件，可以用 GetComponentByClass
		return;
	}

	// 2. 根据 Buff 类型分发逻辑
	// FNodeBuffValueFinal 中的 Strength, Accelerate 等已经在 BuffNode 中计算了累加值
	// Strength = NodeValue * (AccDamage + 1) * 10
	// Accelerate = NodeValue * (AccReduceDelay + 1)
    
	switch (BuffValueFinal.BuffType)
	{
	case EBuffType::Accelerate:
		// 加速通常指移动速度，使用 Accelerate 字段
		AttrComp->BuffSpeed(BuffValueFinal.Accelerate, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::Strength:
		// 力量通常指攻击力，使用 Strength 字段
		AttrComp->BuffDamage(BuffValueFinal.Strength, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::AttackSpeed:
		// 攻速提升。
		// 这里需要权衡使用哪个字段。通常 "加速(Accelerate)" 字段在语义上包含时间/速度相关的加成
		// 且你的 BuffNode.cpp 中 Accelerate 计算使用了 AccReduceDelayTimeValue (减少延迟即增加频率)
		// 所以这里使用 Accelerate 字段作为攻速倍率
		AttrComp->BuffAttackSpeed(BuffValueFinal.Accelerate, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::None:
	case EBuffType::Size: // 弃用
	default:
		break;
	}
    
	// Debug Log (可选)
	// UE_LOG(LogTemp, Log, TEXT("Applied Buff: %s, Duration: %f"), 
	//    *UEnum::GetValueAsString(BuffValueFinal.BuffType), BuffValueFinal.BuffDurationTime);
}

void USkillComponent::GenerateItem(FNodeGenerateValueFinal GenerateValueFinal)
{
	//TODO : 实现生成物逻辑
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

