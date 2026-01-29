#include "Gameplay/UI/SkillNodeWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void USkillNodeWidget::InitNode(int32 InHashID)
{
	HashID = InHashID;
	// 这里可以在蓝图中实现 UpdateView，更新图标、名称等
}

USkillNode* USkillNodeWidget::GetSkillNode() const
{
	USkillsManagerSubsystem* Manager = USkillsManagerSubsystem::Get(this);
	return Manager ? Manager->GetSkillNodeByHash(HashID) : nullptr;
}

FReply USkillNodeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 检测左键点击，准备拖拽
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void USkillNodeWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	USkillDragDropOperation* DragOp = NewObject<USkillDragDropOperation>();
	DragOp->HashID = HashID;
	DragOp->DefaultDragVisual = this; // 拖拽时显示自己，也可以做一个专用的 Visual Widget
	DragOp->Pivot = EDragPivot::CenterCenter;
	
	// 计算鼠标抓取点相对于 Widget 的偏移，防止拖拽时 Widget 瞬移
	DragOp->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	OutOperation = DragOp;
}