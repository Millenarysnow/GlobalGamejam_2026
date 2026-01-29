#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "VNTypes.generated.h"

// --- 对应 CSV: DT_Chapter ---
USTRUCT(BlueprintType)
struct FVNChapterRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Text; // 对话文本

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BackgroundImg; // 背景ID

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CharacterImg; // 立绘ID

	// SP1: 0=普通, 1=演出, 2=按钮, 4=跳转
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SP1;

	// SP2: 跳转逻辑 或 下一行ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SP2;
};

// --- 游戏状态枚举 ---
UENUM(BlueprintType)
enum class EVNState : uint8
{
	Idle,           // 初始状态
	Typing,         // 正在打字输出
	WaitingInput,   // 等待玩家点击（继续）
	Selecting,      // 等待玩家选择分支
	Animating       // 特殊演出中
};