// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Component/ExecuteSkillComponent.h"

#include "SkillComponent.generated.h"


class AGeneratedExplosion;
class AGeneratedBullet;
class UCharacterAttributeComponent;


UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class GLOBALGAMEJAM_2026_API USkillComponent : public UExecuteSkillComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<AGeneratedBullet> GeneratedBulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<AGeneratedExplosion> GeneratedExplosionClass;

#pragma region SkillInterface

	// 血量是否低于阈值 // 暂时弃用
	virtual bool IfBloodLow() override;

	// 是否未接触地面 // 暂时弃用
	virtual bool IfInAir() const override;

	// 施加增益
	virtual void EffectBuff(FNodeBuffValueFinal BuffValueFinal) override;

	// 生成生成物
	virtual void GenerateItem(FNodeGenerateValueFinal GenerateValueFinal) override;

	void ReportSkillHit(AActor* HitTarget);

#pragma endregion

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	AActor* OwnerActor = nullptr;

	UPROPERTY()
	UCharacterAttributeComponent* AttributeComponent = nullptr;

	/** 临时修改移动速度 */
	void BuffSpeed(float Multiplier, float Duration);

	/** 临时修改攻击力 */
	void BuffDamage(float Multiplier, float Duration);

	/** 临时修改攻击速度 */
	void BuffAttackSpeed(float Multiplier, float Duration);

	// 用于计时结束后的回调
	void RevertSpeed(float Multiplier, FTimerHandle Handle);
	void RevertDamage(float Multiplier, FTimerHandle Handle);
	void RevertAttackSpeed(float Multiplier, FTimerHandle Handle);
};
