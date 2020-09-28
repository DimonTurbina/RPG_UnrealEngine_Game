// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "PickUpItem.h"
#include "Containers/Array.h"
#include "PickUpIteminventory.h"
#include "Engine/World.h"
#include "MainPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Doors.h"
#include "Candle.h"

AMainCharacter::AMainCharacter()
{
	//SorceControll Test
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 90.0f);

	//Set camera and camera spring arm settings
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 400;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetOffset = FVector(0.0f, -70.0f, 50.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//Our mesh will not get rotation by controller
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	//character movement settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//Capsule for overlap event (do not use root component capsule for this purposes)
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);


	NewValueForward = 0.0f;
	NewValueRight = 0.0f;

	bIsStoping = false;

	//Max stats
	MaxHealth = 100.f;
	MaxStamina = 100.f;

	//Test stats
	Health = 70.0f;
	Stamina = 70.0f;

	RunSpeed = 500;
	WalkSpeed = 120;
	SprintSpeed = 850;

	//In .h files we have enum classes for stamina and for movement mode use them to change velocity in SetMovementStatus function
	MovementStatus = EMovementStatus::EMS_Running;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 5.f;
	MinimumStamina = 20.f;

	RaycastRange = 700.0f;

	Inventory.SetNum(8);

	CurrentDoor = NULL;
	CurrentCandle = NULL;
}

//This Function is for changing movement status and velocity
void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	if (MovementStatus == EMovementStatus::EMS_Running)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	if (MovementStatus == EMovementStatus::EMS_Walking)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	LastItemSeen = nullptr;

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);
}

void AMainCharacter::Raycast()
{
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + (Camera->GetForwardVector() * RaycastRange);

	FHitResult Hit;
		
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CQP);
		//UE_LOG(LogTemp, Warning, TEXT("Hit"))
	
	APickUpIteminventory* Item = Cast<APickUpIteminventory>(Hit.GetActor());

	//Some debug inforamtion. Use it to see which actor was hitted by raycast

	//if (GEngine && Hit.GetActor())
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("hit %s"), *Hit.Actor->GetName()));
	//UE_LOG(LogTemp, Warning, TEXT("You hit %s"), RaycastHit.Actor->GetName());
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1, 0, 1.f);

	if (LastItemSeen && LastItemSeen != Item)
	{
		//If our character sees a different pickup then disable the glowing effect on the previous seen item
		LastItemSeen->SetGlowEffect(false);
	}
	if (Item) {
		LastItemSeen = Item;
		Item->SetGlowEffect(true);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftPressed) {
			if (Stamina - DeltaStamina <= MinimumStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			else
			{
				Stamina -= DeltaStamina;
			}
		}
		else {
			if (Stamina + DeltaStamina >= MaxStamina) {
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftPressed) {
			if (Stamina - DeltaStamina <= 0.f) {
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Running);
			}
			else
			{
				Stamina -= DeltaStamina;
			}
		}
		else
		{
			if (Stamina + DeltaStamina >= MinimumStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}

		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftPressed) {
			Stamina = 0.0f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovery);
			Stamina += DeltaStamina;
		}
		if (bXPressed)
			SetMovementStatus(EMovementStatus::EMS_Walking);
		else
			SetMovementStatus(EMovementStatus::EMS_Running);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovery:
		if (Stamina + DeltaStamina >= MinimumStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		if (bXPressed)
			SetMovementStatus(EMovementStatus::EMS_Walking);
		else
			SetMovementStatus(EMovementStatus::EMS_Running);
		break;
	default:
		;
	}

	Raycast();

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMainCharacter::SprintBegin);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMainCharacter::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &AMainCharacter::WalkToggle);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMainCharacter::OnAction);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMainCharacter::HandleInventoryInput);
}

//Overlap. Add here some code for adding you class to overlap
void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ADoors::StaticClass()))
	{
		CurrentDoor = Cast<ADoors>(OtherActor);
	}

	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ACandle::StaticClass()))
	{
		CurrentCandle = Cast<ACandle>(OtherActor);
	}

	UE_LOG(LogTemp, Warning, TEXT("You hit"));
}

void AMainCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CurrentDoor = NULL;
		CurrentCandle = NULL;
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		NewValueForward= FMath::FInterpTo(MovementStatus != EMovementStatus::EMS_Walking ? NewValueForward : Value, Value > 0.f ? 1.0f : -1.0f, GetWorld()->GetDeltaSeconds(), 1.0f);
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator = FRotator(0.0f, Rotation.Yaw, 0.0f);
		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewValueForward);
		bIsMovingForward = true;
		bIsMovingRight = false;
	}
	if (Value == 0 && !bIsMovingRight)
	{
		bIsMovingForward = true;
		NewValueForward = FMath::FInterpTo(MovementStatus != EMovementStatus::EMS_Walking ? NewValueForward : Value, 0.0f, GetWorld()->GetDeltaSeconds(), 3.0f);
		if (FMath::Abs(NewValueForward) < 0.05f)
			NewValueForward = 0;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator = FRotator(0.0f, Rotation.Yaw, 0.0f);

		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewValueForward);
		//GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (bIsMovingRight)
		NewValueForward = 0;
}

void AMainCharacter::MoveRight(float Value)
{	
	if ((Controller != nullptr) && (Value != 0.0f)) {
		bIsMovingRight = true;
		bIsMovingForward = false;
		//GetCharacterMovement()->bOrientRotationToMovement = true;
		NewValueRight = FMath::FInterpTo(MovementStatus != EMovementStatus::EMS_Walking ? NewValueRight : Value, Value > 0 ? 1.0f : -1.0f, GetWorld()->GetDeltaSeconds(), 1.0f);
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator = FRotator(0.0f, Rotation.Yaw, 0.0f);

		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, NewValueRight);
	}
	if (Value == 0 && !bIsMovingForward) {
		bIsMovingRight = true;
		NewValueRight = FMath::FInterpTo(MovementStatus != EMovementStatus::EMS_Walking ? NewValueRight : Value, 0.0f, GetWorld()->GetDeltaSeconds(), 3.0f);
		if (FMath::Abs(NewValueRight) < 0.05f )
			NewValueRight = 0;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator = FRotator(0.0f, Rotation.Yaw, 0.0f);

		FVector Direction = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, NewValueRight);
		//GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (bIsMovingForward)
		NewValueRight = 0;
}

void AMainCharacter::WalkToggle()
{
	bXPressed = !bXPressed;
	if (bXPressed)
		SetMovementStatus(EMovementStatus::EMS_Walking);
	else
		SetMovementStatus(EMovementStatus::EMS_Running);
}

void AMainCharacter::SprintBegin()
{
	SetMovementStatus(EMovementStatus::EMS_Sprinting);
	bShiftPressed = true;
}

void AMainCharacter::SprintEnd()
{
	SetMovementStatus(EMovementStatus::EMS_Running);
	bShiftPressed = false;
}

void AMainCharacter::HandleInventoryInput()
{
	AMainPlayerController* Con = Cast<AMainPlayerController>(GetController());
	if (Con) Con->HandleInventoryInput();
}

void AMainCharacter::OnAction()
{
	if (CurrentDoor)
	{
		FVector ForwardVector = Camera->GetForwardVector();
		CurrentDoor->ToggleDoor(ForwardVector);
	}
	if (CurrentCandle) {
		CurrentCandle->ToggleCandle();
	}
	
}