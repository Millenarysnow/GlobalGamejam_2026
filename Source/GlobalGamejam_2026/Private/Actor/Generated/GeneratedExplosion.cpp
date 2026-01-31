#include "Actor/Generated/GeneratedExplosion.h"

#include "Component/SkillComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/OnHit.h"


AGeneratedExplosion::AGeneratedExplosion()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	ExplosionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = ExplosionSphere;
}

void AGeneratedExplosion::BeginPlay()
{
	Super::BeginPlay();

	// 设置爆炸半径 (优先使用技能节点传入的 Radius)
	float ActualRadius = (SkillRadius > 0.0f) ? SkillRadius : 300.0f;
	ExplosionSphere->SetSphereRadius(ActualRadius);

	// 获取范围内所有 Actor
	TArray<AActor*> OverlappingActors;
	ExplosionSphere->GetOverlappingActors(OverlappingActors);

	bool bHasTriggeredChain = false;

	for (AActor* Target : OverlappingActors)
	{
		if (Target && Target != GetOwner() && IsEnemy(Target))
		{
			// 造成伤害
			// 尝试通过接口调用
			if (Target->Implements<UOnHit>())
			{
				IOnHit::Execute_OnGetHit(Target, SkillDamage, GetOwner());
			}

			// 触发技能链
			// 注意：爆炸通常是AOE，可能会触发多次链式反应，或者只对主要目标触发。
			// 这里我们对每一个命中的敌人都触发一次后续技能（如：爆炸生成的每一团火焰都附着在对应敌人身上）
			if (SourceSkillComponent)
			{
				SourceSkillComponent->ReportSkillHit(Target);
			}
		}
	}

	// 爆炸特效播放完即可销毁 (这里暂定0.5秒)
	SetLifeSpan(0.5f);
}
