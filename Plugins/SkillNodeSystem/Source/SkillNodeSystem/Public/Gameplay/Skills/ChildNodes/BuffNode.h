// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/SkillNode.h"
#include "BuffNode.generated.h"


/**
 * 
 */
UCLASS()
class SKILLNODESYSTEM_API UBuffNode : public USkillNode
{
	GENERATED_BODY()

public:
	//~ Begin USkillNode Interface.
	virtual void Ability(TSharedPtr<FSkillExecutionContext> Context) override;
	//~ End USkillNode Interface.
};
