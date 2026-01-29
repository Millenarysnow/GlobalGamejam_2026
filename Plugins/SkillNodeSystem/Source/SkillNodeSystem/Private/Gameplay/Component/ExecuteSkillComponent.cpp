// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Component/ExecuteSkillComponent.h"

#include "Gameplay/Skills/SkillNode.h"


UExecuteSkillComponent::UExecuteSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	this->ComponentTags.Add(USkillNode::Tag_SkillComponent);
}

void UExecuteSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UExecuteSkillComponent::TriggerSkillChain(AActor* TargetActor)
{
	if (!CurrentSkillContext.IsValid()) return ;
	
	TSharedPtr<FSkillExecutionContext> Context = MakeShared<FSkillExecutionContext>(*CurrentSkillContext);
	Context->SetTarget(TargetActor);
	CurrentSkillNode->Delivery(Context);
}

bool UExecuteSkillComponent::IfBloodLow()
{
	return false;
}

bool UExecuteSkillComponent::IfInAir() const
{
	return false;
}

void UExecuteSkillComponent::SetCurrentSkillNode(USkillNode* Node)
{
	CurrentSkillNode = Node;
}

void UExecuteSkillComponent::SetCurrentSkillContext(TSharedPtr<FSkillExecutionContext> Context)
{
	CurrentSkillContext = Context;
}
