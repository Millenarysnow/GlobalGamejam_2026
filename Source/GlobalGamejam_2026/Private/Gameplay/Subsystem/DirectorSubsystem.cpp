// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/DirectorSubsystem.h"

#include "GameFramework/GameSession.h"
#include "Gameplay/Subsystem/GoldSystem.h"
#include "Kismet/GameplayStatics.h"

void UDirectorSubsystem::Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld)
{
	// TODO : 初始化逻辑

	if (LbWorld == nullptr)
		LbWorld = _LbWorld;
	if (GmWorld == nullptr)
		GmWorld = _GmWorld;
}

void UDirectorSubsystem::SwitchToLobby()
{
	if (LbWorld.ToSoftObjectPath().IsValid())
	{
		const FString MapName = LbWorld.ToSoftObjectPath().GetAssetName();
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));
	}
}

void UDirectorSubsystem::SwitchToGame()
{
	if (GmWorld.ToSoftObjectPath().IsValid())
	{
		const FString MapName = GmWorld.ToSoftObjectPath().GetAssetName();
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));
	}
}

void UDirectorSubsystem::PlayerDead()
{
	UGoldSystem* GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (!GoldSystem) return ;

	GoldSystem->SetGold_InGame(0);
	GoldSystem->Settlement();

	// TODO : 死亡逻辑（显示死亡UI、然后返回大厅）
}

void UDirectorSubsystem::PassPerGame()
{
	UGoldSystem* GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (!GoldSystem) return ;

	GoldSystem->Settlement();

	// TODO : 通关逻辑（显示通关UI、然后返回大厅）
}

void UDirectorSubsystem::WinTheGame()
{
	// TODO : 胜利逻辑（显示胜利UI、然后返回主界面）
}
