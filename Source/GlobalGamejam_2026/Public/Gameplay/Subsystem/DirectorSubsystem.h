// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DirectorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UDirectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld);

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
};
