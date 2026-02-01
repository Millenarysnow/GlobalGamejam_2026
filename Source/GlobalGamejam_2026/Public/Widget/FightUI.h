// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FightUI.generated.h"

class UDirectorSubsystem;
class UImage;
class AHero;
class UMaterialInstanceDynamic;


/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UFightUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ErosionImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BloodImage;

	UFUNCTION(BlueprintCallable)
	void FlushErosionUI();

	UFUNCTION(BlueprintCallable)
	void FlushBloodUI(float CurrentHealthPercent);

	UFUNCTION(BlueprintCallable)
	AHero* GetPlayer();

	UFUNCTION(BlueprintCallable)
	UDirectorSubsystem* GetDirectorSubsystem();
	
private:
	UPROPERTY()
	AHero* Player;

	UPROPERTY()
	UMaterialInstanceDynamic* BloodMaterialInst;

	UPROPERTY()
	UDirectorSubsystem* DirectorSubsystem;
};
