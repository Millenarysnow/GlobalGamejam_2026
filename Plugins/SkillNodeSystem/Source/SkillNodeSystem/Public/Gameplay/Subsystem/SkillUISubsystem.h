// Source/SkillNodeSystem/Public/Gameplay/Subsystem/SkillUISubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillUISubsystem.generated.h"

/**
 * 管理技能树 UI 数据
 */
UCLASS()
class SKILLNODESYSTEM_API USkillUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static USkillUISubsystem* Get(const UObject* WorldContextObject);

	// 设置节点坐标
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void SetNodePosition(int32 HashID, FVector2D Position);

	// 获取节点坐标
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	FVector2D GetNodePosition(int32 HashID);

	// 移除节点 UI 数据（当逻辑层删除节点时调用，或者定期清理）
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void RemoveNodeUIData(int32 HashID);

	// 清理所有无效的 UI 数据（对比 Manager 中的实际节点）
	UFUNCTION(BlueprintCallable, Category = "SkillUI")
	void CleanStaleData();

private:
	// Key: HashID, Value: Canvas 上的坐标
	UPROPERTY()
	TMap<int32, FVector2D> NodePositions;
};