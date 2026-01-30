// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/GeneratedActor.h"
#include "GeneratedBullet.generated.h"

UCLASS()
class GLOBALGAMEJAM_2026_API AGeneratedBullet : public AGeneratedActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGeneratedBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
