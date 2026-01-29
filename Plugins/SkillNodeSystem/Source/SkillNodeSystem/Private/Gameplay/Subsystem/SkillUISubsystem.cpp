// Source/SkillNodeSystem/Private/Gameplay/Subsystem/SkillUISubsystem.cpp

#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Gameplay/Subsystem/SkillsManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

USkillUISubsystem* USkillUISubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	UGameInstance* GI = WorldContextObject->GetWorld()->GetGameInstance();
	return GI ? GI->GetSubsystem<USkillUISubsystem>() : nullptr;
}

void USkillUISubsystem::SetNodePosition(int32 HashID, FVector2D Position)
{
	NodePositions.Add(HashID, Position);
}

FVector2D USkillUISubsystem::GetNodePosition(int32 HashID)
{
	// 如果没有存过位置，返回默认值 (0,0) 或给一个随机偏移避免重叠
	if (NodePositions.Contains(HashID))
	{
		return NodePositions[HashID];
	}
	return FVector2D(100.0f, 100.0f); 
}

void USkillUISubsystem::RemoveNodeUIData(int32 HashID)
{
	NodePositions.Remove(HashID);
}

void USkillUISubsystem::CleanStaleData()
{
	USkillsManagerSubsystem* Manager = USkillsManagerSubsystem::Get(this);
	if (!Manager) return;

	TArray<int32> StaleKeys;
	for (const auto& Pair : NodePositions)
	{
		if (!Manager->GetSkillNodeByHash(Pair.Key))
		{
			StaleKeys.Add(Pair.Key);
		}
	}

	for (int32 Key : StaleKeys)
	{
		NodePositions.Remove(Key);
	}
}