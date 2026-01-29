// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Gameplay/Skills/SkillNode.h"

#include "ExecuteSkillComponent.generated.h"


class USkillNode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLNODESYSTEM_API UExecuteSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UExecuteSkillComponent();

#pragma region SkillInterface

	// 血量是否低于阈值
	virtual bool IfBloodLow();

	// 是否未接触地面
	virtual bool IfInAir() const;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal) { }

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal) { }

#pragma endregion

#pragma region UTIL

	void SetCurrentSkillNode(USkillNode* Node);
	void SetCurrentSkillContext(TSharedPtr<FSkillExecutionContext> Context);

#pragma endregion

protected:
	virtual void BeginPlay() override;
	
	// 获取到目标对象后必须手动调用，否则无法触发后续技能链
	void TriggerSkillChain(AActor* TargetActor);

private:
	UPROPERTY()
	USkillNode* CurrentSkillNode;

	TSharedPtr<FSkillExecutionContext> CurrentSkillContext;
};
