// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "FullInventoryWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UFullInventoryWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	bIsInventoryOpen = false;
}

void AMainPlayerController::HandleInventoryInput()
{
	AMainCharacter* Char = Cast<AMainCharacter>(GetPawn());
	if (HUDOverlay)
	{
		//if (bIsInventoryOpen)
		//{
			//Mark the inventory as closed
			//bIsInventoryOpen = false;

			//Remove it from the viewport
			//HUDOverlay->RemoveFromViewport();
		//}
		//else
		//{
			//Mark the inventory as open
			bIsInventoryOpen = true;

			//Re-populate the ItemsArray
			HUDOverlay->Inventory = Char->GetCharInventory();

			//Show the inventory
			HUDOverlay->Show();
		//}

	}
}

//void AMainPlayerController::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//
//	if (InventoryWidgetBP)
//	{
//		//Create the Inventory Widget based on the Blueprint reference we will input from within the Editor
//		InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
//
//	}
//
//	//Initial value
//	bIsInventoryOpen = false;
//}

