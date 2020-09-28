// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricalMainCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Doors.h"
#include "Candle.h"

// Sets default values
AIsometricalMainCharacter::AIsometricalMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TriggerComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerBox"));
	TriggerComp->InitCapsuleSize(55.f, 96.0f);
	TriggerComp->SetCollisionProfileName(TEXT("Trigger"));
	TriggerComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AIsometricalMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AIsometricalMainCharacter::OnOverlapBegin);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &AIsometricalMainCharacter::OnOverlapEnd);
}

// Called every frame
void AIsometricalMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CursorToWorld != nullptr)
	{
		
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AIsometricalMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ADoors::StaticClass()))
	{ 
		CurrentDoor = Cast<ADoors>(OtherActor);
	}
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ACandle::StaticClass()))
	{ 
		CurrentCandle = Cast<ACandle>(OtherActor);
	}
	UE_LOG(LogTemp, Warning, TEXT("You hit"));
}

void AIsometricalMainCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		CurrentDoor = NULL;
		CurrentCandle = NULL;
	}
}

void AIsometricalMainCharacter::OnAction()
{
	if (CurrentDoor)
	{
		FVector ForwardVector = GetActorForwardVector();
		CurrentDoor->ToggleDoor(ForwardVector);
	}
	if (CurrentCandle) {
		CurrentCandle->ToggleCandle();
	}
}
