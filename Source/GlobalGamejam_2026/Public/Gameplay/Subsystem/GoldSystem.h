// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GoldSystem.generated.h"

class USkillNode;
class USkillsManagerSubsystem;
/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UGoldSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// 结算一回合金币
	UFUNCTION(BlueprintCallable)
	void Settlement();

	// 新增节点时结算金币（被Manager调用）
	UFUNCTION(BlueprintCallable)
	void AddNewNodeGold_InGame(USkillNode* NewNode);
	
	void AddGold_InGame(float AddGold);
	void SubtractGold_InGame(float SubtractGold);
	
	// 所有关于金币的操作接口，其余最终都会调用这两个接口
	float GetGold_InGame();
	void SetGold_InGame(float NewGold);

	// 总金币接口

	float GetGold_OfAll();
	void SetGold_OfAll(float NewGold);
	
private:
	float Gold_InGame = 0;
	float GoldInInventory = 0;

	UPROPERTY()
	USkillsManagerSubsystem* SkillsManagerSubsystem;
};
