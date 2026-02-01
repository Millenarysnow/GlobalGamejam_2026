#pragma once

#include "CoreMinimal.h"
#include "InteractActor.h"
#include "Gameplay/Skills/SkillType.h" // 引入 FSkillNodeInfo
#include "RandomEventInteractActor.generated.h"

class USkillNode;
class ACharacter;

UCLASS()
class GLOBALGAMEJAM_2026_API ARandomEventInteractActor : public AInteractActor
{
	GENERATED_BODY()

public:
	virtual void OnInteract_Implementation(AHero* Interactor) override;

protected:
	// --- 配置 ---
	
	/** 随机生成的怪物列表 */
	UPROPERTY(EditAnywhere, Category = "RandomEvent")
	TArray<TSubclassOf<ACharacter>> MonsterClasses;

	/** 创建节点时使用的 SkillNode 基类 (通常填 USkillNode 或自定义 BP) */
	UPROPERTY(EditAnywhere, Category = "RandomEvent")
	TSubclassOf<USkillNode> SkillNodeClass;

	/** 触发生成怪物的概率 (0.0 - 1.0)，默认 0.5 */
	UPROPERTY(EditAnywhere, Category = "RandomEvent")
	float SpawnMonsterProbability = 0.4f;

private:
	FSkillNodeInfo GenerateRandomNodeInfo();
};