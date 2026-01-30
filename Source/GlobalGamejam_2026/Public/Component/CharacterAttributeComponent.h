// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CharacterAttributeComponent.generated.h"

/**
 * 用于挂载在角色和敌人身上，存储其属性信息
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GLOBALGAMEJAM_2026_API UCharacterAttributeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/** 临时修改移动速度 */
	void BuffSpeed(float Multiplier, float Duration);

	/** 临时修改攻击力 */
	void BuffDamage(float Multiplier, float Duration);

	/** 临时修改攻击速度 */
	void BuffAttackSpeed(float Multiplier, float Duration);

private:
	float MaxHealth = 100.f;
	float Health;

	float MaxSpeed = 100.f;
	float Speed;

	float MaxDamage = 100.f;
	float Damage;

	float MaxAttackSpeed = 5.f;
	float AttackSpeed;

	// 内部辅助：用于计时结束后的回调
	void RevertSpeed(float Multiplier);
	void RevertDamage(float Multiplier);
	void RevertAttackSpeed(float Multiplier);
};
