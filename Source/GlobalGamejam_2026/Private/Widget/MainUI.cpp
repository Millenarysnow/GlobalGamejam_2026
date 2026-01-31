// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainUI.h"

#include "Animation/DebugSkelMeshComponent.h"
#include "Components/Button.h"
#include "Gameplay/Subsystem/DirectorSubsystem.h"

void UMainUI::OnStartButtonReleased()
{
	StopBling();
	
	GetGameInstance()->GetSubsystem<UDirectorSubsystem>()->SwitchToLobby();

	RemoveFromParent();
}

void UMainUI::BeginBling()
{
	bPlayingAnim = true;

	AnimTime = 0.f;
}

void UMainUI::StopBling()
{
	bPlayingAnim = false;

	bBlingState = true;
	StartButton->SetVisibility(ESlateVisibility::Visible);
	AnimTime = 0.f;
}

void UMainUI::SwitchBlingState()
{
	bBlingState = !bBlingState;

	if (bBlingState)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StartButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnReleased.AddDynamic(this, &UMainUI::OnStartButtonReleased);
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayingAnim)
	{
		AnimTime += InDeltaTime;
		float delta = 1.f / AnimFrame;
		if (AnimTime >= delta)
		{
			AnimTime -= delta;
			SwitchBlingState();
		}
	}
}
