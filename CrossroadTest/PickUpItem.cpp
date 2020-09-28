// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SphereVolume = CreateDefaultSubobject<USphereComponent>(TEXT("SphereVolume"));
	RootComponent = SphereVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	//Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	//SphereVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	

	Effects = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effects->SetupAttachment(RootComponent);

	PickupTexture = CreateDefaultSubobject<UTexture2D>(FName("ItemTexture"));

	DestroyTime = 3.0f;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereVolume->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnOverlapBegin);
	SphereVolume->OnComponentEndOverlap.AddDynamic(this, &APickUpItem::OnOverlapEnd);
}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin"));
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &APickUpItem::Foo, DestroyTime);
}

void APickUpItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End"));
}

void APickUpItem::Foo()
{
	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
		UE_LOG(LogTemp, Warning, TEXT("Sound"));
	}
	//Destroy();
}

void APickUpItem::SetGlowEffect(bool Status)
{
	Mesh->SetRenderCustomDepth(Status);
}