#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Gameplay/Skills/SkillDetailType.h" // for OnBranchNode
#include "Gameplay/Skills/SkillType.h"
#include "SkillConnectionDragDropOperation.generated.h"

UCLASS()
class SKILLNODESYSTEM_API USkillConnectionDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// 发起连线的源节点 HashID
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	int32 SourceNodeHashID;

	// 如果源节点是分支节点，这个引脚代表的分支类型（TrU/FaL/No）
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	OnBranchNode SourceBranchType = OnBranchNode::No;
};