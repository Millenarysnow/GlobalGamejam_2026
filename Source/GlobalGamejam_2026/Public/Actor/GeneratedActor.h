#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratedActor.generated.h"

class USkillComponent;

UCLASS()
class GLOBALGAMEJAM_2026_API AGeneratedActor : public AActor
{
	GENERATED_BODY()

public:
	AGeneratedActor();

	// 初始化生成物数据
	// 生成时一定要调用，否则无法获取到来源技能组件和伤害等信息
	virtual void InitGeneratedActor(USkillComponent* InSkillComponent, float InDamage, float InRadius);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	USkillComponent* SourceSkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillRadius;

	// 敌我判断逻辑
	bool IsEnemy(AActor* TargetActor) const;
};
