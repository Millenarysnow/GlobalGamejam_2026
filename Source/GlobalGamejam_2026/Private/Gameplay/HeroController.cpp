// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/HeroController.h"

#include "Gameplay/Skills/SkillDetailType.h"
#include "Gameplay/Subsystem/SkillsRuntimeSubsystem.h"

void AHeroController::BeginPlay()
{
	Super::BeginPlay();

	SkillsRuntimeSubsystem = GetGameInstance()->GetSubsystem<USkillsRuntimeSubsystem>();
	if (!SkillsRuntimeSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillsRuntimeSubsystem is not found!"));
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	// TODO: 暂时直接解锁节点，且没有锁定
	SkillsRuntimeSubsystem->LaunchAllStartNode();
}

void AHeroController::LeftMousePressed()
{
	SkillsRuntimeSubsystem->TriggerStartNode(EStartNodeType::LeftButton);
}

void AHeroController::RightMousePressed()
{
	SkillsRuntimeSubsystem->TriggerStartNode(EStartNodeType::RightButton);
}
