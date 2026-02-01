#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "VNMainWidget.generated.h"

class UImage;
class UTextBlock;
class UVNGameSubsystem;

UCLASS()
class GALGAMEFRAMEWORK_API UVNMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
    
	// --- 绑定UI事件 ---
	// 假设你有一个全屏的透明按钮用于点击下一句
	UFUNCTION(BlueprintCallable)
	void OnScreenClicked();

	void StartChapter(int32 ChapterID, int32 StartIndex = 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> BackgroundImages; // 背景图数组

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> CharacterImages; // 角色立绘数组
	
	// --- 事件回调 (从 Subsystem 接收) ---
	UFUNCTION()
	void HandleTextUpdate(const FString& Text, int32 SpeakerID, int32 BgID, int32 CharID);

	UFUNCTION()
	void HandleShowOptions(int32 BranchID);

	// --- 打字机逻辑 ---
	void StartTypewriter(FString Content);
	void OnTypewriterTick();
	void InstantFinish();

	// --- 变量 ---
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Dialogue; // 绑定 UMG 中的文本框

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Background; // 绑定 UMG 中的背景图

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Character; // 绑定 UMG 中的角色立绘

	FTimerHandle TypewriterTimer;
	FString TargetText;
	int32 CurrentCharIndex;
	bool bIsTyping;

	// 缓存 Subsystem 指针
	UPROPERTY()
	UVNGameSubsystem* VNSubsystem;
};