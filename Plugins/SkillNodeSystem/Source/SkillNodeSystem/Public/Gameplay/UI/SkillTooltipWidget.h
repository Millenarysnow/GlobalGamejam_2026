#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTooltipWidget.generated.h"

class USkillNode;
class UTextBlock;
class UVerticalBox;

/**
 * 节点悬停提示框
 */
UCLASS(Abstract)
class SKILLNODESYSTEM_API USkillTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 更新提示框显示的内容
	UFUNCTION(BlueprintCallable)
	void UpdateTooltipData(USkillNode* InNode);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NodeNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NodeDescText;

	// 用于动态添加属性文本的容器
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PropertyListContainer;

private:
	// 辅助函数：添加一行属性文本
	void AddPropertyRow(const FString& Label, const FString& Value);
	void AddPropertyRow(const FString& Label, float Value);
	void AddPropertyRow(const FString& Label, int32 Value);
};