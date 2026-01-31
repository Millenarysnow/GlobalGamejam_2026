#pragma once

#include "CoreMinimal.h"
#include "Actor/GeneratedActor.h"
#include "GeneratedExplosion.generated.h"


class USphereComponent;


UCLASS()
class GLOBALGAMEJAM_2026_API AGeneratedExplosion : public AGeneratedActor
{
	GENERATED_BODY()

public:
	AGeneratedExplosion();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* ExplosionSphere;
};
