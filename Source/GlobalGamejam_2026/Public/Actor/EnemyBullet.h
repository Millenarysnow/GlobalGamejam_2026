#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GLOBALGAMEJAM_2026_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyBullet();

	// 属性（由发射者赋值）
	float Damage = 10.f;
	float SlowDuration = 2.0f;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
};