// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatementUI.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Gameplay/Subsystem/DirectorSubsystem.h"

void UStatementUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UStatementUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ContinueButton)
	{
		ContinueButton->OnReleased.AddDynamic(this, &UStatementUI::OnContinueButtonReleased);
	}
}

void UStatementUI::SetState(EStatementState State)
{
	switch (State)
	{
	case EStatementState::None:
		;
		break;
	case EStatementState::Win:
		BackgroundBorder->SetBrushFromTexture(SuccessTexture); // 胜利图片
		break;
	case EStatementState::Lose:
		BackgroundBorder->SetBrushFromTexture(FailureTexture); // 失败图片
		break;
	default:
		break;
	}
	PlaySFX(State);
}

void UStatementUI::OnContinueButtonReleased()
{
	GetGameInstance()->GetSubsystem<UDirectorSubsystem>()->SwitchToLobby();
	RemoveFromParent();
}

void UStatementUI::PlaySFX_Implementation(EStatementState State)
{
}
