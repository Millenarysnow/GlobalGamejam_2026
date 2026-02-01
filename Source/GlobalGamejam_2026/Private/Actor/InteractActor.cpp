#include "Actor/InteractActor.h"

AInteractActor::AInteractActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	// 设置碰撞预设，确保能被检测到 (假设使用 WorldDynamic 或自定义交互 Channel)
	MeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
}

void AInteractActor::OnInteract_Implementation(AHero* Interactor)
{
	// 基类默认行为（可选）
	UE_LOG(LogTemp, Log, TEXT("Interacted with %s"), *GetName());
}