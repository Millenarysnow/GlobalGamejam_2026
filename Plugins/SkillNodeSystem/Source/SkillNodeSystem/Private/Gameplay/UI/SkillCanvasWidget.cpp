#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void USkillCanvasWidget::InitSkillTree()
{
	USkillsManagerSubsystem* Manager = USkillsManagerSubsystem::Get(this);
	USkillUISubsystem* UISys = USkillUISubsystem::Get(this);
	UCanvasPanel* Canvas = GetCanvasPanel();

	if (!Manager || !UISys || !Canvas) return;

	Canvas->ClearChildren();
	NodeWidgetMap.Empty();

	// 获取所有逻辑节点 HashID
	const TMap<int32, USkillNode*>& AllNodes = Manager->GetAllNodes(); 

	// 遍历所有节点生成 Widget
	for (const auto& Pair : Manager->GetAllNodes()) 
	{
		USkillNodeWidget* NewWidget = CreateNodeWidget(Pair.Key);
		if (NewWidget)
		{
			UCanvasPanelSlot* CurrentSlot = Canvas->AddChildToCanvas(NewWidget);
			FVector2D Pos = UISys->GetNodePosition(Pair.Key);
			CurrentSlot->SetPosition(Pos);
			CurrentSlot->SetSize(FVector2D(100, 50)); // 设置默认大小或由 Widget 自身决定
			
			NewWidget->InitNode(Pair.Key);
			NodeWidgetMap.Add(Pair.Key, NewWidget);
		}
	}
}

bool USkillCanvasWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	USkillDragDropOperation* Op = Cast<USkillDragDropOperation>(InOperation);
	if (Op)
	{
		// 1. 计算放置位置（Canvas 局部坐标）
		FVector2D LocalPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		// 减去抓取时的偏移，使放置位置自然
		LocalPos -= Op->DragOffset;

		// 2. 更新数据 Subsystem
		if (USkillUISubsystem* UISys = USkillUISubsystem::Get(this))
		{
			UISys->SetNodePosition(Op->HashID, LocalPos);
		}

		// 3. 更新当前视觉 (如果该 Widget 已经在 Canvas 上)
		if (NodeWidgetMap.Contains(Op->HashID))
		{
			if (UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(NodeWidgetMap[Op->HashID]->Slot))
			{
				CurrentSlot->SetPosition(LocalPos);
			}
		}
		
		return true;
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

int32 USkillCanvasWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// 绘制连线
	for (const auto& Pair : NodeWidgetMap)
	{
		USkillNodeWidget* ParentWidget = Pair.Value;
		if (!ParentWidget) continue;

		USkillNode* ParentNode = ParentWidget->GetSkillNode();
		if (!ParentNode) continue;

		// 遍历该节点的子节点
		ParentNode->ForEachChild([&](USkillNode* ChildNode)
		{
			if (ChildNode && NodeWidgetMap.Contains(ChildNode->GetHashID()))
			{
				USkillNodeWidget* ChildWidget = NodeWidgetMap[ChildNode->GetHashID()];
				
				// 获取几何中心
				FVector2D StartPos = ParentWidget->GetCachedGeometry().GetAbsolutePositionAtCoordinates(FVector2D(0.5f, 1.0f)); // 底部中心
				FVector2D EndPos = ChildWidget->GetCachedGeometry().GetAbsolutePositionAtCoordinates(FVector2D(0.5f, 0.0f)); // 顶部中心

				// 转换为 Canvas 局部坐标进行绘制
				StartPos = AllottedGeometry.AbsoluteToLocal(StartPos);
				EndPos = AllottedGeometry.AbsoluteToLocal(EndPos);

				// 绘制直线
				TArray<FVector2D> Points;
				Points.Add(StartPos);
				Points.Add(EndPos);

				FSlateDrawElement::MakeLines(
					OutDrawElements,
					LayerId + 1, // 绘制在 Widget 上层
					AllottedGeometry.ToPaintGeometry(),
					Points,
					ESlateDrawEffect::None,
					FLinearColor::White,
					true,
					2.0f
				);
			}
		});
	}

	return Result;
}