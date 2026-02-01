#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractActor.generated.h"

class AHero;

UCLASS()
class GLOBALGAMEJAM_2026_API AInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractActor();

	// 交互接口，由 AHero 调用
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AHero* Interactor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
};