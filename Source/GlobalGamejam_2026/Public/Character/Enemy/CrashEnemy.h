#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy.h"
#include "CrashEnemy.generated.h"

UCLASS()
class GLOBALGAMEJAM_2026_API ACrashEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ACrashEnemy();
	virtual void Tick(float DeltaTime) override;

private:
	// 爆炸触发半径
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TriggerRadius = 150.f;

	// 爆炸伤害半径
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ExplosionRadius = 300.f;

	void SelfDestruct();
};