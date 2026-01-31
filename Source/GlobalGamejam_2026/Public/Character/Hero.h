#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "InputActionValue.h" // 引入 EnhancedInput 值类型
#include "Hero.generated.h"

class AHeroController;
class UPauseWidget;
class USkillCanvasWidget;
// 前置声明
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UENUM()
enum class EPlayerType : uint8
{
	None = 0,
	Playing,
	Backpack,
	Pause
};


UCLASS()
class GLOBALGAMEJAM_2026_API AHero : public ACharacterBase
{
	GENERATED_BODY()

public:
	AHero();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** 摄像机吊臂 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	/** 跟随摄像机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// ----- 部分蓝图类 ----- 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TSubclassOf<USkillCanvasWidget> BackpackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TSubclassOf<UPauseWidget> PauseMenuClass;

	// --- Enhanced Input 资源配置 ---
	
	/** 输入映射上下文 (IMC) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** 移动输入 (WASD) - Axis2D */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** 缩放输入 (鼠标滚轮) - Axis1D */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;

	/** 打开背包 (H) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InventoryAction;

	/** 交互 (F) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	/** 暂停 (ESC) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;

	/** 主要能力 (左键) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PrimaryAbilityAction;

	/** 次要能力 (右键) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SecondaryAbilityAction;

protected:
	// --- 输入回调函数 ---

	/** 处理移动 */
	void Move(const FInputActionValue& Value);

	/** 处理摄像机缩放 */
	void Zoom(const FInputActionValue& Value);

	/** 打开背包 */
	void OpenInventory(const FInputActionValue& Value);

	/** 交互 */
	void Interact(const FInputActionValue& Value);

	/** 暂停游戏 */
	void PauseGame(const FInputActionValue& Value);

	/** 主要能力释放 */
	void PrimaryAbility(const FInputActionValue& Value);

	/** 次要能力释放 */
	void SecondaryAbility(const FInputActionValue& Value);

private:
	UPROPERTY()
	USkillCanvasWidget* Backpack;

	UPROPERTY()
	UPauseWidget* PauseMenu;

	UFUNCTION()
	void SwitchPlayerType(EPlayerType NewType);

	EPlayerType CurrentType = EPlayerType::Playing;
	
	bool bInBackpack = false;

	UPROPERTY()
	AHeroController* HeroController;
};