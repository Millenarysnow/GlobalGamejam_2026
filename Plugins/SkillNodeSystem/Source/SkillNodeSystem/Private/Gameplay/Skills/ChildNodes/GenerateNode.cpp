// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Skills/ChildNodes/GenerateNode.h"

#include "Gameplay/Component/ExecuteSkillComponent.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"

void UGenerateNode::Ability(TSharedPtr<FSkillExecutionContext> Context)
{
	AActor* Target = Context->GetTarget();
	if (NodeInfo.NodeType == ESkillNodeType::GenerateNode)
	{
		UExecuteSkillComponent* ExecuteSkillComponent = GetTargetExecuteSkillComponent(Target);
		if (!ExecuteSkillComponent) return;
		
		FNodeGenerateValueFinal GenerateValueFinal;

		GenerateValueFinal.GeneratedType = NodeInfo.GeneratedType;
		GenerateValueFinal.Damage = NodeInfo.Damage * (AccumulativeInfo.AccDamageValue + 1.0f) * USkillsManagerSubsystem::AdditionalParamForAttack;
		GenerateValueFinal.Radius = NodeInfo.Radius * (AccumulativeInfo.RadiusValue + 1.0f);

		ExecuteSkillComponent->SetCurrentSkillNode(this);
		ExecuteSkillComponent->SetCurrentSkillContext(Context);
		ExecuteSkillComponent->GenerateItem(GenerateValueFinal);
	}
}
