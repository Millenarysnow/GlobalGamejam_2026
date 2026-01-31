// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerBase.h"
#include "HeroController.generated.h"

class USkillsRuntimeSubsystem;
/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API AHeroController : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void LeftMousePressed();
	void RightMousePressed();
	
private:
	UPROPERTY()
	USkillsRuntimeSubsystem* SkillsRuntimeSubsystem;
};
