#pragma once

#include "CoreMinimal.h"

#include "Gameplay/Skills/SkillDetailType.h"

#include "SkillType.generated.h"

UENUM(BlueprintType)
enum class ESkillNodeType:uint8
{
	NullNode = 0, // 空类型
	StartNode, // 起始类型
	GenerateNode, // 生成物类型
	BuffNode, // buff类型
	BranchNode, // 分支类型
	ParamNode, // 参数类型
	LoopStartNode, // 循环起始类型
	LoopEndNode // **已弃用** 循环结束类型
};

UENUM(BlueprintType)
enum class ESwitchTargetType:uint8
{
	Unchanged = 0,
	Changed
};

USTRUCT(BlueprintType)
struct FSkillNodeInfo
{
	GENERATED_BODY()

	// ----- 公有信息 -----
	UPROPERTY(BlueprintReadOnly)
	ESkillNodeType NodeType = ESkillNodeType::NullNode; // 节点类型
	UPROPERTY(BlueprintReadOnly)
	ESwitchTargetType SwitchTargetType = ESwitchTargetType::Unchanged; // 对象是否变化（是否需要回传）
	UPROPERTY(BlueprintReadOnly)
	float DelayTime = 0; // 延迟时间
	UPROPERTY(BlueprintReadOnly)
	int32 OutPinCount = 0; // 输出引脚数量
	UPROPERTY(BlueprintReadOnly)
	EStartNodeType StartNodeType = EStartNodeType::None;
	UPROPERTY(BlueprintReadOnly)
	EBranchType BranchType = EBranchType::None;
	UPROPERTY(BlueprintReadOnly)
	EGeneratedType GeneratedType = EGeneratedType::None;
	UPROPERTY(BlueprintReadOnly)
	EParamType ParamType = EParamType::None;
	UPROPERTY(BlueprintReadOnly)
	EBuffType BuffType = EBuffType::None;
	// ----- 公有信息 -----

	// ----- 生成物信息 -----
	UPROPERTY(BlueprintReadOnly)
	float Damage = 1.0f; // 伤害
	UPROPERTY(BlueprintReadOnly)
	float Radius = 1.0f; // 半径
	// ----- 生成物信息 -----

	// ----- 增益信息 -----
	UPROPERTY(BlueprintReadOnly)
	float BuffDurationTime = 1.0f; // buff持续时间
	UPROPERTY(BlueprintReadOnly)
	float BuffValue = 1.0f; // buff值
	// ----- 增益信息 -----

	// ----- 分支信息 -----
	UPROPERTY(BlueprintReadOnly)
	float BranchValue = 0.4f; // 分支值
	// ----- 分支信息 -----
	
	// ----- 参数信息 -----
	UPROPERTY(BlueprintReadOnly)
	int32 ParamIntValue = 0; // 整型参数
	UPROPERTY(BlueprintReadOnly)
	float ParamFloatValue = 1.0f; // 浮点型参数
	// ----- 参数信息 -----	

	// ----- 循环信息 -----
	UPROPERTY(BlueprintReadOnly)
	int32 LoopCount = 3; // 循环次数
	// ----- 循环信息 -----
};

USTRUCT(BlueprintType)
struct FAccumulativeInfo
{
	GENERATED_BODY()
	
	float AccDamageValue = 0; // 伤害
	float AccEffectiveTimeValue = 0.0f; // 生效时间
	float AccReduceDelayTimeValue = 0; // 延迟时间
	float RadiusValue = 0.0f; // 半径

	static const FAccumulativeInfo ZeroValue;

	FAccumulativeInfo operator+(const FAccumulativeInfo& Other) const
	{
		FAccumulativeInfo Result;
		Result.AccDamageValue = this->AccDamageValue + Other.AccDamageValue;
		Result.AccEffectiveTimeValue = this->AccEffectiveTimeValue + Other.AccEffectiveTimeValue;
		Result.AccReduceDelayTimeValue = this->AccReduceDelayTimeValue + Other.AccReduceDelayTimeValue;
		Result.RadiusValue = this->RadiusValue + Other.RadiusValue;
		return Result;
	}

	FAccumulativeInfo& operator+=(const FAccumulativeInfo& Other)
	{
		this->AccDamageValue += Other.AccDamageValue;
		this->AccEffectiveTimeValue += Other.AccEffectiveTimeValue;
		this->AccReduceDelayTimeValue += Other.AccReduceDelayTimeValue;
		this->RadiusValue += Other.RadiusValue;
		return *this;
	}
};

UENUM(BlueprintType)
enum class OnBranchNode:uint8 // 判断当前节点是否链接在分支节点上
{
	No = 0,
	TrU,
	FaL
};

USTRUCT(BlueprintType)
struct FNodeBuffValueFinal // 结算后用于传给组件的buff信息
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	EBuffType BuffType = EBuffType::None;
	UPROPERTY(BlueprintReadWrite)
	float BuffDurationTime = 1.0f; // buff持续时间
	UPROPERTY(BlueprintReadWrite)
	float Strength = 1.0f; // 伤害提升
	UPROPERTY(BlueprintReadWrite)
	float Radius = 1.0f; // 半径
	UPROPERTY(BlueprintReadWrite)
	float Accelerate = 1.0f; // 加速
};

USTRUCT(BlueprintType)
struct FNodeGenerateValueFinal // 结算后用于传给组件的生成物信息
{
	GENERATED_BODY()

	EGeneratedType GeneratedType = EGeneratedType::None; // 生成物类型
	float Damage = 0.0f; // 伤害
	float Radius = 0.0f; // 半径
};

