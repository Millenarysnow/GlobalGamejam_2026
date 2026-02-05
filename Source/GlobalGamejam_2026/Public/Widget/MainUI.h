// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UFUNCTION()
	void OnStartButtonReleased();

	UFUNCTION(BlueprintCallable)
	void BeginBling();

	UFUNCTION(BlueprintCallable)
	void StopBling();

	void SwitchBlingState();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool bPlayingAnim = false;

	float AnimTime = 0.f;
	float AnimFrame = 5.f;

	bool bBlingState = true;
};
