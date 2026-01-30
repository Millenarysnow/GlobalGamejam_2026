// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "TimerManager.h"

#include "SkillsRuntimeSubsystem.generated.h"

class USkillsManagerSubsystem;
enum class EStartNodeType:uint8;
class USkillNode;
struct FTimerHandle;


USTRUCT()
struct FStartSkillNodeMap
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<USkillNode*, FTimerHandle> NodeMap;

	UPROPERTY()
	TMap<USkillNode*, bool> CanTrigger;
};


/**
 * 
 */
UCLASS()
class SKILLNODESYSTEM_API USkillsRuntimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * 解锁所有开始节点
	 */
	void LaunchAllStartNode();

	/**
	 * 锁定所有开始节点
	 */
	void StopAllStartNode();

	/**
	 * 触发开始节点
	 */
	void TriggerStartNode(EStartNodeType StartNodeType);

	UFUNCTION()
	static USkillsRuntimeSubsystem* Get(const UObject* WorldContextObject);

	void SetCooldownTrue(USkillNode* Node);

#pragma region UTIL
	
	void AddStartSkillNode(USkillNode* Node);
	void RemoveStartSkillNode(USkillNode* Node);

#pragma endregion
	
private:

	UPROPERTY()
	TMap<int32, FStartSkillNodeMap> StartSkillNodeByMap;

	UPROPERTY()
	USkillsManagerSubsystem* SkillsManagerSubsystem = nullptr;

	USkillsManagerSubsystem* GetSkillsManagerSubsystem();
};
