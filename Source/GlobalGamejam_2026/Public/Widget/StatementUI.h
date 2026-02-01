// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatementUI.generated.h"

class UButton;

UENUM(BlueprintType)
enum class EStatementState : uint8
{
	None,
	Win,
	Lose
};


class UBorder;
/**
 * 结算情况UI
 */
UCLASS()
class GLOBALGAMEJAM_2026_API UStatementUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statement UI")
	UTexture2D* SuccessTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statement UI")
	UTexture2D* FailureTexture;
	
	UFUNCTION(BlueprintCallable)
	void SetState(EStatementState State);

	UFUNCTION()
	void OnContinueButtonReleased();
};
