#include "Gameplay/UI/SkillNodeWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/UI/SkillPinWidget.h"
#include "Components/PanelWidget.h"

void USkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillNodeWidget::InitNodeData(USkillNode* InNode)
{
	BindNode = InNode;
	if (!BindNode) return;

	// 1. 设置名称
	if (NodeNameText)
	{
		FString Name = UEnum::GetValueAsString(BindNode->GetNodeType());
		// 截取掉 "ESkillNodeType::" 前缀
		int32 Index = Name.Find("::");
		if (Index != INDEX_NONE) Name = Name.RightChop(Index + 2);
		NodeNameText->SetText(FText::FromString(Name));
	}

	if (!PinWidgetClass) return;

	// 2. 生成输入引脚
	if (InputPinContainer)
	{
		InputPinContainer->ClearChildren();
		InputPinWidget = nullptr;

		// 起始节点通常没有父节点逻辑，但根据要求"固定为1"，或者是为了UI统一
		// 这里我们在逻辑上判断：如果不是StartNode，则生成输入引脚
		// 或者根据你的要求 "入引脚固定为1"，即使是StartNode也生成（虽然逻辑上连不上）
		// 为了严谨，StartNode不生成Input，其他都生成
		if (BindNode->GetNodeType() != ESkillNodeType::StartNode)
		{
			USkillPinWidget* Pin = CreateWidget<USkillPinWidget>(this, PinWidgetClass);
			Pin->InitPinData(BindNode, true, OnBranchNode::No);
			InputPinContainer->AddChild(Pin);
			InputPinWidget = Pin;
		}
	}

	// 3. 生成输出引脚
	if (OutputPinContainer)
	{
		OutputPinContainer->ClearChildren();
		OutputPinWidgets.Empty();

		// 分支节点特殊处理
		if (BindNode->GetNodeType() == ESkillNodeType::BranchNode)
		{
			// True 分支
			USkillPinWidget* TruePin = CreateWidget<USkillPinWidget>(this, PinWidgetClass);
			TruePin->InitPinData(BindNode, false, OnBranchNode::TrU);
			OutputPinContainer->AddChild(TruePin);
			OutputPinWidgets.Add(OnBranchNode::TrU, TruePin);

			// False 分支
			USkillPinWidget* FalsePin = CreateWidget<USkillPinWidget>(this, PinWidgetClass);
			FalsePin->InitPinData(BindNode, false, OnBranchNode::FaL);
			OutputPinContainer->AddChild(FalsePin);
			OutputPinWidgets.Add(OnBranchNode::FaL, FalsePin);
		}
		else
		{
			// 普通节点，根据 OutPinCount 生成（通常为 1，或者 ParamNode 可能有多个？）
			// 既然是树形结构，且没有多分支，通常就是一个默认输出
			int32 Count = BindNode->GetNodeInfo().OutPinCount;
			if (Count > 0)
			{
				USkillPinWidget* Pin = CreateWidget<USkillPinWidget>(this, PinWidgetClass);
				Pin->InitPinData(BindNode, false, OnBranchNode::No);
				OutputPinContainer->AddChild(Pin);
				OutputPinWidgets.Add(OnBranchNode::No, Pin);
			}
		}
	}
}

int32 USkillNodeWidget::GetHashID() const
{
	return BindNode ? BindNode->GetHashID() : -1;
}

FVector2D USkillNodeWidget::GetInputPinPosition() const
{
	if (InputPinWidget)
	{
		return InputPinWidget->GetPinAbsolutePosition();
	}
	// 兜底：如果没有输入引脚，返回 Widget 顶部中心
	FGeometry Geom = GetCachedGeometry();
	FVector2D LocalTop = FVector2D(Geom.GetLocalSize().X * 0.5f, 0.f);
	return Geom.LocalToAbsolute(LocalTop);
}

FVector2D USkillNodeWidget::GetOutputPinPosition(OnBranchNode BranchType) const
{
	if (USkillPinWidget* const* FoundPin = OutputPinWidgets.Find(BranchType))
	{
		if (*FoundPin)
		{
			return (*FoundPin)->GetPinAbsolutePosition();
		}
	}
	
	// 尝试找默认
	if (USkillPinWidget* const* FoundDefault = OutputPinWidgets.Find(OnBranchNode::No))
	{
		if (*FoundDefault) return (*FoundDefault)->GetPinAbsolutePosition();
	}

	// 兜底：Widget 底部中心
	FGeometry Geom = GetCachedGeometry();
	FVector2D LocalBottom = FVector2D(Geom.GetLocalSize().X * 0.5f, Geom.GetLocalSize().Y);
	return Geom.LocalToAbsolute(LocalBottom);
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
