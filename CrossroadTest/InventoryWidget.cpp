// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "PickUpIteminventory.h"

void UInventoryWidget::SetEquippedItem()
{
}

void UInventoryWidget::SetItemTexture(APickUpIteminventory* Item)
{
	if (Item) { 
		ItemTexture = Item->GetPickUpTexture();
		if (Item->GetPickUpTexture() != nullptr)
			UE_LOG(LogTemp, Warning, TEXT("TextureSetted"))
	}
	else ItemTexture = nullptr;
}
	