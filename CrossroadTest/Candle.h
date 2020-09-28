// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Candle.generated.h"

UCLASS()
class CROSSROADTEST_API ACandle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeshSettings")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeshSettings")
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeshSettings")
	class UParticleSystemComponent* Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshSettings")
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshSettings")
	FVector SpawnLocation;

	//Set this value as true if you want the light to be always
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LogicSettings")
	bool bConstLightning;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LogicSettings")
	bool bIsShine;

	UFUNCTION(BlueprintCallable)
	void StartShine();

	UFUNCTION(BlueprintCallable)
	void StopShine();

	UFUNCTION(BlueprintCallable)
	void ToggleCandle();
};
