#pragma once

#include "CoreMinimal.h"

#include "Gameplay/Skills/SkillType.h"

#include "SkillChildType.generated.h"


/**
 *
 * **不要使用默认构造函数**
 *
 */

USTRUCT(BlueprintType)
struct FBranchNodeInfo : public FSkillNodeInfo
{
	GENERATED_BODY()

	FBranchNodeInfo() = default;

	FBranchNodeInfo(EBranchType _BranchType, float _DelayTime = 100.0f, float _BranchValue = 0.4f);
};

USTRUCT(BlueprintType)
struct FBuffNodeInfo : public FSkillNodeInfo
{
	GENERATED_BODY()

	FBuffNodeInfo() = default;

	FBuffNodeInfo(EBuffType _BuffType, float _BuffDurationTime, float _BuffValue, float _DelayTime = 100.0f, int32 _OutPinCount = 1);
};

USTRUCT(BlueprintType)
struct FGenerateNodeInfo : public FSkillNodeInfo

{
	GENERATED_BODY()

	FGenerateNodeInfo() = default;

	FGenerateNodeInfo(EGeneratedType _GeneratedType, float _Damage, float _Radius, float _DelayTime = 100.0f, int32 _OutPinCount = 1);
};

USTRUCT(BlueprintType)
struct FLoopNodeInfo : public FSkillNodeInfo
{
	GENERATED_BODY()

	FLoopNodeInfo() = default;
	
	FLoopNodeInfo(float _DelayTime, int32 _OutPinCount = 1, int32 _LoopCount = 3);
};

USTRUCT(BlueprintType)
struct FParamNodeInfo : public FSkillNodeInfo
{
	GENERATED_BODY()

	FParamNodeInfo() = default;

	FParamNodeInfo(EParamType _ParamType, int32 _ParamIntValue = 0, float _ParamFloatValue = 1.0f, int32 _OutPinCount = 1);
};

USTRUCT(BlueprintType)
struct FStartNodeInfo : public FSkillNodeInfo
{
	GENERATED_BODY()

	FStartNodeInfo() = default;

	FStartNodeInfo(EStartNodeType _StartNodeType, float _DelayTime = 100.0f, int32 _OutPinCount = 1);
};
