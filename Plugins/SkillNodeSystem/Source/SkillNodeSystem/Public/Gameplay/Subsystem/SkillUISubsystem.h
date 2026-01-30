#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillUISubsystem.generated.h"

class USkillsManagerSubsystem;

/**
 * 管理技能树 UI 数据 (Controller)
 */
UCLASS()
class SKILLNODESYSTEM_API USkillUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	USkillsManagerSubsystem* GetManagerSubsystem() const;

	// --- UI 数据管理 (坐标) ---
    
	// 获取节点在画布上的位置
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	FVector2D GetNodePosition(int32 HashID) const;

	// 更新节点位置
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void SetNodePosition(int32 HashID, FVector2D NewPosition);

private:
	// 存储节点 HashID -> 画布坐标
	UPROPERTY()
	TMap<int32, FVector2D> NodePositions;
    
	UPROPERTY()
	USkillsManagerSubsystem* ManagerSubsystem;
};