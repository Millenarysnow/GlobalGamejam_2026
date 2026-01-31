#include "Actor/Generated/GeneratedBullet.h"

#include "Component/SkillComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/OnHit.h"


AGeneratedBullet::AGeneratedBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建碰撞组件
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(20.0f);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 或者自定义的 Projectile Profile
	RootComponent = SphereComp;

	// 创建移动组件
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->UpdatedComponent = SphereComp;
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->MaxSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.0f; // 直线飞行
}

void AGeneratedBullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGeneratedBullet::OnOverlap);

	// 设置寿命，防止无限飞行
	SetLifeSpan(5.0f);
}

void AGeneratedBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 忽略自己和所有者
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	// 1. 敌我判断 (命中敌人扣血，命中友军无视)
	if (IsEnemy(OtherActor))
	{
		// 尝试通过接口调用
		if (OtherActor->Implements<UOnHit>())
		{
			IOnHit::Execute_OnGetHit(OtherActor, SkillDamage, GetOwner());
		}

		// 2. 命中后必须调用父类回调 (TriggerSkillChain)
		if (SourceSkillComponent)
		{
			SourceSkillComponent->ReportSkillHit(OtherActor);
		}

		// 命中敌人销毁子弹
		Destroy();
	}
	else
	{
		// 命中墙壁等非角色物体也销毁，但不触发技能链
		// 这里简单判定：如果不是友军（且不是敌人），则视为障碍物
		if (!OtherActor->ActorHasTag(FName("Player")) && !OtherActor->ActorHasTag(FName("Enemy"))) 
		{
			Destroy();
		}
	}
}

