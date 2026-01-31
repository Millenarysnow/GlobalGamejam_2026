#include "Character/CharacterBase.h"

#include "Component/CharacterAttributeComponent.h"
#include "Component/SkillComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::OnGetHit_Implementation(float Damage, AActor* InstigatorActor)
{
	if (AttributeComponent)
	{
		AttributeComponent->ModifyHealth(-Damage);
	}
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SkillComponent = NewObject<USkillComponent>(this, SkillComponentClass);
	if (SkillComponent) 
	{
		SkillComponent->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SkillComponent!"));
	}

	AttributeComponent = NewObject<UCharacterAttributeComponent>(this, AttributeComponentClass);
	if (AttributeComponent)
	{
		AttributeComponent->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CharacterAttributeComponent!"));
	}
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

