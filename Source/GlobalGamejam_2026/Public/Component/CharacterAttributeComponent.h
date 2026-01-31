// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CharacterAttributeComponent.generated.h"

/**
 * 用于挂载在角色和敌人身上，存储其属性信息
 */
UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
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


#pragma region UTIL

	float GetHealth() const { return CurrentHealth; }
	float GetMaxHealth() const { return MaxHealth; }
	float GetSpeed() const { return CurrentSpeed; }
	float GetMaxSpeed() const { return MaxSpeed; }
	float GetDamage() const { return CurrentDamage; }
	float GetMaxDamage() const { return MaxDamage; }
	float GetAttackSpeed() const { return CurrentAttackSpeed; }
	float GetMaxAttackSpeed() const { return MaxAttackSpeed; }

	// void SetHealth(float h) { CurrentHealth = h; }
	// void SetSpeed(float s) { CurrentSpeed = s; }
	// void SetDamage(float d) { CurrentDamage = d; }
	// void SetAttackSpeed(float a) { CurrentAttackSpeed = a; }

	void ModifySpeedRate(float rate);
	void ModifyDamageRate(float rate);
	void ModifyAttackSpeedRate(float rate);

	void ModifyHealth(float DeltaValue);
	
#pragma endregion

protected:

	// 属性上限
	
	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float MaxSpeed = 100.f;

	UPROPERTY(EditdefaultsOnly, Blueprintable)
	float MaxDamage = 100.f;

	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float MaxAttackSpeed = 5.f;

	// 初始属性值
	
	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float InitHealth = 50.f;

	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float InitSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float InitDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, Blueprintable)
	float InitAttackSpeed = 1.f;

private:
	float CurrentHealth = 50.f;
	float CurrentSpeed = 50.f;
	float CurrentDamage = 20.f;
	float CurrentAttackSpeed = 1.f;

	// 临时属性修改倍率 （属性*倍率 = 当前属性值，倍率间为相加关系）
	
	float SpeedModifyRate = 1.f;
	float DamageModifyRate = 1.f;
	float AttackSpeedModifyRate = 1.f;
};
