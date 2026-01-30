#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Skills/SkillType.h"
#include "SkillNodeWidget.generated.h"

class USkillNode;
class USkillUISubsystem;
class UTextBlock;
class USkillCanvasWidget;
class USkillPinWidget;

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

	// --- 连线支持接口 ---
	
	// 获取输入引脚的屏幕坐标 (假设只有一个输入)
	FVector2D GetInputPinPosition() const;

	// 获取指定分支类型的输出引脚屏幕坐标
	FVector2D GetOutputPinPosition(OnBranchNode BranchType) const;

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

protected: // 引脚
	// 【新增】输入引脚容器 (HorizontalBox/VerticalBox)
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* InputPinContainer;

	// 【新增】输出引脚容器 (HorizontalBox/VerticalBox)
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* OutputPinContainer;

	// 【新增】引脚 Widget 类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillPinWidget> PinWidgetClass;

	// 缓存引脚实例以便查询位置
	UPROPERTY()
	USkillPinWidget* InputPinWidget = nullptr;

	UPROPERTY()
	TMap<OnBranchNode, USkillPinWidget*> OutputPinWidgets;
};