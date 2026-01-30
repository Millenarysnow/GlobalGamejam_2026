#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Engine/GameInstance.h"

void USkillUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ManagerSubsystem = Collection.InitializeDependency<USkillsManagerSubsystem>();
}

USkillsManagerSubsystem* USkillUISubsystem::GetManagerSubsystem() const
{
	return ManagerSubsystem;
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