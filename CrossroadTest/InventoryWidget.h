// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CROSSROADTEST_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/*Holds a reference to the item texture*/

public:
	/*Sets the item texture*/
	UFUNCTION(BlueprintCallable, Category = UI)
	void SetItemTexture(class APickUpIteminventory* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;

	/*Tells the player the equip the represented item from this widget*/
	UFUNCTION(BlueprintCallable, Category = UI)
	void SetEquippedItem();
};
