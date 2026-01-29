// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Skills/ChildNodes/BuffNode.h"

#include "Gameplay/Component/ExecuteSkillComponent.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"

void UBuffNode::Ability(TSharedPtr<FSkillExecutionContext> Context)
{
	AActor* Target = Context->GetTarget();
	if (NodeInfo.NodeType == ESkillNodeType::BuffNode)
	{
		FNodeBuffValueFinal BuffValueFinal;

		BuffValueFinal.BuffType = NodeInfo.BuffType;
		BuffValueFinal.BuffDurationTime = NodeInfo.BuffDurationTime * (AccumulativeInfo.AccEffectiveTimeValue + 1.0f);
		BuffValueFinal.Strength = NodeInfo.BuffValue * (AccumulativeInfo.AccDamageValue + 1.0f) * USkillsManagerSubsystem::AdditionalParamForAttack;
		BuffValueFinal.Radius = NodeInfo.BuffValue * (AccumulativeInfo.RadiusValue + 1.0f);
		BuffValueFinal.Accelerate = NodeInfo.BuffValue * (AccumulativeInfo.AccReduceDelayTimeValue + 1.0f);

		UExecuteSkillComponent* SkillComponent = GetTargetExecuteSkillComponent(Target);
		if (SkillComponent)
			SkillComponent->EffectBuff(BuffValueFinal);
	}
}


