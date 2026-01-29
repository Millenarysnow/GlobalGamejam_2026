// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/SkillNode.h"
#include "GenerateNode.generated.h"

/**
 * 
 */
UCLASS()
class SKILLNODESYSTEM_API UGenerateNode : public USkillNode
{
	GENERATED_BODY()

public:
	//~ Begin USkillNode Interface.
	virtual void Ability(TSharedPtr<FSkillExecutionContext> Context) override;
	//~ End USkillNode Interface.
};
