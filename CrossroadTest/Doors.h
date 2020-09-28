// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doors.generated.h"

UCLASS()
class CROSSROADTEST_API ADoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OpenDoor(float dt);

	UFUNCTION(BlueprintCallable)
	virtual void CloseDoor(float dt);

	UFUNCTION(BlueprintCallable)
	void ToggleDoor(FVector ForwardVector);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	bool bIsOpen = false;

	bool Opening = false;
	bool Closing = false;

	float DotP = 0.f;
	float MaxDegree = 0.f;
	float AddRotation = 0.f;
	float PosNeg = 0.f;
	float DoorCurrentRotation = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door Settings")
	class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door Settings")
	class UBoxComponent* BoxComp;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door Settings")
	class UBoxComponent* OnClickBox;*/
};
