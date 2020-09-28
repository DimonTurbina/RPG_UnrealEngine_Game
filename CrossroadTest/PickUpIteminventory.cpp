// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpIteminventory.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void APickUpIteminventory::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	AMainCharacter* Char;
	Char = Cast<AMainCharacter>(OtherActor);
	if (Char) {
		int32 AvailableSlot = Char->Inventory.Find(nullptr);
		if (AvailableSlot != INDEX_NONE) {
			Char->Inventory[AvailableSlot] = this;
			Destroy();
		}
		else GLog->Log("You can't carry any more items!");
	}
	AMainPlayerController* Con = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(Char, 0));
	Con->HandleInventoryInput();
}

void APickUpIteminventory::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
