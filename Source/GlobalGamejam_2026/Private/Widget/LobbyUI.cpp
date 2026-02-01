// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyUI.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Gameplay/Subsystem/DirectorSubsystem.h"
#include "Gameplay/Subsystem/GoldSystem.h"
#include "Kismet/KismetSystemLibrary.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (HelpButton)
	{
		HelpButton->OnPressed.AddDynamic(this, &ULobbyUI::OnHelpButtonPressed);
	}
	if (FightButton)
	{
		FightButton->OnPressed.AddDynamic(this, &ULobbyUI::OnFightButtonPressed);
	}
	if (ExitButton)
	{
		ExitButton->OnPressed.AddDynamic(this, &ULobbyUI::OnExitButtonPressed);
	}

	FlushState();
}

void ULobbyUI::FlushState()
{
	UGoldSystem* GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (!GoldSystem) return ;

	float TargetGold = GoldSystem->GetTargetGold();
	float CurrentGold = GoldSystem->GetGold_OfAll();

	float Ratio = FMath::Clamp(CurrentGold + 1 / TargetGold, 1.f, 99.f);

	int32 Index = FMath::Clamp((int32)Ratio / 25, 0, 3);
	
	Background->SetBrushFromTexture(BorderImages[Index] ? BorderImages[Index] : nullptr);
	HeroImage->SetBrushFromTexture(HeroImages[Index] ? HeroImages[Index] : nullptr);
}

void ULobbyUI::OnHelpButtonPressed()
{
	// ...
}

void ULobbyUI::OnFightButtonPressed()
{
	GetGameInstance()->GetSubsystem<UDirectorSubsystem>()->SwitchToGame();

	RemoveFromParent();
}

void ULobbyUI::OnExitButtonPressed()
{
	if (UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::QuitGame(World, GetOwningPlayer(), EQuitPreference::Quit, false);
	}
}
