// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SkillComponent.h"

#include "Actor/Generated/GeneratedBullet.h"
#include "Actor/Generated/GeneratedExplosion.h"
#include "Component/CharacterAttributeComponent.h"


USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USkillComponent::IfBloodLow()
{
	return true;
}

bool USkillComponent::IfInAir() const
{
	return false;
}

void USkillComponent::EffectBuff(FNodeBuffValueFinal BuffValueFinal)
{
	// 根据 Buff 类型分发逻辑

	switch (BuffValueFinal.BuffType)
	{
	case EBuffType::Accelerate: // 移动速度提升
		BuffSpeed(BuffValueFinal.Accelerate, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::Strength: // 攻击力提升
		BuffDamage(BuffValueFinal.Strength, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::AttackSpeed: // 攻速提升
		BuffAttackSpeed(BuffValueFinal.Accelerate, BuffValueFinal.BuffDurationTime);
		break;

	case EBuffType::None:
	case EBuffType::Size: // 弃用
	default:
		break;
	}
    
	UE_LOG(LogTemp, Log, TEXT("Applied Buff: %s, Duration: %f"), 
	   *UEnum::GetValueAsString(BuffValueFinal.BuffType), BuffValueFinal.BuffDurationTime);
}

void USkillComponent::GenerateItem(FNodeGenerateValueFinal GenerateValueFinal)
{
	// BUG: 当前生成物的回调是通过发射对象身上的 SkillComponent 来触发的，会导致如果发射对象提前死亡，生成物的回调将无法触发
	
	if (!OwnerActor) return;

	if (!GeneratedBulletClass || !GeneratedExplosionClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Generated classes not set in SkillComponent!"));
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World) return;

	// 确定生成位置和旋转 (基于 Owner 的朝向)
	FVector SpawnLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * 100.0f; // 稍微靠前一点生成
	FRotator SpawnRotation = OwnerActor->GetActorRotation();

	AGeneratedActor* SpawnedActor = nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 根据类型生成对应的 Actor
	switch (GenerateValueFinal.GeneratedType)
	{
	case EGeneratedType::StandardBullet:
	case EGeneratedType::EnhancedBullet:
		SpawnedActor = World->SpawnActor<AGeneratedBullet>(GeneratedBulletClass, SpawnLocation, SpawnRotation, SpawnParams);
		break;

	case EGeneratedType::Explosion:
		// 爆炸通常在原地或指定位置生成，这里暂定为前方
		SpawnedActor = World->SpawnActor<AGeneratedExplosion>(GeneratedExplosionClass, SpawnLocation, SpawnRotation, SpawnParams);
		break;

	case EGeneratedType::None:
	default:
		break;
	}

	// 初始化生成物数据
	if (SpawnedActor)
	{
		SpawnedActor->InitGeneratedActor(this, GenerateValueFinal.Damage, GenerateValueFinal.Radius);
	}
}

void USkillComponent::ReportSkillHit(AActor* HitTarget)
{
	if (HitTarget)
	{
		TriggerSkillChain(HitTarget);
	}
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	AttributeComponent = OwnerActor ? OwnerActor->GetComponentByClass<UCharacterAttributeComponent>() : nullptr;

	if (!OwnerActor)
		UE_LOG(LogTemp, Error, TEXT("SkillComponent has no Owner Actor!"));
	if (!AttributeComponent)
		UE_LOG(LogTemp, Error, TEXT("Owner Actor has no CharacterAttributeComponent!"));
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillComponent::BuffSpeed(float Multiplier, float Duration)
{
	if (!AttributeComponent) return;
	if (Multiplier <= 0.f) return;
	
	// 应用修改
	AttributeComponent->ModifySpeedRate(Multiplier) ;

	// 设定定时器还原
	FTimerHandle Handle;
	FTimerDelegate Delegate;
	
	// 绑定带参数的回调
	Delegate.BindUObject(this, &USkillComponent::RevertSpeed, Multiplier, Handle);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void USkillComponent::BuffDamage(float Multiplier, float Duration)
{
	if (!AttributeComponent) return;
	if (Multiplier <= 0.f) return;

	AttributeComponent->ModifyDamageRate(Multiplier);
	
	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &USkillComponent::RevertDamage, Multiplier, Handle);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void USkillComponent::BuffAttackSpeed(float Multiplier, float Duration)
{
	if (!AttributeComponent) return;
	if (Multiplier <= 0.f) return;

	AttributeComponent->ModifyAttackSpeedRate(Multiplier);

	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &USkillComponent::RevertAttackSpeed, Multiplier, Handle);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, Duration, false);
	}
}

void USkillComponent::RevertSpeed(float Multiplier, FTimerHandle Handle)
{
	if (!AttributeComponent) return ;
	AttributeComponent->ModifySpeedRate(-Multiplier);
	Handle.Invalidate();
}

void USkillComponent::RevertDamage(float Multiplier, FTimerHandle Handle)
{
	if (!AttributeComponent) return ;
	AttributeComponent->ModifyDamageRate(-Multiplier);

	Handle.Invalidate();
}

void USkillComponent::RevertAttackSpeed(float Multiplier, FTimerHandle Handle)
{
	if (!AttributeComponent) return ;
	AttributeComponent->ModifyAttackSpeedRate(-Multiplier);
	Handle.Invalidate();
}
