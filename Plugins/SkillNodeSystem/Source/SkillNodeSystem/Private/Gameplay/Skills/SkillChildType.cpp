#include "Gameplay/Skills/SkillChildType.h"

#include <cmath>

FBranchNodeInfo::FBranchNodeInfo(EBranchType _BranchType, float _DelayTime, float _BranchValue)
{
	NodeType = ESkillNodeType::BranchNode; 
	SwitchTargetType = ESwitchTargetType::Unchanged; 
	DelayTime = _DelayTime; 
	OutPinCount = 2; 
	BranchType = _BranchType;

	BranchValue = _BranchValue;
}

FBuffNodeInfo::FBuffNodeInfo(EBuffType _BuffType, float _BuffDurationTime, float _BuffValue, float _DelayTime, int32 _OutPinCount)
{
	NodeType = ESkillNodeType::BuffNode; 
	SwitchTargetType = ESwitchTargetType::Unchanged;
	DelayTime = _DelayTime;
	OutPinCount = _OutPinCount;
	BuffType = _BuffType;

	BuffDurationTime = _BuffDurationTime; // buff持续时间
	BuffValue = _BuffValue; // buff值

	checkf(_OutPinCount >= 0, TEXT("Millenarysnow : FBuffNodeInfo OutPinCount must be a non-negative number"));
}

FGenerateNodeInfo::FGenerateNodeInfo(EGeneratedType _GeneratedType, float _Damage, float _Radius, float _DelayTime, int32 _OutPinCount)
{
	NodeType = ESkillNodeType::GenerateNode; 
	SwitchTargetType = ESwitchTargetType::Changed; 
	DelayTime = _DelayTime; 
	OutPinCount = _OutPinCount;
	GeneratedType = _GeneratedType;

	Damage = _Damage; // 伤害
	Radius = _Radius; // 半径

	checkf(_OutPinCount >= 0, TEXT("Millenarysnow : FGenerateNodeInfo OutPinCount must be a non-negative number"));
}

FLoopNodeInfo::FLoopNodeInfo(float _DelayTime, int32 _OutPinCount, int32 _LoopCount)
{
	NodeType = ESkillNodeType::LoopStartNode;
	SwitchTargetType = ESwitchTargetType::Unchanged; 
	DelayTime = _DelayTime; 
	OutPinCount = _OutPinCount; 

	LoopCount = _LoopCount;

	checkf(_OutPinCount >= 0, TEXT("Millenarysnow : FLoopNodeInfo OutPinCount must be a non-negative number"));
}

FParamNodeInfo::FParamNodeInfo(EParamType _ParamType, int32 _ParamIntValue, float _ParamFloatValue, int32 _OutPinCount)
{
	NodeType = ESkillNodeType::ParamNode; 
	SwitchTargetType = ESwitchTargetType::Unchanged; 
	DelayTime = 0;
	OutPinCount = _OutPinCount; 
	ParamType = _ParamType;

	ParamIntValue = _ParamIntValue; // 整型参数
	ParamFloatValue = _ParamFloatValue; // 浮点型参数

	checkf(_OutPinCount >= 0, TEXT("Millenarysnow : FParamNodeInfo OutPinCount must be a non-negative number"));
}

FStartNodeInfo::FStartNodeInfo(EStartNodeType _StartNodeType, float _DelayTime, int32 _OutPinCount)
{
	NodeType = ESkillNodeType::StartNode; 
	SwitchTargetType = ESwitchTargetType::Unchanged; 
	DelayTime = std::max(_DelayTime, 100.0f);
	OutPinCount = _OutPinCount; 
	StartNodeType = _StartNodeType;

	checkf(_OutPinCount >= 0, TEXT("Millenarysnow : FStartNodeInfo OutPinCount must be a non-negative number"));
}
