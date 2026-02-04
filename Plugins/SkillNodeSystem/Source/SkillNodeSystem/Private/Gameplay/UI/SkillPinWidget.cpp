#include "Gameplay/UI/SkillPinWidget.h"
#include "Gameplay/UI/SkillConnectionDragDropOperation.h"
#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"

void USkillPinWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillPinWidget::InitPinData(USkillNode* InNode, bool bInIsInput, OnBranchNode InBranchType)
{
	OwnerNode = InNode;
	bIsInput = bInIsInput;
	BranchType = InBranchType;

	// 简单的视觉区分（实际应在蓝图中设置）
	if (PinButton)
	{
		// 只是为了拦截鼠标事件，不做点击逻辑
		PinButton->SetIsEnabled(false); 
		// 注意：在蓝图中 Button 的 Visibility 必须是 Visible/HitTestInvisible，
		// 但为了让 Widget 本身响应 OnMouseButtonDown，Button 的 IsFocusable 应设为 false 或者 HitTestInvisible
	}
}

FVector2D USkillPinWidget::GetPinAbsolutePosition() const
{
	// 获取几何中心
	FGeometry Geom = GetCachedGeometry();
	FVector2D LocalCenter = Geom.GetLocalSize() * 0.5f;
	return Geom.LocalToAbsolute(LocalCenter);
}

FReply USkillPinWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 右键断开连接
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (OwnerNode.IsValid())
		{
			USkillUISubsystem* UISub = GetGameInstance()->GetSubsystem<USkillUISubsystem>();
			if (UISub && UISub->GetManagerSubsystem())
			{
				USkillsManagerSubsystem* Manager = UISub->GetManagerSubsystem();
				bool bNeedRefresh = false;

				if (bIsInput)
				{
					// --- 输入引脚：断开与父节点的连接 ---
					if (USkillNode* Parent = OwnerNode->GetParentNode())
					{
						// 从父节点中断开自己
						Manager->DisconnectNode(Parent, OwnerNode.Get(), OnBranchNode::No);
						bNeedRefresh = true;
					}
				}
				else
				{
					// --- 输出引脚：断开子节点 ---
					
					if (BranchType != OnBranchNode::No)
					{
						// 1. 分支引脚 (TrU/FaL)：只断开对应分支的连接
						USkillNode* TargetChild = nullptr;
						if (BranchType == OnBranchNode::TrU)
						{
							TargetChild = OwnerNode->GetBranchTrueNode();
						}
						else 
						{
							TargetChild = OwnerNode->GetBranchFalseNode();
						}

						if (TargetChild)
						{
							Manager->DisconnectNode(OwnerNode.Get(), TargetChild, BranchType);
							bNeedRefresh = true;
						}
					}
					else
					{
						// 普通/主输出引脚：断开所有从此引脚连出的子节点
						// 先收集需要断开的子节点（避免在遍历过程中修改容器导致迭代器失效）
						TArray<USkillNode*> ChildrenToDisconnect;
						OwnerNode->ForEachChild([&ChildrenToDisconnect](USkillNode* Child)
						{
							if (Child) ChildrenToDisconnect.Add(Child);
						});

						for (USkillNode* Child : ChildrenToDisconnect)
						{
							Manager->DisconnectNode(OwnerNode.Get(), Child, OnBranchNode::No);
							bNeedRefresh = true;
						}
					}
				}

				// 如果发生了断开操作，刷新画布以更新连线视图
				if (bNeedRefresh)
				{
					if (USkillCanvasWidget* Canvas = GetTypedOuter<USkillCanvasWidget>())
					{
						Canvas->RefreshCanvas();
					}
				}
				
				return FReply::Handled();
			}
		}
	}
	
	// 只有输出引脚可以发起连线拖拽
	if (!bIsInput && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void USkillPinWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!OwnerNode.IsValid()) return;

	USkillConnectionDragDropOperation* ConnOp = NewObject<USkillConnectionDragDropOperation>();
	ConnOp->SourceNodeHashID = OwnerNode->GetHashID();
	ConnOp->SourceBranchType = BranchType;
	
	// 设置一个简单的视觉反馈
	ConnOp->DefaultDragVisual = this; // 或者创建一个临时的线段 Widget
	ConnOp->Pivot = EDragPivot::CenterCenter;

	OutOperation = ConnOp;
}

bool USkillPinWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 只有输入引脚可以接收连线
	if (!bIsInput) return false;
	if (!OwnerNode.IsValid()) return false;

	USkillConnectionDragDropOperation* ConnOp = Cast<USkillConnectionDragDropOperation>(InOperation);
	if (ConnOp)
	{
		USkillUISubsystem* UISub = GetGameInstance()->GetSubsystem<USkillUISubsystem>();
		if (UISub && UISub->GetManagerSubsystem())
		{
			USkillsManagerSubsystem* Manager = UISub->GetManagerSubsystem();
			USkillNode* SourceNode = Manager->GetSkillNodeByHash(ConnOp->SourceNodeHashID);
			USkillNode* TargetNode = OwnerNode.Get();

			if (SourceNode && TargetNode && SourceNode != TargetNode)
			{
				// 调用逻辑层接口进行连接
				if (Manager->ConnectNode(SourceNode, TargetNode, ConnOp->SourceBranchType))
				{
					// 连接成功，刷新画布
					// 也可以只局部刷新，但 RefreshCanvas 最稳妥
					// 向上寻找 Canvas 刷新
					if (USkillCanvasWidget* Canvas = GetTypedOuter<USkillCanvasWidget>())
					{
						Canvas->RefreshCanvas();
					}
				}
			}
		}
		return true;
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}