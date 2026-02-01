// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DirectorSubsystem.generated.h"


class UStatementUI;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnErosionChanged, float, CurrentHealth, float, MaxHealth);



/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UDirectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld,
		TSubclassOf<UStatementUI> _InStatementUIClass);

	UPROPERTY()
	TSoftObjectPtr<UWorld> LbWorld;

	UPROPERTY()
	TSoftObjectPtr<UWorld> GmWorld;

	UFUNCTION(BlueprintCallable)
	void SwitchToLobby();

	UFUNCTION(BlueprintCallable)
	void SwitchToGame();

	UFUNCTION(BlueprintCallable)
	void PlayerDead();

	UFUNCTION(BlueprintCallable)
	void PassPerGame();
	
	UFUNCTION(BlueprintCallable)
	void WinTheGame();

	UFUNCTION(BlueprintCallable)
	void SetCurrentErosionValue(float NewErosionValue);

	// 0 - 100
	UFUNCTION(BlueprintCallable)
	float GetErosionRate();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnErosionChanged OnErosionChanged;

	float GetErosionValue();
	float GetMaxErosionValue();

private:
	float CurrentErosionValue = 0.f;
	float MaxErosionValue = 1000.f;

	UPROPERTY()
	TSubclassOf<UStatementUI> StatementUIClass;
};
