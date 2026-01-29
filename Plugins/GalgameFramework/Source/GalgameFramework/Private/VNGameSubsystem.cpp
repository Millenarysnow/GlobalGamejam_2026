#include "VNGameSubsystem.h"

#include "Engine/DataTable.h"


void UVNGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    CurrentState = EVNState::Idle;
}

void UVNGameSubsystem::StartChapter(int32 ChapterID, int32 StartIndex)
{
    CurrentChapterID = ChapterID;
    CurrentLineID = StartIndex;
    LoadRow(CurrentChapterID, CurrentLineID);
}

FName UVNGameSubsystem::GetRowName(int32 Chapter, int32 Line)
{
    // 构造 RowName，例如 "Chapter1_1"
    return FName(*FString::Printf(TEXT("Chapter%d_%d"), Chapter, Line));
}

void UVNGameSubsystem::SwitchState(EVNState NewState)
{
    switch (NewState)
    {
    case EVNState::Animating:
        CurrentState = EVNState::Idle;
        break;
    case EVNState::Idle:
        CurrentState = EVNState::Idle;
        break;
    case EVNState::Selecting:
        CurrentState = EVNState::Selecting;
        break;
    case EVNState::Typing:
        CurrentState = EVNState::Typing;
        break;
    case EVNState::WaitingInput:
        CurrentState = EVNState::WaitingInput;
        break;
    default:
        break;
    }
}

void UVNGameSubsystem::LoadRow(int32 Chapter, int32 Line)
{
    if (!ChapterDataTable) return;

    FString Context;
    FVNChapterRow* Row = ChapterDataTable->FindRow<FVNChapterRow>(GetRowName(Chapter, Line), Context);

    if (!Row)
    {
        UE_LOG(LogTemp, Warning, TEXT("VN: Row Not Found Chapter%d_%d"), Chapter, Line);
        return;
    }

    // --- 核心状态机逻辑  ---

    // SP1=4: 跳转 (Jump)
    if (Row->SP1 == 4)
    {
        // 假设 SP2 是目标行号，或者解析 Text 里的内容作为目标
        // 这里根据你的描述简单处理：
        int32 TargetLine = (Row->SP2 > 0) ? Row->SP2 : Line + 1;
        LoadRow(Chapter, TargetLine); 
        return; // 立即执行跳转，不更新UI
    }

    // SP1=2: 按钮 (Branch/Options)
    if (Row->SP1 == 2)
    {
        SwitchState(EVNState::Selecting);
        // 通知 UI 生成按钮 (传入 SP2 作为参数，UI 根据这个去查 Branch 表)
        if (OnShowOptions.IsBound())
            OnShowOptions.Broadcast(Row->SP2);
        return;
    }

    // SP1=1: 特殊演出 (Anim)
    if (Row->SP1 == 1)
    {
        SwitchState(EVNState::Animating);
        if (OnSpecialEvent.IsBound())
            OnSpecialEvent.Broadcast(Row->SP2); // SP2 是演出ID
        // 注意：演出结束后，UI 需要回调 TryNextStep() 来继续
        return;
    }

    // SP1=0: 普通对话 (Normal)
    // 更新状态
    SwitchState(EVNState::Typing);
    
    // 广播数据给 UI，让 UI 负责显示
    if (OnTextUpdate.IsBound())
    {
        // 这里假设 SpeakerID 可以从 Text 解析，或者你有单独字段
        // 为了演示，直接传 Background 和 Character
        OnTextUpdate.Broadcast(Row->Text, 0, Row->BackgroundImg, Row->CharacterImg);
    }
}

void UVNGameSubsystem::TryNextStep()
{
    // UI点击时调用此函数

    if (CurrentState == EVNState::Typing)
    {
        // 如果正在打字，不做逻辑跳转，而是让UI完成快进
        // 这里不需要做任何事，因为 UI 自己会判断 IsTyping 然后 InstantFinish
        // 但为了状态同步，可以在 NotifyTypingFinished 里切换状态
    }
    else if (CurrentState == EVNState::WaitingInput)
    {
        // 只有在“等待输入”时，点击才意味着“下一句”
        
        // 简单自增逻辑，你需要根据 SP2 实现具体的跳转规则
        // 例如：如果 SP2 > 0 则跳转到 SP2，否则 +1
        CurrentLineID++; 
        LoadRow(CurrentChapterID, CurrentLineID);
    }
}

void UVNGameSubsystem::NotifyTypingFinished()
{
    // UI 通知 Logic：打字机效果播完了
    if (CurrentState == EVNState::Typing)
    {
        SwitchState(EVNState::WaitingInput);
    }
}

void UVNGameSubsystem::SelectOption(int32 TargetChapterID)
{
    // UI 按钮点击后调用，传入目标章节
    CurrentState = EVNState::Idle;
    StartChapter(TargetChapterID, 1);
}