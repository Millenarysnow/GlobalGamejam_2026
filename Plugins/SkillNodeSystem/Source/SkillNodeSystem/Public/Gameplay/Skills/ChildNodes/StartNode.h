// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/SkillNode.h"
#include "StartNode.generated.h"

/**
 * 
 */
UCLASS()
class SKILLNODESYSTEM_API UStartNode : public USkillNode
{
	GENERATED_BODY()

public:
	//~ Begin USkillNode Interface.
	virtual void Start() override;
	//~ End USkillNode Interface.
};
