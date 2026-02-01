#include "Actor/WinGameInteractActor.h" // 假设你放在 Actor 目录下
#include "Gameplay/Subsystem/DirectorSubsystem.h"
#include "Kismet/GameplayStatics.h"

void AWinGameInteractActor::OnInteract_Implementation(AHero* Interactor)
{
	Super::OnInteract_Implementation(Interactor);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDirectorSubsystem* DirectorSubsystem = GI->GetSubsystem<UDirectorSubsystem>())
		{
			DirectorSubsystem->PassPerGame();
			
			// 可选：通关后销毁自身防止重复触发
			Destroy(); 
		}
	}
}