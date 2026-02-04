#include "VNMainWidget.h"

#include "Kismet/GameplayStatics.h"
#include "VNGameSubsystem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UVNMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. 获取 Subsystem
    UGameInstance* GI = GetGameInstance();
    if (GI)
    {
        VNSubsystem = GI->GetSubsystem<UVNGameSubsystem>();
    }

    // 2. 绑定委托 (这是解耦的关键！)
    if (VNSubsystem)
    {
        VNSubsystem->OnTextUpdate.AddDynamic(this, &UVNMainWidget::HandleTextUpdate);
        VNSubsystem->OnShowOptions.AddDynamic(this, &UVNMainWidget::HandleShowOptions);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("VNMainWidget: Failed to get VNGameSubsystem"));
    }
}

void UVNMainWidget::OnScreenClicked()
{
    if (!VNSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("VNMainWidget: VNSubsystem is null in OnScreenClicked"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("VNMainWidget: Screen clicked. IsTyping=%s"), bIsTyping ? TEXT("True") : TEXT("False"));
    
    // 核心交互逻辑：
    // 如果正在打字 -> 快进 (InstantFinish)
    // 如果不在打字 -> 通知 Subsystem 进行下一步 (TryNextStep)

    if (bIsTyping)
    {
        InstantFinish();
    }
    else
    {
        VNSubsystem->TryNextStep();
    }
}

void UVNMainWidget::StartChapter(int32 ChapterID, int32 StartIndex)
{
    GetGameInstance()->GetSubsystem<UVNGameSubsystem>()->StartChapter(ChapterID, StartIndex);
}

void UVNMainWidget::HandleTextUpdate(const FString& Text, int32 SpeakerID, int32 BgID, int32 CharID)
{
    // 处理背景图、立绘的更新

    /**
     * BgID 和 CharID 的约定：
     * 0 = 隐藏该图像
     * 1 = 第一张图片
     * 2 = 第二张图片
     * ...
     */

    UE_LOG(LogTemp, Log, TEXT("VNMainWidget: HandleTextUpdate BgID=%d CharID=%d Text=%s"), BgID, CharID, *Text);

    if (BgID < 0 || BgID > BackgroundImages.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid BgID: %d"), BgID);
        return;
    }

    if (CharID < 0 || CharID > CharacterImages.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid CharID: %d"), CharID);
        return;
    }

    if (BgID == 0)
    {
        Image_Background->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        Image_Background->SetVisibility(ESlateVisibility::Visible);
        Image_Background->SetBrushFromTexture(BackgroundImages[BgID]);
    }

    if (CharID == 0)
    {
        Image_Character->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        Image_Character->SetVisibility(ESlateVisibility::Visible);
        Image_Character->SetBrushFromTexture(CharacterImages[CharID]);
    }

    // 开始打字
    
    StartTypewriter(Text);
}

void UVNMainWidget::StartTypewriter(FString Content)
{
    TargetText = Content;
    CurrentCharIndex = 0;
    bIsTyping = true;
    
    if (Text_Dialogue) Text_Dialogue->SetText(FText::GetEmpty());

    // 启动 Timer，每 0.05 秒打一个字
    GetWorld()->GetTimerManager().SetTimer(TypewriterTimer, this, &UVNMainWidget::OnTypewriterTick, 0.05f, true);
}

void UVNMainWidget::OnTypewriterTick()
{
    CurrentCharIndex++;
    if (Text_Dialogue)
    {
        Text_Dialogue->SetText(FText::FromString(TargetText.Left(CurrentCharIndex)));
    }

    if (CurrentCharIndex >= TargetText.Len())
    {
        InstantFinish();
    }
}

void UVNMainWidget::InstantFinish()
{
    GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
    bIsTyping = false;

    if (Text_Dialogue)
    {
        Text_Dialogue->SetText(FText::FromString(TargetText));
    }

    // 关键：通知 Subsystem 打字结束，状态切换为 WaitingInput
    if (VNSubsystem)
    {
        VNSubsystem->NotifyTypingFinished();
    }
}

void UVNMainWidget::HandleShowOptions(int32 BranchID)
{
    // 根据 BranchID 生成按钮...
    // 每个按钮被点击时，调用 VNSubsystem->SelectOption(TargetID);
}