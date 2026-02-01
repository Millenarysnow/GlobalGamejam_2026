// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

class UButton;
class UBorder;
class UImage;

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM_2026_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UBorder* Background;

	UPROPERTY(meta = (BindWidget))
	UImage* HeroImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ColorValue;

	public:
	UPROPERTY(meta = (BindWidget))
	UButton* HelpButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FightButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> BorderImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> HeroImages;
	
	UFUNCTION(BlueprintCallable)
	void FlushState();
	
private:
	UFUNCTION()
	void OnHelpButtonPressed();

	UFUNCTION()
	void OnFightButtonPressed();

	UFUNCTION()
	void OnExitButtonPressed();
};
