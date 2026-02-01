// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FightUI.h"

#include "Character/Hero.h"
#include "Component/CharacterAttributeComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/Subsystem/DirectorSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UFightUI::InitializeWidget(UCharacterAttributeComponent* AttributeComp)
{
	if (AttributeComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterAttributeComponent found on PlayerPawn"));
			
		// 绑定 C++ 函数到代理
		AttributeComp->OnHealthChanged.AddDynamic(this, &UFightUI::FlushBloodUI);

		// 初始化显示（防止一开始是空的）
		FlushBloodUI(AttributeComp->GetHealth(), AttributeComp->GetMaxHealth());
	}
}

void UFightUI::NativeConstruct()
{
	Player = nullptr;

	if (BloodImage)
	{
		BloodMaterialInst = BloodImage->GetDynamicMaterial();

		UE_LOG(LogTemp, Warning, TEXT("Blood Material Instance Created"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BloodImage is null in FightUI"));
	}

	if (ErosionImage)
	{
		ErosionMaterialInst = ErosionImage->GetDynamicMaterial();

		UE_LOG(LogTemp, Warning, TEXT("Erosion Material Instance Created"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ErosionImage is null in FightUI"));
	}

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (AHero* MyChar = Cast<AHero>(OwningPawn))
	{
		// Check 1: 如果组件已经存在了（角色运行得比UI快）
		if (MyChar->GetAttributeComp())
		{
			InitializeWidget(MyChar->GetAttributeComp());
		}
		// Check 2: 如果组件还没创建（UI运行得比角色快），绑定委托等待通知
		else
		{
			// AddDynamic 绑定函数
			MyChar->OnAttributeCompReady.AddDynamic(this, &UFightUI::HandleAttributeCompReady);
		}
	}

	if (GetDirectorSubsystem())
	{
		UE_LOG(LogTemp, Warning, TEXT("DirectorSubsystem found in FightUI"));
		GetDirectorSubsystem()->OnErosionChanged.AddDynamic(this, &UFightUI::FlushErosionUI);

		FlushErosionUI(GetDirectorSubsystem()->GetErosionValue(), GetDirectorSubsystem()->GetMaxErosionValue());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DirectorSubsystem is null in FightUI"));
	}
}

void UFightUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFightUI::FlushErosionUI(float CurrentValue, float MaxValue)
{
	UE_LOG(LogTemp, Warning, TEXT("FlushErosionUI called with CurrentErosion: %.2f, MaxErosion: %.2f"), CurrentValue, MaxValue);
	
	if (!ErosionMaterialInst) return;

	// 计算百分比 (0.0 - 1.0)
	float Percent = (MaxValue > 0.f) ? (CurrentValue / MaxValue) : 0.f;

	// 3. 设置材质参数
	// 注意：这里的 "Percent" 必须和你材质编辑器里 Scalar Parameter 的名字完全一致！
	ErosionMaterialInst->SetScalarParameterValue(FName("Percent"), Percent);

	ErosionText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentValue, MaxValue)) );
}

void UFightUI::FlushBloodUI(float CurrentHealth, float MaxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("FlushBloodUI called with CurrentHealth: %.2f, MaxHealth: %.2f"), CurrentHealth, MaxHealth);
	
	if (!BloodMaterialInst) return;

	// 计算百分比 (0.0 - 1.0)
	float Percent = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;

	// 3. 设置材质参数
	// 注意：这里的 "Percent" 必须和你材质编辑器里 Scalar Parameter 的名字完全一致！
	BloodMaterialInst->SetScalarParameterValue(FName("Percent"), Percent);

	BloodText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth)) );
}

AHero* UFightUI::GetPlayer()
{
	if (Player) return Player;

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this,0);
	if (!Character) Player = nullptr;
	else Player = Cast<AHero>(Character);

	return Player;
}

UDirectorSubsystem* UFightUI::GetDirectorSubsystem()
{
	if (DirectorSubsystem) return DirectorSubsystem;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance) return nullptr;

	DirectorSubsystem = GameInstance->GetSubsystem<UDirectorSubsystem>();
	return DirectorSubsystem;
}

void UFightUI::HandleAttributeCompReady(UCharacterAttributeComponent* NewComp)
{
	InitializeWidget(NewComp);
    
	// 可选：解绑委托（如果不需要再次初始化）
	if (AHero* MyChar = Cast<AHero>(GetOwningPlayerPawn()))
	{
		MyChar->OnAttributeCompReady.RemoveDynamic(this, &UFightUI::HandleAttributeCompReady);
	}
}
