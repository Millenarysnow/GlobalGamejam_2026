#include "Gameplay/UI/SkillTooltipWidget.h"
#include "Gameplay/Skills/SkillNode.h"
#include "Gameplay/Subsystem/SkillUISubsystem.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void USkillTooltipWidget::UpdateTooltipData(USkillNode* InNode)
{
	if (!InNode) return;

	// 1. 设置节点名称
	if (NodeNameText)
	{
		FString Name = UEnum::GetValueAsString(InNode->GetNodeType());
		int32 Index = Name.Find("::");
		if (Index != INDEX_NONE) Name = Name.RightChop(Index + 2);
		NodeNameText->SetText(FText::FromString(Name));
	}

	// 2. 设置类型描述
	if (NodeDescText)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (USkillUISubsystem* UISub = GI->GetSubsystem<USkillUISubsystem>())
			{
				FString Desc = UISub->GetDescribeTextByType(InNode->GetNodeType());
				NodeDescText->SetText(FText::FromString(Desc));
			}
		}
	}

	// 3. 设置具体属性
	if (PropertyListContainer)
	{
		PropertyListContainer->ClearChildren();

		const FSkillNodeInfo& Info = InNode->GetNodeInfo();

		// --- 根据类型显示特定属性 ---
		switch (Info.NodeType)
		{
		case ESkillNodeType::GenerateNode:
			AddPropertyRow("Type", UEnum::GetValueAsString(Info.GeneratedType));
			AddPropertyRow("Damage", Info.Damage);
			AddPropertyRow("Radius", Info.Radius);
			break;
		case ESkillNodeType::BuffNode:
			AddPropertyRow("Type", UEnum::GetValueAsString(Info.BuffType));
			AddPropertyRow("Value", Info.BuffValue);
			AddPropertyRow("Duration", Info.BuffDurationTime);
			break;
		case ESkillNodeType::BranchNode:
			AddPropertyRow("Type", UEnum::GetValueAsString(Info.BranchType));
			AddPropertyRow("Threshold", Info.BranchValue);
			break;
		case ESkillNodeType::ParamNode:
			AddPropertyRow("Type", UEnum::GetValueAsString(Info.ParamType));
			if (Info.ParamType == EParamType::Damage || Info.ParamType == EParamType::EffectiveTime || Info.ParamType == EParamType::Radius || Info.ParamType == EParamType::ReduceDelayTime)
			{
				AddPropertyRow("Float Val", Info.ParamFloatValue);
			}
			else
			{
				AddPropertyRow("Int Val", Info.ParamIntValue);
			}
			break;
		case ESkillNodeType::LoopStartNode:
			AddPropertyRow("Loop Count", Info.LoopCount);
			break;
		case ESkillNodeType::StartNode:
			AddPropertyRow("Trigger", UEnum::GetValueAsString(Info.StartNodeType));
			break;
		default:
			break;
		}

		// --- 固定显示的最后两行 ---
		// 节点自身延迟 (配置值)
		AddPropertyRow("Self Delay", Info.DelayTime);
		
		// 子树总延迟 (计算值)
		AddPropertyRow("Total Delay", InNode->GetDelayTime());
	}
}

void USkillTooltipWidget::AddPropertyRow(const FString& Label, const FString& Value)
{
	if (!PropertyListContainer) return;

	// 创建一个新的 TextBlock
	UTextBlock* NewText = NewObject<UTextBlock>(PropertyListContainer);
	
	// 格式化文本 "Label : Value"
	// 处理枚举值的 "Type::Value" 清理
	FString CleanValue = Value;
	int32 Index = CleanValue.Find("::");
	if (Index != INDEX_NONE) CleanValue = CleanValue.RightChop(Index + 2);

	FString FinalStr = FString::Printf(TEXT("%s : %s"), *Label, *CleanValue);
	NewText->SetText(FText::FromString(FinalStr));
	
	// 设置简单的样式 (可选)
	NewText->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 10));

	PropertyListContainer->AddChildToVerticalBox(NewText);
}

void USkillTooltipWidget::AddPropertyRow(const FString& Label, float Value)
{
	// 格式化浮点数，保留2位小数
	AddPropertyRow(Label, FString::Printf(TEXT("%.2f"), Value));
}

void USkillTooltipWidget::AddPropertyRow(const FString& Label, int32 Value)
{
	AddPropertyRow(Label, FString::FromInt(Value));
}