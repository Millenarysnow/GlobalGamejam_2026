#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h" 
#include "Enemy.generated.h"

class UCharacterAttributeComponent;
class AAIController;

UCLASS()
class GLOBALGAMEJAM_2026_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// 缓存 AIController
	UPROPERTY()
	AAIController* EnemyAIController;

	// 获取目标（玩家）
	AActor* GetTargetPlayer() const;

	// 使用寻路移动到目标
	// AcceptanceRadius: 停止距离（到达这个半径内即视为到达）
	void NavMoveToTarget(AActor* Target, float AcceptanceRadius);
	
	// 简单的朝向旋转（辅助攻击时的朝向）
	void FaceTarget(AActor* Target);
};