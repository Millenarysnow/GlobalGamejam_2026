// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/CookEnums.h"
#include "DirectorSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnErosionChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartErosionTimeline);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopErosionTimeline);

class UVNMainWidget;
class UStatementUI;


/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UDirectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld,
		TSubclassOf<UStatementUI> _InStatementUIClass, TSubclassOf<UUserWidget> _InVNMainWidgetClass,
		USoundCue* _InMainBgm, USoundCue* _InLobbyBgm, USoundCue* _InFightBgm);

	UFUNCTION(BlueprintCallable)
	void SwitchToVN(int32 ChapterID);

	// 监听来自 VN 子系统的事件
	UFUNCTION()
	void OnVNSP1Called(int32 SP1Value, int32 SP2Value);
	
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

	// ------ BGM ------

	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundCue* BGMToPlay, float FadeInTime = 0.5f, float FadeOutTime = 0.5f);

	UFUNCTION(BlueprintCallable)
	void StopBGM(float FadeOutTime = 0.5f);

private:
	UPROPERTY()
	TSubclassOf<UStatementUI> StatementUIClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> VNMainWidgetClass;

	UPROPERTY()
	UUserWidget* VNUI;

	// --- BGM ---
	
	UPROPERTY()
	USoundCue* MainBgm;

	UPROPERTY()
	USoundCue* LobbyBgm;

	UPROPERTY()
	USoundCue* FightBgm;

	UPROPERTY()
	UAudioComponent* BgmAudioComponent;

	UPROPERTY()
	USoundCue* CurrentSound;

	
	// ------ 侵蚀值 -------
	
public:
	UFUNCTION(BlueprintCallable)
	void ResetErosionValue();

	UFUNCTION(BlueprintCallable)
	void SetCurrentErosionValue(float NewErosionValue);

	UFUNCTION(BlueprintCallable)
	void SetCurrentErosionValueByRate(float ErosionRate);

	// 0 - 100
	UFUNCTION(BlueprintCallable)
	float GetErosionRate();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnErosionChanged OnErosionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStartErosionTimeline StartErosionTimeline;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStopErosionTimeline StopErosionTimeline;

	float GetErosionValue();
	float GetMaxErosionValue();

private:
	float CurrentErosionValue = 0.f;
	float MaxErosionValue = 1000.f;

};
