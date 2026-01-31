#include "Component/CharacterAttributeComponent.h"


UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSpeed = InitSpeed;
	CurrentDamage = InitDamage;
	CurrentAttackSpeed = InitAttackSpeed;
	CurrentHealth = InitHealth;
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

void UCharacterAttributeComponent::ModifySpeedRate(float rate)
{
	SpeedModifyRate += rate;
	SpeedModifyRate = FMath::Max(SpeedModifyRate, 0.1f);
	CurrentSpeed = FMath::Clamp(InitSpeed * SpeedModifyRate, 0.f, MaxSpeed);
}

void UCharacterAttributeComponent::ModifyDamageRate(float rate)
{
	DamageModifyRate += rate;
	DamageModifyRate = FMath::Max(DamageModifyRate, 0.1f);
	CurrentDamage = FMath::Clamp(InitDamage * DamageModifyRate, 0.f, MaxDamage);
}

void UCharacterAttributeComponent::ModifyAttackSpeedRate(float rate)
{
	AttackSpeedModifyRate += rate;
	AttackSpeedModifyRate = FMath::Max(AttackSpeedModifyRate, 0.1f);
	CurrentAttackSpeed = FMath::Clamp(InitAttackSpeed * AttackSpeedModifyRate, 0.f, MaxAttackSpeed);
}

void UCharacterAttributeComponent::ModifyHealth(float DeltaValue)
{
	// 防止死亡后鞭尸或过度治疗
	if (CurrentHealth <= 0.0f && DeltaValue < 0.0f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + DeltaValue, 0.0f, MaxHealth);

	// 这里可以添加死亡广播，例如：
	// if (CurrentHealth <= 0.0f) { OnDeath.Broadcast(); }
}

