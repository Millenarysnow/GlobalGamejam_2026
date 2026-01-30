#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/UI/SkillNodeWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USkillCanvasWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UISubsystem = GetGameInstance()->GetSubsystem<USkillUISubsystem>();
    RefreshCanvas();
}

void USkillCanvasWidget::RefreshCanvas()
{
    if (!MainCanvas || !UISubsystem || !NodeWidgetClass) return;

    MainCanvas->ClearChildren();
    ActiveNodeWidgets.Empty();

    USkillsManagerSubsystem* Manager = UISubsystem->GetManagerSubsystem();
    if (!Manager) return;

    TMap<int32, USkillNode*>& AllNodes = Manager->GetAllNodes();

    for (auto& Elem : AllNodes)
    {
        USkillNode* Node = Elem.Value;
        if (!Node) continue;

        // 创建 Widget
        USkillNodeWidget* NewNodeWidget = CreateWidget<USkillNodeWidget>(this, NodeWidgetClass);
        NewNodeWidget->InitNodeData(Node);
        
        // 添加到画布
        UCanvasPanelSlot* CanvasSlot = MainCanvas->AddChildToCanvas(NewNodeWidget);
        
        // 设置位置
        FVector2D Pos = UISubsystem->GetNodePosition(Node->GetHashID());
        CanvasSlot->SetPosition(Pos);
        CanvasSlot->SetSize(FVector2D(150.f, 80.f)); // 默认大小，建议在 UMG 蓝图中设置

        ActiveNodeWidgets.Add(Node->GetHashID(), NewNodeWidget);
    }
}

void USkillCanvasWidget::ApplyNodeDrop(int32 NodeHashID, const FVector2D& ScreenPos, const FVector2D& DragOffset)
{
    if (!UISubsystem || !MainCanvas)
    {
        return;
    }

    // 1. 获取鼠标在 Canvas 内的局部坐标
    const FGeometry& CanvasGeometry = MainCanvas->GetCachedGeometry();
    FVector2D LocalMousePos = CanvasGeometry.AbsoluteToLocal(ScreenPos);

    // 2. 减去拖拽时的偏移，保证松手时节点不跳变
    FVector2D NewPos = LocalMousePos - DragOffset;

    // 3. 更新 Controller 数据
    UISubsystem->SetNodePosition(NodeHashID, NewPos);

    // 4. 直接更新 UI 位置 (不用完全 RefreshCanvas，性能更好)
    if (USkillNodeWidget** FoundWidget = ActiveNodeWidgets.Find(NodeHashID))
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>((*FoundWidget)->Slot))
        {
            CanvasSlot->SetPosition(NewPos);
        }
    }
}

bool USkillCanvasWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    USkillDragDropOperation* SkillOp = Cast<USkillDragDropOperation>(InOperation);
    if (SkillOp)
    {
        ApplyNodeDrop(SkillOp->NodeHashID, InDragDropEvent.GetScreenSpacePosition(), SkillOp->DragOffset);
        return true;
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

int32 USkillCanvasWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 MaxLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (!UISubsystem) return MaxLayerId;
    USkillsManagerSubsystem* Manager = UISubsystem->GetManagerSubsystem();
    if (!Manager) return MaxLayerId;

    // 遍历所有节点画线
    for (auto& Elem : ActiveNodeWidgets)
    {
        int32 ParentID = Elem.Key;
        USkillNodeWidget* ParentWidget = Elem.Value;
        USkillNode* ParentNode = Manager->GetSkillNodeByHash(ParentID);

        if (!ParentNode || !ParentWidget) continue;

        // 遍历所有子节点
        ParentNode->ForEachChild([&](USkillNode* ChildNode)
        {
            if (!ChildNode) return;
            
            if (USkillNodeWidget* ChildWidgetPtr = ActiveNodeWidgets.FindRef(ChildNode->GetHashID()))
            {
                // 获取 Widget 在 Canvas 中的几何中心
                // 注意：这里需要考虑 Widget 的实际位置
                FVector2D ParentPos = UISubsystem->GetNodePosition(ParentID);
                FVector2D ChildPos = UISubsystem->GetNodePosition(ChildNode->GetHashID());

                // 假设 Size 是 150x80，连接点设为 Parent 底部中心 -> Child 顶部中心
                FVector2D StartPoint = ParentPos + FVector2D(75.f, 80.f); 
                FVector2D EndPoint = ChildPos + FVector2D(75.f, 0.f);

                DrawConnectionLine(StartPoint, EndPoint, OutDrawElements, LayerId);
            }
        });
    }

    return MaxLayerId;
}

void USkillCanvasWidget::DrawConnectionLine(const FVector2D& Start, const FVector2D& End, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
    TArray<FVector2D> Points;
    Points.Add(Start);
    
    // 简单的贝塞尔曲线模拟：添加两个控制点
    float TangentLength = FMath::Abs(End.Y - Start.Y) * 0.5f;
    FVector2D ControlPoint1 = Start + FVector2D(0.f, TangentLength);
    FVector2D ControlPoint2 = End - FVector2D(0.f, TangentLength);

    // 这里简化为直接画直线，如果想画曲线可以使用 FPaintContext 或 Spline 算法生成点集
    // 为演示简洁，这里画直线
    Points.Add(End);

    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        FPaintGeometry(),
        Points,
        ESlateDrawEffect::None,
        FLinearColor::White,
        true,
        2.0f
    );
}