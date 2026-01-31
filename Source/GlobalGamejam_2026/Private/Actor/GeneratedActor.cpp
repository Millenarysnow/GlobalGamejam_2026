#include "Actor/GeneratedActor.h"


AGeneratedActor::AGeneratedActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SkillDamage = 0.f;
	SkillRadius = 0.f;
	SourceSkillComponent = nullptr;
}

void AGeneratedActor::InitGeneratedActor(USkillComponent* InSkillComponent, float InDamage, float InRadius)
{
	SourceSkillComponent = InSkillComponent;
	SkillDamage = InDamage;
	SkillRadius = InRadius;
}

bool AGeneratedActor::IsEnemy(AActor* TargetActor) const
{
	if (!TargetActor || !GetOwner()) return false;

	// TODO: 确认敌我关系的逻辑（是否有友伤）

	// Tag 区分
	// 如果 Owner 是 Player，则 Target 有 Enemy 标签时为敌人
	if (GetOwner()->ActorHasTag(FName("Player")) && TargetActor->ActorHasTag(FName("Enemy")))
	{
		return true;
	}
	// 如果 Owner 是 Enemy，则 Target 有 Player 标签时为敌人
	if (GetOwner()->ActorHasTag(FName("Enemy")) && TargetActor->ActorHasTag(FName("Player")))
	{
		return true;
	}

	return false;
}

