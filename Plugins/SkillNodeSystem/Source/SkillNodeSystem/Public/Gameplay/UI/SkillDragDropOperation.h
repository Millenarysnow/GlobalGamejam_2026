// Source/SkillNodeSystem/Public/UI/SkillDragDropOperation.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragDropOperation.generated.h"

UCLASS()
class SKILLNODESYSTEM_API USkillDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// 被拖拽的节点 HashID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	int32 HashID;

	// 鼠标点击位置相对于 Widget 左上角的偏移，用于实现精确拖拽
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FVector2D DragOffset;
};