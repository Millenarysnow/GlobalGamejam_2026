// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Skills/ChildNodes/StartNode.h"

#include "GameFramework/Character.h"

void UStartNode::Start()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	checkf(PlayerController, TEXT("Millenarysnow : Start node cant get player controller"));

	ACharacter* PlayerCharacter = PlayerController->GetCharacter();
	checkf(PlayerCharacter, TEXT("Millenarysnow : Start node cant get player character"));

	TSharedPtr<FSkillExecutionContext> Context = MakeShared<FSkillExecutionContext>();
	Context->SetTarget(PlayerCharacter);

	Trigger(Context);
}
