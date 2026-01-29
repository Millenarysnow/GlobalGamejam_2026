#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VNTypes.h"
#include "VNGameSubsystem.generated.h"

// --- 定义委托：通知 UI 发生变化 ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnVNTextUpdate, const FString&, Text, int32, SpeakerID, int32, BgID, int32, CharID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVNShowOptions, int32, BranchID); // 通知UI显示选项
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVNSpecialEvent, int32, EventID); // 通知UI播放演出
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVNChapterEnd); // 章节结束

class UDataTable;

UCLASS()
class GALGAMEFRAMEWORK_API UVNGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // --- 供 UI 调用的操作接口 ---

    // 开始指定章节
    UFUNCTION(BlueprintCallable, Category = "VN Logic")
    void StartChapter(int32 ChapterID, int32 StartIndex = 1);

    // 玩家点击屏幕（用于推进剧情）
    UFUNCTION(BlueprintCallable, Category = "VN Logic")
    void TryNextStep();

    // 玩家选择了某个分支
    UFUNCTION(BlueprintCallable, Category = "VN Logic")
    void SelectOption(int32 TargetChapterID);

    // 设置当前的文本状态（UI打字结束时通知这里）
    UFUNCTION(BlueprintCallable, Category = "VN Logic")
    void NotifyTypingFinished();

    // --- 数据 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VN Data")
    UDataTable* ChapterDataTable; // 赋值你的 DT_Chapter

    // --- 状态 ---
    UPROPERTY(BlueprintReadOnly, Category = "VN State")
    EVNState CurrentState;

    // --- 事件分发 ---
    UPROPERTY(BlueprintAssignable, Category = "VN Events")
    FOnVNTextUpdate OnTextUpdate;

    UPROPERTY(BlueprintAssignable, Category = "VN Events")
    FOnVNShowOptions OnShowOptions;

    UPROPERTY(BlueprintAssignable, Category = "VN Events")
    FOnVNSpecialEvent OnSpecialEvent;

private:
    int32 CurrentChapterID;
    int32 CurrentLineID;

    // 内部函数：加载并解析一行数据
    void LoadRow(int32 Chapter, int32 Line);
    FName GetRowName(int32 Chapter, int32 Line);

    // 内部函数：切换状态机状态
    void SwitchState(EVNState NewState);
};