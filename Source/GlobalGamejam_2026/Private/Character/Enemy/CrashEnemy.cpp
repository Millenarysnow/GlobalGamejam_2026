#include "Character/Enemy/CrashEnemy.h"
#include "Interface/HitByEnemy.h"
#include "Component/CharacterAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

ACrashEnemy::ACrashEnemy()
{
	// 可以在这里或蓝图中调高默认速度
}

void ACrashEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Target = GetTargetPlayer();
	if (!Target) return;

	// 1. 寻路移动：直到贴脸 (半径5.0)
	NavMoveToTarget(Target, 5.0f);

	// 2. 距离检测
	float DistSq = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSq <= FMath::Square(TriggerRadius))
	{
		SelfDestruct();
	}
}

void ACrashEnemy::SelfDestruct()
{
	// 播放特效逻辑可在此处添加 (SpawnEmitterAtLocation)

	// 范围伤害检测
	TArray<FHitResult> HitResults;
	FVector Start = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	
	// 扫描 Pawn 类型对象
	GetWorld()->SweepMultiByChannel(HitResults, Start, Start, FQuat::Identity, ECC_Pawn, Sphere);

	float Damage = AttributeComponent ? AttributeComponent->GetDamage() : 20.f;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor != this && HitActor->Implements<UHitByEnemy>())
		{
			// 造成伤害，无减速
			IHitByEnemy::Execute_OnIHit(HitActor, Damage, 0.f, this);
		}
	}
	
	Destroy();
}