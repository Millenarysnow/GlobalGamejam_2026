// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillSpawnSubsystem.generated.h"

class USkillUISubsystem;
class UGoldSystem;
class USkillsManagerSubsystem;
class USkillNode;
struct FSkillNodeInfo;
/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API USkillSpawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 调试函数

	UFUNCTION(BlueprintCallable)
	void Test1();

	UFUNCTION(Blueprintable)
	void Test2();

	UFUNCTION(Blueprintable)
	void Test3();

	// -- 局中真正调用的函数 --
	
	UFUNCTION(BlueprintCallable)
	void NewNode(const FSkillNodeInfo& NodeInfo, TSubclassOf<USkillNode> SkillNodeClass);

	UFUNCTION(BlueprintCallable)
	void ClearAllNodeInformation();

private:
	UPROPERTY()
	USkillsManagerSubsystem* SkillsManagerSubsystem;

	UPROPERTY()
	USkillUISubsystem* SkillUISubsystem;
	
	UPROPERTY()
	UGoldSystem* GoldSystem;
};
