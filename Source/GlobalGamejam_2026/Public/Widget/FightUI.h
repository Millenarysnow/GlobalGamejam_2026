// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FightUI.generated.h"

class UCharacterAttributeComponent;
class UTextBlock;
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
	void InitializeWidget(UCharacterAttributeComponent* AttributeComp);
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ErosionImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BloodImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BloodText;

	UFUNCTION(BlueprintCallable)
	void FlushErosionUI();

	UFUNCTION(BlueprintCallable)
	void FlushBloodUI(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	AHero* GetPlayer();

	UFUNCTION(BlueprintCallable)
	UDirectorSubsystem* GetDirectorSubsystem();

	UFUNCTION()
	void HandleAttributeCompReady(UCharacterAttributeComponent* NewComp);
	
private:
	UPROPERTY()
	AHero* Player;

	UPROPERTY()
	UMaterialInstanceDynamic* BloodMaterialInst;

	UPROPERTY()
	UDirectorSubsystem* DirectorSubsystem;
};
