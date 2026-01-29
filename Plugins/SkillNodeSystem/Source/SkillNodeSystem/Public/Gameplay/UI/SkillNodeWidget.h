// Source/SkillNodeSystem/Public/UI/SkillNodeWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Skills/SkillNode.h"
#include "SkillNodeWidget.generated.h"

UCLASS()
class SKILLNODESYSTEM_API USkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 初始化节点数据
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void InitNode(int32 InHashID);

	// 获取关联的逻辑节点
	UFUNCTION(BlueprintPure, Category = "SkillUI")
	USkillNode* GetSkillNode() const;

	UFUNCTION(BlueprintPure, Category = "SkillUI")
	int32 GetHashID() const { return HashID; }

protected:
	// 重写鼠标按下事件，检测拖拽
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	// 重写拖拽发现事件，创建 DragDropOperation
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "SkillUI")
	int32 HashID = -1;
};