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

