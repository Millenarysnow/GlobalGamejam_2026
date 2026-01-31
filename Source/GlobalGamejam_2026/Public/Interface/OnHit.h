#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnHit.generated.h"

UINTERFACE()
class UOnHit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLOBALGAMEJAM_2026_API IOnHit
{
	GENERATED_BODY()

public:
	// 受击函数：传入伤害量和施法者
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGetHit(float Damage, AActor* InstigatorActor);

	virtual void OnGetHit_Implementation(float Damage, AActor* InstigatorActor) = 0;
};
