// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FightUI.h"

#include "Character/Hero.h"
#include "Components/Image.h"
#include "Gameplay/Subsystem/DirectorSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UFightUI::NativeConstruct(const FGeometry& MyGeometry, float InDeltaTime)
{
	Player = nullptr;

	if (BloodImage)
	{
		BloodMaterialInst = BloodImage->GetDynamicMaterial();
	}
}

void UFightUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetPlayer())
	{
		FlushBloodUI(GetPlayer()->GetHealthPercent());
	}
}

void UFightUI::FlushErosionUI()
{
	if (!GetDirectorSubsystem()) return ;

	GetDirectorSubsystem()->GetErosionRate();

	// ...
}

void UFightUI::FlushBloodUI(float CurrentHealthPercent)
{
	if (BloodMaterialInst)
	{
		BloodMaterialInst->SetScalarParameterValue(FName("Percent"), CurrentHealthPercent);
	}
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
