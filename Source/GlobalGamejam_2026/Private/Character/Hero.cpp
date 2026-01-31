#include "Character/Hero.h"


AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Player");
}

void AHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

