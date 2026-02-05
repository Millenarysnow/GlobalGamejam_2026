// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystem/DirectorSubsystem.h"

#include "VNGameSubsystem.h"
#include "Animation/AnimSubsystem_PropertyAccess.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameSession.h"
#include "Gameplay/Subsystem/GoldSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Widget/StatementUI.h"

void UDirectorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 监听 VN 子系统的事件
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UVNGameSubsystem* VNSubsystem = GameInstance->GetSubsystem<UVNGameSubsystem>();
		if (VNSubsystem)
		{
			VNSubsystem->OnSP1Event.AddDynamic(this, &UDirectorSubsystem::OnVNSP1Called);
		}
	}
}

void UDirectorSubsystem::Init(TSoftObjectPtr<UWorld> _LbWorld, TSoftObjectPtr<UWorld> _GmWorld, TSubclassOf<UStatementUI> _InStatementUIClass, TSubclassOf<UUserWidget> _InVNMainWidgetClass,
	USoundCue* _InMainBgm, USoundCue* _InLobbyBgm, USoundCue* _InFightBgm)
{
	// 初始化逻辑
	if (LbWorld == nullptr)
		LbWorld = _LbWorld;
	if (GmWorld == nullptr)
		GmWorld = _GmWorld;
	StatementUIClass = _InStatementUIClass;
	VNMainWidgetClass = _InVNMainWidgetClass;

	MainBgm = _InMainBgm;
	LobbyBgm = _InLobbyBgm;
	FightBgm = _InFightBgm;
}

void UDirectorSubsystem::SwitchToVN(int32 ChapterID)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	VNUI = CreateWidget(PC, VNMainWidgetClass);
	VNUI->AddToViewport();
    GetGameInstance()->GetSubsystem<UVNGameSubsystem>()->StartChapter(ChapterID, 1);
}

void UDirectorSubsystem::OnVNSP1Called(int32 SP1Value, int32 SP2Value)
{
	if (SP1Value == 10) // 需要切换场景
	{
		if (SP2Value == 1) // 序言：从主界面切换至大厅
		{
			if (VNUI)
			{
				VNUI->RemoveFromParent();
				VNUI = nullptr;
			}
			SwitchToLobby();
		}
		if (SP2Value == 2) // 结尾：从游戏切换至主界面
		{
			
		}
	}
}

void UDirectorSubsystem::SwitchToLobby()
{
	if (LbWorld.ToSoftObjectPath().IsValid())
	{
		const FString MapName = LbWorld.ToSoftObjectPath().GetAssetName();
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));

		PlayBGM(LobbyBgm);
	}
}

void UDirectorSubsystem::SwitchToGame()
{
	if (GmWorld.ToSoftObjectPath().IsValid())
	{
		const FString MapName = GmWorld.ToSoftObjectPath().GetAssetName();
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());

		// 重置并开启侵蚀值
		ResetErosionValue();
		if (StartErosionTimeline.IsBound())
		{
			StartErosionTimeline.Broadcast();
		}

		PlayBGM(FightBgm);
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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
	if (PC)
	{
		// 禁止输入 (Disable Input)
		PC->DisableInput(PC);

		// 显示鼠标光标
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly()); // 切换到仅UI模式，防止鼠标点击影响场景
	}

	// 停止侵蚀值变化
	if (StopErosionTimeline.IsBound())
	{
		StopErosionTimeline.Broadcast();
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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
	if (PC)
	{
		// 禁止输入 (Disable Input)
		// 这会阻止玩家角色的移动和操作，但不会停止 Tick
		PC->DisableInput(PC);

		// 显示鼠标光标
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly()); // 切换到仅UI模式，防止鼠标点击影响场景
	}

	// 暂停游戏 (Pause Game)
	// 这会停止所有 Actor 的 Tick（除非设置了 bTickEvenWhenPaused），停止物理模拟
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 停止侵蚀值变化
	if (StopErosionTimeline.IsBound())
	{
		StopErosionTimeline.Broadcast();
	}

	// 显示胜利UI
	UUserWidget* StatementUI = CreateWidget(PC, StatementUIClass);
	Cast<UStatementUI>(StatementUI)->SetState(EStatementState::Win);
	StatementUI->AddToViewport();
}

void UDirectorSubsystem::WinTheGame()
{
	// TODO : 胜利逻辑（显示胜利UI、然后返回主界面）
}

void UDirectorSubsystem::PlayBGM(USoundCue* BGMToPlay, float FadeInTime, float FadeOutTime)
{
	if (!BGMToPlay) return;

	// 1. 如果新音乐和当前音乐相同且正在播放，直接返回，实现跨关卡无缝无断点
	if (CurrentSound == BGMToPlay && BgmAudioComponent && BgmAudioComponent->IsPlaying())
	{
		return;
	}

	// 2. 如果已有音乐在播放，先淡出并清理
	if (BgmAudioComponent)
	{
		BgmAudioComponent->FadeOut(FadeOutTime, 0.0f);
		// 注意：FadeOut 不会立即销毁组件，旧组件会被垃圾回收处理
	}

	// 3. 播放新音乐
	CurrentSound = BGMToPlay;
	// 使用 SpawnSound2D 以便获取引用并控制其持久性
	BgmAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BGMToPlay, 1.0f, 1.0f, 0.0f, nullptr, true);
    
	if (BgmAudioComponent)
	{
		// 关键：确保在切换关卡时不会因为音量静音被意外停止
		BgmAudioComponent->bIsUISound = true; 
		BgmAudioComponent->FadeIn(FadeInTime);
	}
}

void UDirectorSubsystem::StopBGM(float FadeOutTime)
{
	if (BgmAudioComponent)
	{
		BgmAudioComponent->FadeOut(FadeOutTime, 0.0f);
		BgmAudioComponent = nullptr;
		CurrentSound = nullptr;
	}
}

void UDirectorSubsystem::ResetErosionValue()
{
	SetCurrentErosionValue(MaxErosionValue);
}

void UDirectorSubsystem::SetCurrentErosionValue(float NewErosionValue)
{
	CurrentErosionValue = FMath::Clamp(NewErosionValue, 0.f, MaxErosionValue);

	// 广播侵蚀值变化事件（用于UI）
	if (OnErosionChanged.IsBound())
	{
		OnErosionChanged.Broadcast(CurrentErosionValue, MaxErosionValue);
	}

	if (CurrentErosionValue >= MaxErosionValue)
	{
		// 侵蚀值达到最大，玩家死亡
		PlayerDead();
	}
}

void UDirectorSubsystem::SetCurrentErosionValueByRate(float ErosionRate)
{
	float NewErosionValue = FMath::Clamp(ErosionRate, 0.f, 100.f) / 100.f * MaxErosionValue;
	SetCurrentErosionValue(NewErosionValue);
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
