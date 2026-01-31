// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/CrashEnemy.h"


// Sets default values
ACrashEnemy::ACrashEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACrashEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrashEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACrashEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

