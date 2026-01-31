#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/OnHit.h"
#include "CharacterBase.generated.h"


class USkillComponent;
class UCharacterAttributeComponent;


UCLASS()
class GLOBALGAMEJAM_2026_API ACharacterBase : public ACharacter, public IOnHit
{
	GENERATED_BODY()

public:
	ACharacterBase();

	virtual void OnGetHit_Implementation(float Damage, AActor* InstigatorActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkillComponent> SkillComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCharacterAttributeComponent> AttributeComponentClass;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	USkillComponent* SkillComponent;

	UPROPERTY()
	UCharacterAttributeComponent* AttributeComponent;
};
