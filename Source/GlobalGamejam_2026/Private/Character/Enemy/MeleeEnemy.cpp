#include "Character/Enemy/MeleeEnemy.h"
#include "Interface/HitByEnemy.h"
#include "Component/CharacterAttributeComponent.h"

AMeleeEnemy::AMeleeEnemy()
{
}

void AMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Target = GetTargetPlayer();
	if (!Target) return;

	// 1. 寻路移动：由于有障碍物，我们依然让它寻路，直到距离 AttackRange - 10 停止
	// 减去一点距离是为了确保能碰到玩家
	NavMoveToTarget(Target, AttackRange - 20.f);

	// 2. 冷却处理
	if (AttackCooldown > 0.f)
	{
		AttackCooldown -= DeltaTime;
	}

	// 3. 攻击判定
	float DistSq = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSq <= FMath::Square(AttackRange) && AttackCooldown <= 0.f)
	{
		TryAttack(Target);
	}
}

void AMeleeEnemy::TryAttack(AActor* Target)
{
	if (Target->Implements<UHitByEnemy>())
	{
		float Damage = AttributeComponent ? AttributeComponent->GetDamage() : 10.f;
		// 造成伤害
		IHitByEnemy::Execute_OnIHit(Target, Damage, 0.f, this);
	}

	// 重置冷却：1 / 攻速
	float AttackSpeed = AttributeComponent ? AttributeComponent->GetAttackSpeed() : 1.f;
	AttackCooldown = 1.0f / FMath::Max(AttackSpeed, 0.1f);
}