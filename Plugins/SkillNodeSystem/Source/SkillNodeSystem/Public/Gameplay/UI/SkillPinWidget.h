#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Skills/SkillDetailType.h"
#include "Gameplay/Skills/SkillType.h"
#include "SkillPinWidget.generated.h"

class USkillNode;
class UButton;

/**
 * 节点的引脚 (Input/Output)
 */
UCLASS(Abstract)
class SKILLNODESYSTEM_API USkillPinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/**
	 * 初始化引脚
	 * @param InNode 所属节点
	 * @param bInIsInput true=输入引脚, false=输出引脚
	 * @param InBranchType 如果是输出引脚，代表的分支类型
	 */
	UFUNCTION(BlueprintCallable)
	void InitPinData(USkillNode* InNode, bool bInIsInput, OnBranchNode InBranchType = OnBranchNode::No);

	// 获取引脚在视口/Canvas中的中心坐标
	FVector2D GetPinAbsolutePosition() const;

	bool IsInputPin() const { return bIsInput; }
	OnBranchNode GetBranchType() const { return BranchType; }

protected:
	// --- 拖拽处理 (Output Pin 发起) ---
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// --- 放置处理 (Input Pin 接收) ---
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 弱引用所属节点，防止循环引用
	TWeakObjectPtr<USkillNode> OwnerNode;

	bool bIsInput = false;
	OnBranchNode BranchType = OnBranchNode::No;

	// 用于显示的按钮或Image（在蓝图中绑定）
	UPROPERTY(meta = (BindWidget))
	UButton* PinButton;
};