#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCanvasWidget.generated.h"

class UCanvasPanel;
class USkillUISubsystem;
class USkillNodeWidget;

/**
 * 技能树画布
 */
UCLASS(Abstract)
class SKILLNODESYSTEM_API USkillCanvasWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
    
	// 绘制连线
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	// 刷新画布（重新生成节点）
	UFUNCTION(BlueprintCallable)
	void RefreshCanvas();

	// 通过屏幕坐标更新节点位置（拖拽松手/取消时使用）
	void ApplyNodeDrop(int32 NodeHashID, const FVector2D& ScreenPos, const FVector2D& DragOffset);

protected:
	// 处理节点拖拽松手后的位置更新
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillNodeWidget> NodeWidgetClass;

private:
	UPROPERTY()
	USkillUISubsystem* UISubsystem;

	// 缓存当前创建的 Widget，用于画线时查找坐标
	UPROPERTY()
	TMap<int32, USkillNodeWidget*> ActiveNodeWidgets;

	void DrawConnectionLine(const FGeometry& AllottedGeometry, const FVector2D& Start, const FVector2D& End, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
};