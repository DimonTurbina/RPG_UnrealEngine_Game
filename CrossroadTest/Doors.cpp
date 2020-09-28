// Fill out your copyright notice in the Description page of Project Settings.


#include "Doors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoors::ADoors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	BoxComp->InitBoxExtent(FVector(150, 100, 100));
	BoxComp->SetCollisionProfileName("Trigger");
	RootComponent = BoxComp;

	//OnClickBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OnClickTrigger"));
	//OnClickBox->InitBoxExtent(FVector(20, 70, 90));
	//OnClickBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	//OnClickBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
	//OnClickBox->SetCollisionProfileName("ActiveObjects");
	//OnClickBox->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	//Door->SetCollisionObjectType(ECC_GameTraceChannel2);
	Door->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECollisionResponse::ECR_Block);
	//Door->SetCollisionProfileName("ActiveTrace");
	Door->SetupAttachment(RootComponent);

	Door->SetRelativeLocation(FVector(0.0f, 50.0f, -100.0f));
	Door->SetWorldScale3D(FVector(1.f));

}

// Called when the game starts or when spawned
void ADoors::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), BoxComp->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, 999, 0, 2);

}

// Called every frame
void ADoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		OpenDoor(DeltaTime);
	}

	if (Closing)
	{
		CloseDoor(DeltaTime);
	}


}

void ADoors::OpenDoor(float dt)
{
	DoorCurrentRotation = Door->RelativeRotation.Yaw;

	AddRotation = PosNeg * dt * 80;

	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Opening)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

void ADoors::CloseDoor(float dt)
{
	DoorCurrentRotation = Door->RelativeRotation.Yaw;

	if (DoorCurrentRotation > 0)
	{
		AddRotation = -dt * 80;
	}
	else
	{
		AddRotation = dt * 80;
	}

	if (FMath::IsNearlyEqual(DoorCurrentRotation, 0.0f, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Closing)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}


void ADoors::ToggleDoor(FVector ForwardVector)
{

	// alternatively you can grab the froward vector from the character inside theis function, remember to #include "GameFramework/Character.h" and #include "Camera/CameraComponent.h" at the top of the script
	// ACharacter* OurPlayerController = UGameplayStatics::GetPlayerCharacter(this, 0);

	// UCameraComponent* PlayerCamera = OurPlayerController->FindComponentByClass<UCameraComponent>();

	// FVector ForwardVector = PlayerCamera->GetForwardVector();

	// is the chacter in front or behind the door
	DotP = FVector::DotProduct(BoxComp->GetForwardVector(), ForwardVector);

	// get 1 or -1 from the the dot product
	PosNeg = FMath::Sign(DotP);

	// degree to clamp at
	MaxDegree = PosNeg * 90.0f;

	// toggle bools
	if (!bIsOpen) {
		bIsOpen = true;
		Closing = false;
		Opening = true;

	}
	else {
		Opening = false;
		bIsOpen = false;
		Closing = true;
	}
}