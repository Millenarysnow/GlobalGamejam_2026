#include "Character/Enemy/RangedEnemy.h"
#include "Actor/EnemyBullet.h"
#include "Component/CharacterAttributeComponent.h"

ARangedEnemy::ARangedEnemy()
{
}

void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Target = GetTargetPlayer();
	if (!Target) return;

	// 1. 寻路移动：保持在射程内
	NavMoveToTarget(Target, AttackRange * 0.8f);

	// 2. 冷却
	if (AttackCooldown > 0.f)
	{
		AttackCooldown -= DeltaTime;
	}

	// 3. 射击判定 (检查视线 LineOfSight 可以用 AIController->LineOfSightTo，这里简化为距离)
	float DistSq = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSq <= FMath::Square(AttackRange) && AttackCooldown <= 0.f)
	{
		Shoot(Target);
	}
}

void ARangedEnemy::Shoot(AActor* Target)
{
	if (!BulletClass) return;

	FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * 60.f;
	FRotator SpawnRot = (Target->GetActorLocation() - SpawnLoc).Rotation();

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEnemyBullet* Bullet = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, SpawnLoc, SpawnRot, Params);
	if (Bullet)
	{
		Bullet->Damage = AttributeComponent ? AttributeComponent->GetDamage() : 10.f;
		Bullet->SlowDuration = 2.0f; // 击中减速2秒
	}

	float AttackSpeed = AttributeComponent ? AttributeComponent->GetAttackSpeed() : 1.f;
	AttackCooldown = 1.0f / FMath::Max(AttackSpeed, 0.1f);
}