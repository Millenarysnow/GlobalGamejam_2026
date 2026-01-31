#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Hero.generated.h"

UCLASS()
class GLOBALGAMEJAM_2026_API AHero : public ACharacterBase
{
	GENERATED_BODY()

public:
	AHero();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
