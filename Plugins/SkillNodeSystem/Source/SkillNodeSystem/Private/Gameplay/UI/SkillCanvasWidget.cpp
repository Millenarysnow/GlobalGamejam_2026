#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/UI/SkillNodeWidget.h"
#include "Gameplay/UI/SkillDragDropOperation.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Gameplay/UI/SkillDragDropOperation.h" // 节点移动的Drop
#include "Gameplay/UI/SkillConnectionDragDropOperation.h" // 连线的Drop

void USkillCanvasWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UISubsystem = GetGameInstance()->GetSubsystem<USkillUISubsystem>();
    RefreshCanvas();

    UISubsystem->RegisterSkillCanvasWidget(this);

    UE_LOG(LogTemp, Log, TEXT("SkillCanvasWidget Constructed and Canvas Refreshed."));
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

void USkillCanvasWidget::ClearNode()
{
    for (auto& Elem : ActiveNodeWidgets)
    {
        Elem.Value->RemoveFromParent();
    }

    ActiveNodeWidgets.Empty();
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

    if (!UISubsystem || !MainCanvas) return MaxLayerId;
    USkillsManagerSubsystem* Manager = UISubsystem->GetManagerSubsystem();
    if (!Manager) return MaxLayerId;

    const FGeometry& CanvasGeometry = MainCanvas->GetCachedGeometry();

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
		
            if (USkillNodeWidget* ChildWidget = ActiveNodeWidgets.FindRef(ChildNode->GetHashID()))
            {
                // 1. 确定输出引脚类型 (Branch logic)
                OnBranchNode OutputBranchType = OnBranchNode::No;
                if (ParentNode->GetNodeType() == ESkillNodeType::BranchNode)
                {
                    if (ChildNode == ParentNode->GetBranchTrueNode()) OutputBranchType = OnBranchNode::TrU;
                    else if (ChildNode == ParentNode->GetBranchFalseNode()) OutputBranchType = OnBranchNode::FaL;
                }

                // 2. 获取屏幕绝对坐标
                FVector2D StartScreenPos = ParentWidget->GetOutputPinPosition(OutputBranchType);
                FVector2D EndScreenPos = ChildWidget->GetInputPinPosition();

                // 3. 转换为 Canvas 的局部坐标
                FVector2D StartLocal = CanvasGeometry.AbsoluteToLocal(StartScreenPos);
                FVector2D EndLocal = CanvasGeometry.AbsoluteToLocal(EndScreenPos);

                // 4. 画线
                DrawConnectionLine(CanvasGeometry, StartLocal, EndLocal, OutDrawElements, LayerId);
            }
        });
    }
	
    // 【新增】如果正在拖拽连线，画一条临时线（可选，增强体验）
    // 这通常需要监听 DragDrop 事件或绑定一个变量，这里暂时略过，先保证基础连线显示

    return MaxLayerId;
}

void USkillCanvasWidget::DrawConnectionLine(const FGeometry& AllottedGeometry, const FVector2D& Start, const FVector2D& End, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
    TArray<FVector2D> Points;
    Points.Add(Start);
    Points.Add(End);

    // 【核心修复】
    // 使用 AllottedGeometry.ToPaintGeometry() 代替 FPaintGeometry()。
    // 这会将 Canvas 的位置、缩放（DPI Scale）应用到绘制操作中。
    // 配合传入的 Local 坐标，线条就能准确贴合在 Widget 上了。
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        Points,
        ESlateDrawEffect::None,
        FLinearColor::White,
        true,
        2.0f
    );
}