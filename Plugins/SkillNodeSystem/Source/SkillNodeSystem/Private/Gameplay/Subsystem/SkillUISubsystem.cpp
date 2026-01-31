#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Engine/GameInstance.h"

#include "Gameplay/Skills/SkillType.h"
#include "Gameplay/UI/SkillCanvasWidget.h"

void USkillUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ManagerSubsystem = Collection.InitializeDependency<USkillsManagerSubsystem>();
}

USkillsManagerSubsystem* USkillUISubsystem::GetManagerSubsystem() const
{
	return ManagerSubsystem;
}

FString USkillUISubsystem::GetDescribeTextByType(const ESkillNodeType NodeType)
{
	switch (NodeType)
	{
		case ESkillNodeType::NullNode:
			return FString("Null Node: No functionality.");
		case ESkillNodeType::StartNode:
			return FString("Start Node: Entry point of the skill.");
		case ESkillNodeType::GenerateNode:
			return FString("Generate Node: Creates entities or effects.");
		case ESkillNodeType::BuffNode:
			return FString("Buff Node: Applies buffs or debuffs.");
		case ESkillNodeType::BranchNode:
			return FString("Branch Node: Creates conditional branches.");
		case ESkillNodeType::LoopStartNode:
			return FString("Loop Start Node: Begins a loop structure.");
		case ESkillNodeType::LoopEndNode:
			return FString("Loop End Node: Ends a loop structure.");
		case ESkillNodeType::ParamNode:
			return FString("Param Node: Modifies parameters.");
		default:
			return FString("Unknown Node Type.");
	}
}

void USkillUISubsystem::ClearNode()
{
	NodePositions.Empty();

	if (SkillCanvasWidget)
	{
		SkillCanvasWidget->ClearNode();
	}
}

FVector2D USkillUISubsystem::GetNodePosition(int32 HashID) const
{
	if (NodePositions.Contains(HashID))
	{
		return NodePositions[HashID];
	}
	return FVector2D(0.f, 0.f); // 默认位置
}

void USkillUISubsystem::SetNodePosition(int32 HashID, FVector2D NewPosition)
{
	NodePositions.Add(HashID, NewPosition);
}

void USkillUISubsystem::RegisterSkillCanvasWidget(USkillCanvasWidget* InWidget)
{
	SkillCanvasWidget = InWidget;
}
