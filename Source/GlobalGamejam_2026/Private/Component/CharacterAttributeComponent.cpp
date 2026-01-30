// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterAttributeComponent.h"


// Sets default values for this component's properties
UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Health = MaxHealth;
	Speed = MaxSpeed;
	Damage = MaxDamage;
	AttackSpeed = MaxAttackSpeed;
}


// Called when the game starts
void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterAttributeComponent::BuffSpeed(float Multiplier, float Duration)
{
	if (Multiplier <= 0.f) return;

	// 1. 应用修改 (假设是乘法叠加)
	Speed *= Multiplier;

	// 2. 设定定时器还原
	FTimerHandle Handle;
	FTimerDelegate Delegate;
	// 绑定带参数的回调，传入刚才乘的倍率以便除回去
	Delegate.BindUObject(this, &UCharacterAttributeComponent::RevertSpeed, Multiplier);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void UCharacterAttributeComponent::BuffDamage(float Multiplier, float Duration)
{
	if (Multiplier <= 0.f) return;

	Damage *= Multiplier;

	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UCharacterAttributeComponent::RevertDamage, Multiplier);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void UCharacterAttributeComponent::BuffAttackSpeed(float Multiplier, float Duration)
{
	if (Multiplier <= 0.f) return;

	AttackSpeed *= Multiplier;

	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UCharacterAttributeComponent::RevertAttackSpeed, Multiplier);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void UCharacterAttributeComponent::RevertSpeed(float Multiplier)
{
	if (Multiplier != 0.f)
	{
		Speed /= Multiplier;
		// 简单的数值修正，防止浮点误差导致 Speed 变为 99.999
		if (FMath::IsNearlyEqual(Speed, MaxSpeed, 0.1f)) Speed = MaxSpeed; 
	}
}

void UCharacterAttributeComponent::RevertDamage(float Multiplier)
{
	if (Multiplier != 0.f)
	{
		Damage /= Multiplier;
		if (FMath::IsNearlyEqual(Damage, MaxDamage, 0.1f)) Damage = MaxDamage;
	}
}

void UCharacterAttributeComponent::RevertAttackSpeed(float Multiplier)
{
	if (Multiplier != 0.f)
	{
		AttackSpeed /= Multiplier;
		if (FMath::IsNearlyEqual(AttackSpeed, MaxAttackSpeed, 0.1f)) AttackSpeed = MaxAttackSpeed;
	}
}
