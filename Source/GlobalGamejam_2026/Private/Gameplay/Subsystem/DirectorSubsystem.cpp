// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/DirectorSubsystem.h"

#include "VNGameSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameSession.h"
#include "Gameplay/Subsystem/GoldSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/StatementUI.h"

void UDirectorSubsystem::Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld, TSubclassOf<UStatementUI> _InStatementUIClass, TSubclassOf<UUserWidget> _InVNMainWidgetClass)
{
	// 初始化逻辑

	if (LbWorld == nullptr)
		LbWorld = _LbWorld;
	if (GmWorld == nullptr)
		GmWorld = _GmWorld;
	StatementUIClass = _InStatementUIClass;
	VNMainWidgetClass = _InVNMainWidgetClass;
}

void UDirectorSubsystem::SwitchToVN()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	VNUI = CreateWidget(PC, VNMainWidgetClass);
	VNUI->AddToViewport();
    GetGameInstance()->GetSubsystem<UVNGameSubsystem>()->StartChapter(1, 1);
}

void UDirectorSubsystem::SwitchToLobby()
{
	if (VNUI)
	{
		VNUI->RemoveFromParent();
		VNUI = nullptr;
	}
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

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	}
}

void UDirectorSubsystem::PlayerDead()
{
	UGoldSystem* GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (GoldSystem)
	{
		GoldSystem->SetGold_InGame(0);
		GoldSystem->Settlement();
	}

	// 获取玩家控制器
	// UGameInstanceSubsystem 也可以通过 GetWorld() 获取世界上下文
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
	if (PC)
	{
		// 禁止输入 (Disable Input)
		// 这会阻止玩家角色的移动和操作，但不会停止 Tick
		PC->DisableInput(PC);

		// 显示鼠标光标
		// 死亡后通常需要点击“重试”或“返回大厅”，所以需要释放鼠标
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly()); // 切换到仅UI模式，防止鼠标点击影响场景
	}

	// 暂停游戏 (Pause Game)
	// 这会停止所有 Actor 的 Tick（除非设置了 bTickEvenWhenPaused），停止物理模拟
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 显示失败UI
	UUserWidget* StatementUI = CreateWidget(PC, StatementUIClass);
	Cast<UStatementUI>(StatementUI)->SetState(EStatementState::Lose);
	StatementUI->AddToViewport();
}

void UDirectorSubsystem::PassPerGame()
{
	UGoldSystem* GoldSystem = GetGameInstance()->GetSubsystem<UGoldSystem>();
	if (GoldSystem)
		GoldSystem->Settlement();

	// 获取玩家控制器
	// UGameInstanceSubsystem 也可以通过 GetWorld() 获取世界上下文
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
	if (PC)
	{
		// 禁止输入 (Disable Input)
		// 这会阻止玩家角色的移动和操作，但不会停止 Tick
		PC->DisableInput(PC);

		// 显示鼠标光标
		// 死亡后通常需要点击“重试”或“返回大厅”，所以需要释放鼠标
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly()); // 切换到仅UI模式，防止鼠标点击影响场景
	}

	// 暂停游戏 (Pause Game)
	// 这会停止所有 Actor 的 Tick（除非设置了 bTickEvenWhenPaused），停止物理模拟
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 显示失败UI
	UUserWidget* StatementUI = CreateWidget(PC, StatementUIClass);
	Cast<UStatementUI>(StatementUI)->SetState(EStatementState::Win);
	StatementUI->AddToViewport();
}

void UDirectorSubsystem::WinTheGame()
{
	// TODO : 胜利逻辑（显示胜利UI、然后返回主界面）
}

void UDirectorSubsystem::SetCurrentErosionValue(float NewErosionValue)
{
	CurrentErosionValue = FMath::Clamp(NewErosionValue, 0.f, MaxErosionValue);

	if (OnErosionChanged.IsBound())
	{
		OnErosionChanged.Broadcast(CurrentErosionValue, MaxErosionValue);
	}		
}

float UDirectorSubsystem::GetErosionRate()
{
	return FMath::Clamp((CurrentErosionValue + 1.f) / (MaxErosionValue + 1.f) * 100.f, 0.f, 100.f);
}

float UDirectorSubsystem::GetErosionValue()
{
	return CurrentErosionValue;
}

float UDirectorSubsystem::GetMaxErosionValue()
{
	return MaxErosionValue;
}
