#include "Character/Enemy.h"
#include "Component/CharacterAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h" 
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Enemy");

	// 确保生成时自动创建AIController
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// GetCharacterMovement()->bOrientRotationToMovement = true; // 角色朝向移动方向
	// GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 转身速率
	// GetCharacterMovement()->bConstrainToPlane = true; // 2.5D 通常限制在平面移动
	// GetCharacterMovement()->bSnapToPlaneAtStart = true;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 获取 AIController
	EnemyAIController = Cast<AAIController>(GetController());

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		*FString::Printf(TEXT("Enemy: AIController is %s"), EnemyAIController ? *EnemyAIController->GetName() : TEXT("null")));

	// 初始化移动速度
	if (AttributeComponent && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = AttributeComponent->GetSpeed();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 实时同步速度（支持动态变速）
	if (AttributeComponent && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = AttributeComponent->GetSpeed();

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan,
			*FString::Printf(TEXT("Enemy: Current Speed : %.2f"), GetCharacterMovement()->MaxWalkSpeed) );
	}
}

AActor* AEnemy::GetTargetPlayer() const
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
		*FString::Printf(TEXT("Enemy: Getting Target Player : %s"), *UGameplayStatics::GetPlayerPawn(this, 0)->GetName()));
	return UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemy::NavMoveToTarget(AActor* Target, float AcceptanceRadius)
{
	if (EnemyAIController && Target)
	{
		// 使用 UE 内置寻路 MoveToActor
		// SetFocus 会让敌人自动转向目标

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
			*FString::Printf(TEXT("Enemy: Moving To Target : %s"), *Target->GetName()));
		
		EPathFollowingRequestResult::Type result = EnemyAIController->MoveToActor(Target, AcceptanceRadius);

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow,
			*FString::Printf(TEXT("Enemy: MoveToActor Result : %d"), static_cast<int32>(result)));
		
		EnemyAIController->SetFocus(Target); 
	}
}

void AEnemy::FaceTarget(AActor* Target)
{
	// 如果使用了 SetFocus，这一步通常由 AIController 自动完成
	// 但如果需要手动干预，可以保留
	if (!Target) return;
	FVector Direction = Target->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.f;
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), Direction.Rotation(), GetWorld()->GetDeltaSeconds(), 10.0f);
	SetActorRotation(NewRot);
}