#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class GLOBALGAMEJAM_2026_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	AMeleeEnemy();
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 120.f; // 近战攻击距离

	float AttackCooldown = 0.f;

	void TryAttack(AActor* Target);
};