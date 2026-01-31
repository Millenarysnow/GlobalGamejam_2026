#include "Actor/EnemyBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/HitByEnemy.h"

AEnemyBullet::AEnemyBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic"); // 或者自定义 Profile
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlapBegin);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 900.f;
	ProjectileMovement->MaxSpeed = 900.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 直线飞行
	
	InitialLifeSpan = 5.0f;
}

void AEnemyBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								  bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator()) return;

	if (OtherActor->Implements<UHitByEnemy>())
	{
		// 击中玩家：造成伤害 + 减速
		IHitByEnemy::Execute_OnIHit(OtherActor, Damage, SlowDuration, GetInstigator());
		Destroy();
	}
	else if (OtherActor->GetClass()->IsChildOf(APawn::StaticClass()) == false)
	{
		// 击中墙壁/障碍物销毁
		Destroy();
	}
}