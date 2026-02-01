#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "WinGameInteractActor.generated.h"

UCLASS()
class GLOBALGAMEJAM_2026_API AWinGameInteractActor : public AInteractActor
{
	GENERATED_BODY()
	
public:
	virtual void OnInteract_Implementation(AHero* Interactor) override;
};