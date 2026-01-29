// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/SkillNode.h"
#include "BranchNode.generated.h"


/**
 * 
 */
UCLASS()
class SKILLNODESYSTEM_API UBranchNode : public USkillNode
{
	GENERATED_BODY()

public:
	//~ Begin USkillNode Interface.
	virtual bool Branch(const AActor* Target) override;
	//~ End USkillNode Interface.
};
