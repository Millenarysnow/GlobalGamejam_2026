#include "Gameplay/UI/SkillNodeWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void USkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillNodeWidget::InitNodeData(USkillNode* InNode)
{
	BindNode = InNode;
	if (BindNode && NodeNameText)
	{
		// 根据类型设置显示名称
		FString Name = UEnum::GetValueAsString(BindNode->GetNodeType());
		NodeNameText->SetText(FText::FromString(Name));
	}
}

int32 USkillNodeWidget::GetHashID() const
{
	return BindNode ? BindNode->GetHashID() : -1;
}

FReply USkillNodeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 检测左键点击，触发拖拽检测
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void USkillNodeWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	USkillDragDropOperation* DragOp = NewObject<USkillDragDropOperation>();
	DragOp->NodeHashID = GetHashID();
	
	// 计算鼠标相对于 Widget 左上角的偏移，防止拖拽时 Widget 跳动
	DragOp->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	UClass* VisualClass = DragVisualClass ? DragVisualClass.Get() : GetClass();
	UUserWidget* DragVisual = VisualClass ? CreateWidget<UUserWidget>(GetOwningPlayer(), VisualClass) : nullptr;
	if (USkillNodeWidget* DragNodeWidget = Cast<USkillNodeWidget>(DragVisual))
	{
		DragNodeWidget->InitNodeData(BindNode);
	}
	DragOp->DefaultDragVisual = DragVisual;
	DragOp->Pivot = EDragPivot::MouseDown;

	OutOperation = DragOp;
}

void USkillNodeWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	USkillDragDropOperation* SkillOp = Cast<USkillDragDropOperation>(InOperation);
	if (!SkillOp)
	{
		return;
	}

	if (USkillCanvasWidget* OwnerCanvas = GetTypedOuter<USkillCanvasWidget>())
	{
		OwnerCanvas->ApplyNodeDrop(SkillOp->NodeHashID, InDragDropEvent.GetScreenSpacePosition(), SkillOp->DragOffset);
	}
}
