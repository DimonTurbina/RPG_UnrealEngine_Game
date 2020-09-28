// Fill out your copyright notice in the Description page of Project Settings.


#include "Candle.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACandle::ACandle()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));

	RootComponent = BoxComp;
	BoxComp->InitBoxExtent(FVector(60, 60, 100));
	BoxComp->SetCollisionProfileName("Trigger");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	Effects = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LightningEffect"));
	Effects->SetupAttachment(RootComponent);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bConstLightning = false;
}

// Called when the game starts or when spawned
void ACandle::BeginPlay()
{
	Super::BeginPlay();
	if (bIsShine)
	{
		Effects->SetVisibility(true);
	}
	else
	{
		Effects->SetVisibility(false);
	}
	DrawDebugBox(GetWorld(), GetActorLocation(), BoxComp->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, 999, 0, 2);
}

// Called every frame
void ACandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACandle::StartShine()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Effects->GetComponentLocation(), FRotator(0.f), true);
	Effects->SetVisibility(true);
}

void ACandle::StopShine()
{
	Effects->SetVisibility(false);
}

void ACandle::ToggleCandle()
{
	bIsShine = !bIsShine;
	if (bIsShine)
	{
		StartShine();
	}
	else
	{
		StopShine();
	}
}

