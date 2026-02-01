#include "Character/Hero.h"

#include "BoundShaderStateCache.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/CharacterAttributeComponent.h"
#include "Gameplay/HeroController.h"
#include "Gameplay/UI/SkillCanvasWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/PauseWidget.h"

AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Player");

	// 1. 配置胶囊体 (ACharacter 默认自带，这里可按需调整大小)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 2. 配置角色移动
	// 禁止控制器旋转影响角色本身（角色朝向由移动方向决定）
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // 角色朝向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 转身速率
	GetCharacterMovement()->bConstrainToPlane = true; // 2.5D 通常限制在平面移动
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 3. 创建摄像机吊臂 (SpringArm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // 使用绝对旋转，不随角色转动
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f)); // 需求：45度斜上方
	CameraBoom->TargetArmLength = 800.f; // 初始距离
	CameraBoom->bDoCollisionTest = false; // 2.5D 视角通常不需要相机碰撞检测（防止穿墙抖动）

	// 4. 创建摄像机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 摄像机不随控制器旋转
}

void AHero::BeginPlay()
{
	Super::BeginPlay();

	// 添加输入映射上下文
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 确保在蓝图中设置了 DefaultMappingContext
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (AController* C = GetController())
	{
		HeroController = Cast<AHeroController>(C);
		if (!HeroController)
		{
			UE_LOG(LogTemp, Error, TEXT("Controller is not HeroController!"));
		}
	}
	
	if (!BackpackClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseClass is not set in Hero blueprint."));	
	}
	else
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			Backpack = CreateWidget<USkillCanvasWidget>(PlayerController, BackpackClass);
			Backpack->AddToViewport();
			Backpack->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (!PauseMenuClass)
	{
		UE_LOG(LogTemp, Error, TEXT("BackpackClass is not set in Hero blueprint."));	
	}
	else
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			PauseMenu = CreateWidget<UPauseWidget>(PlayerController, PauseMenuClass);
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定 Enhanced Input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 移动
		if (MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHero::Move);

		// 缩放
		if (ZoomAction)
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AHero::Zoom);

		// 功能按键
		if (InventoryAction)
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AHero::OpenInventory);

		if (InteractAction)
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHero::Interact);

		if (PauseAction)
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AHero::PauseGame);

		if (PrimaryAbilityAction)
			EnhancedInputComponent->BindAction(PrimaryAbilityAction, ETriggerEvent::Triggered, this, &AHero::PrimaryAbility);

		if (SecondaryAbilityAction)
			EnhancedInputComponent->BindAction(SecondaryAbilityAction, ETriggerEvent::Triggered, this, &AHero::SecondaryAbility);
	}
}

// 减速固定为25
void AHero::OnIHit_Implementation(float DamageAmount, float SlowDuration, AActor* HitInstigator)
{
	UE_LOG(LogTemp, Log, TEXT("Hero hit: Damage=%.2f, SlowDuration=%.2f"), DamageAmount, SlowDuration);
	
	if (!AttributeComponent) return;

	// ------- 处理伤害 -------

	AttributeComponent->ModifyHealth(-DamageAmount);
	
	// ------- 应用减速 -------
	
	float Multiplier = 0.25f;
	
	// 应用修改
	AttributeComponent->ModifySpeedRate(Multiplier) ;

	// 设定定时器还原
	FTimerHandle Handle;
	FTimerDelegate Delegate;
	
	// 绑定带参数的回调
	Delegate.BindUObject(this, &AHero::RecoverFromSlow, Handle);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(Handle, Delegate, SlowDuration, false);
	}
}

void AHero::RecoverFromSlow(FTimerHandle Handle)
{
	if (!AttributeComponent) return ;
	AttributeComponent->ModifySpeedRate(0.25f);
	Handle.Invalidate();
}

float AHero::GetHealthPercent()
{
	if (!AttributeComponent) return 0.f;
	return FMath::Clamp((AttributeComponent->GetHealth() + 1.f) / (AttributeComponent->GetMaxHealth() + 1.f), 0.f, 1.f);
}

void AHero::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 找出摄像机的前方和右方，确保按 W 总是朝屏幕上方/斜上方走
		const FRotator Rotation = CameraBoom->GetComponentRotation(); // 使用吊臂的绝对旋转
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取前向和右向向量
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHero::Zoom(const FInputActionValue& Value)
{
	// 获取滚轮值 (通常是 1.0 或 -1.0)
	float ZoomValue = Value.Get<float>();
	
	if (CameraBoom)
	{
		// 调整吊臂长度，限制在 300 到 1500 之间
		float NewLength = CameraBoom->TargetArmLength + (ZoomValue * -50.f); // -50 是缩放速度，负号是为了滚轮向前是放大（距离变短）
		CameraBoom->TargetArmLength = FMath::Clamp(NewLength, 300.f, 1500.f);
	}
}

void AHero::OpenInventory(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Open Inventory Pressed"));

	SwitchPlayerType(EPlayerType::Backpack);
}

void AHero::Interact(const FInputActionValue& Value)
{
	// TODO: 实现交互逻辑
	UE_LOG(LogTemp, Log, TEXT("Interact Pressed"));
}

void AHero::PauseGame(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Pause Game Pressed"));

	if (CurrentType == EPlayerType::Playing)
	{
		SwitchPlayerType(EPlayerType::Pause);
	}
	else
	{
		SwitchPlayerType(EPlayerType::Playing);
	}
}

void AHero::PrimaryAbility(const FInputActionValue& Value)
{
	HeroController->LeftMousePressed();
}

void AHero::SecondaryAbility(const FInputActionValue& Value)
{
	HeroController->RightMousePressed();
}

void AHero::SwitchPlayerType(EPlayerType NewType)
{
	switch (NewType)
	{
	case EPlayerType::None:
		break;
	case EPlayerType::Playing:
		if (CurrentType == EPlayerType::Backpack)
		{
			if (!Backpack) return ;
			Backpack->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (CurrentType == EPlayerType::Pause)
		{
			// BUG: 从Pause回到Playing时，游戏当前仍在暂停，导致接收不到输入
			if (!PauseMenu) return ;
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		CurrentType = NewType;
		break;
	case EPlayerType::Backpack:
		if (CurrentType == EPlayerType::Playing)
		{
			if (!Backpack) return ;
			Backpack->RefreshCanvas();
			Backpack->SetVisibility(ESlateVisibility::Visible);
		}
		CurrentType = NewType;
		break;
	case EPlayerType::Pause:
		if (CurrentType == EPlayerType::Playing)
		{
			if (!PauseMenu) return ;
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
		CurrentType = NewType;
		break;
	default:
		break;	
	}
}
