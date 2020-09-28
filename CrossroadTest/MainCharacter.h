// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Running UMETA(DisplayName = "Running"),
	EMS_Sprinting UMETA(DisplayName = "Sprint"),
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Max UMETA(DisplayName = "DefaultMax")
};


UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovery UMETA(DisplayName = "ExhaustedRecovery"),
	ESS_Default UMETA(DisplayName = "Default")
};

UCLASS()
class CROSSROADTEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats | Stamina")
	EStaminaStatus StaminaStatus;

	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats | Stamina")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float WalkSpeed;

	bool bShiftPressed;

	bool bXPressed;

	TArray<class APickUpIteminventory*> Inventory;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Raycast();

	class APickUpIteminventory* LastItemSeen;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetCameraComponent() { return Camera; }

	void SetCameraComponent(UCameraComponent* NewCamera) { Camera = NewCamera; }

	USpringArmComponent* GetSpringArmComponent() { return CameraBoom; }

	void SetSpringArmComponent(USpringArmComponent* NewCameraBoom) { CameraBoom = NewCameraBoom; }

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; };

	FORCEINLINE TArray<class APickUpIteminventory*> GetCharInventory() { return Inventory; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void WalkToggle();

	void SprintBegin();

	void SprintEnd();

	void OnAction();

	float NewValueForward;

	float NewValueRight;

	bool bIsStoping;

	bool bIsMovingForward;

	bool bIsMovingRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Stamina")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Stamina")
	float MinimumStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
	float RaycastRange;

	UFUNCTION()
	void HandleInventoryInput();

	UPROPERTY(BlueprintReadWrite)
	class ADoors* CurrentDoor;

	UPROPERTY(BlueprintReadWrite)
	class ACandle* CurrentCandle;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;
};
