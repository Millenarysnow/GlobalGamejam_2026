#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragDropOperation.generated.h"

class USkillNode;

UCLASS()
class SKILLNODESYSTEM_API USkillDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
    
public:
	// 被拖拽的节点 ID
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	int32 NodeHashID;

	// 被拖拽时的鼠标偏移（用于保持拖拽时相对位置不变）
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FVector2D DragOffset;
};