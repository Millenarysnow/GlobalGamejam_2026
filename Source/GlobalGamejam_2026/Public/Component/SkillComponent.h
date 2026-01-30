// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Component/ExecuteSkillComponent.h"

#include "SkillComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOBALGAMEJAM_2026_API USkillComponent : public UExecuteSkillComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

#pragma region SkillInterface

	// 血量是否低于阈值
	virtual bool IfBloodLow() override;

	// 是否未接触地面
	virtual bool IfInAir() const override;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal) override;

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal) override;

#pragma endregion

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
