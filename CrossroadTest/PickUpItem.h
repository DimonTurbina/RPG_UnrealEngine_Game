// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

UCLASS()
class CROSSROADTEST_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | collision")
	class USphereComponent* SphereVolume;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Components")
	class UParticleSystemComponent* Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Texture")
	class UTexture2D* PickupTexture;

	UPROPERTY(EditAnywhere, Category = "Item | Name")
	FString ItemName;

	FTimerHandle Handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	float DestroyTime;

	void Foo();

	void SetGlowEffect(bool Status);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OverlapSound;

	FORCEINLINE class UTexture2D* GetPickUpTexture() { return PickupTexture; }
};
