#pragma once

#include "CoreMinimal.h"
#include "Actor/GeneratedActor.h"
#include "GeneratedBullet.generated.h"


class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class GLOBALGAMEJAM_2026_API AGeneratedBullet : public AGeneratedActor
{
	GENERATED_BODY()

public:
	AGeneratedBullet();

protected:
	virtual void BeginPlay() override;

	// 碰撞回调
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;
};
