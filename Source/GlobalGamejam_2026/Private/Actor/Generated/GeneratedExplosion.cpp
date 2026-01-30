// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Generated/GeneratedExplosion.h"


// Sets default values
AGeneratedExplosion::AGeneratedExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeneratedExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneratedExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

