#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNodeWidget.generated.h"

class USkillNode;
class USkillUISubsystem;
class UTextBlock;
class USkillCanvasWidget;

/**
 * 单个节点的 UI 显示
 */
UCLASS(Abstract)
class SKILLNODESYSTEM_API USkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 初始化节点数据
	UFUNCTION(BlueprintCallable)
	void InitNodeData(USkillNode* InNode);
    
	UFUNCTION(BlueprintPure)
	int32 GetHashID() const;

protected:
	// --- 拖拽逻辑 ---
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	USkillNode* BindNode;

	// 在蓝图中绑定 TextBlock 显示名称
	UPROPERTY(meta = (BindWidget)) 
	UTextBlock* NodeNameText;
    
	// 拖拽时的可视 Widget 类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DragVisualClass;
};