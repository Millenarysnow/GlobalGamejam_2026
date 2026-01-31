#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy.h"
#include "RangedEnemy.generated.h"

class AEnemyBullet; 

UCLASS()
class GLOBALGAMEJAM_2026_API ARangedEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ARangedEnemy();
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AEnemyBullet> BulletClass;

	float AttackCooldown = 0.f;

	void Shoot(AActor* Target);
};