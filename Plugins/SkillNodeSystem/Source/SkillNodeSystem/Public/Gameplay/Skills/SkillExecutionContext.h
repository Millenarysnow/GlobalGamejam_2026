// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillExecutionContext.generated.h"


class USkillNode;

USTRUCT()
struct FSkillStackFrame
{
	GENERATED_BODY()

	UPROPERTY()
	USkillNode* LoopBodyStart = nullptr; // 循环节点的子节点

	UPROPERTY()
	int32 MaxCount = 0;

	UPROPERTY()
	int32 CurrentCount = 0;
};

/**
 * 不使用GC管理，依赖C++智能指针负责生命周期，所以不继承自UObject
 * TSharedFromThis<USkillExecutionContext> 允许在类内部获取自身的智能指针
 */
class SKILLNODESYSTEM_API FSkillExecutionContext : public TSharedFromThis<FSkillExecutionContext>
{
public:
	FSkillExecutionContext() = default;

	FSkillExecutionContext(const FSkillExecutionContext& Other) :
		Stack(Other.Stack),
		Target(Other.Target)
	{ }
	
	void PushStack(FSkillStackFrame Frame);
	FSkillStackFrame PopStack();
	FSkillStackFrame& TopStackRef();
	bool IsStackEmpty() const;

	AActor* GetTarget() const;
	void SetTarget(AActor* _Target);
	
private:
	TArray<FSkillStackFrame> Stack;
	TWeakObjectPtr<AActor> Target = nullptr;
};
