// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IsometricalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CROSSROADTEST_API AIsometricalPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AIsometricalPlayerController();

	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CharacterAction();

	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ADoors* CurrentDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ACandle* CurrentCandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AIsometricalMainCharacter* CurrentPawn;
};
