#include "Component/CharacterAttributeComponent.h"


UCharacterAttributeComponent::UCharacterAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// TODO: 记录此处问题
/**
 * 这里遇到坑
 * 如果在构造函数中初始化 CurrentSpeed 等变量，

C++ 构造函数执行：当你运行游戏或编译蓝图时，UCharacterAttributeComponent 的 C++ 构造函数首先执行。此时执行了 CurrentSpeed = InitSpeed;。因为此时蓝图的数据还没加载进来，InitSpeed 使用的是 C++ 默认值 50。所以 CurrentSpeed 被赋值为 50。

蓝图属性应用：构造函数执行完毕后，引擎才会将你在蓝图编辑器中设置的数值（比如 InitSpeed = 2000）反序列化并覆盖到组件上。此时 InitSpeed 变成了 2000，但 CurrentSpeed 仍然是 50（因为它没有再更新）。

BeginPlay 执行：游戏开始。如果 BeginPlay 里没有重新赋值，你的角色就会一直用 50 的速度跑。
 
 */
void UCharacterAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = InitSpeed;
	CurrentDamage = InitDamage;
	CurrentAttackSpeed = InitAttackSpeed;
	CurrentHealth = InitHealth;
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

