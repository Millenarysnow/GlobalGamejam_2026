// Source/SkillNodeSystem/Public/UI/SkillCanvasWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/UI/SkillNodeWidget.h"
#include "SkillCanvasWidget.generated.h"

class UCanvasPanel;

UCLASS()
class SKILLNODESYSTEM_API USkillCanvasWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 初始化技能树：创建所有节点 Widget
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void InitSkillTree();

	// 每一帧绘制连线
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

protected:
	// 处理拖拽放下：更新位置
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 在蓝图中实现：生成具体的 Node Widget 实例
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillUI")
	USkillNodeWidget* CreateNodeWidget(int32 HashID);

	// 在蓝图中实现：获取 Canvas Panel 的引用（用于添加子节点）
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillUI")
	UCanvasPanel* GetCanvasPanel() const;

private:
	// 缓存当前画布上的节点 Widget，Key 为 HashID
	UPROPERTY()
	TMap<int32, USkillNodeWidget*> NodeWidgetMap;
};