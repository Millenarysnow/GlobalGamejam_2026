// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitByEnemy.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHitByEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 敌人攻击接口，玩家类应当实现此接口
 */
class GLOBALGAMEJAM_2026_API IHitByEnemy
{
	GENERATED_BODY()

public:
	/**
	 * 受击事件
	 * @param DamageAmount 造成的伤害值
	 * @param SlowDuration 减速持续时间（秒），<=0 代表不减速
	 * @param HitInstigator 伤害来源
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnIHit(float DamageAmount, float SlowDuration, AActor* HitInstigator);
};
