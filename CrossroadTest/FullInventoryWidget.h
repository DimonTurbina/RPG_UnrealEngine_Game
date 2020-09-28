// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CROSSROADTEST_API UFullInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void Show();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void Hide();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray <class APickUpIteminventory*> Inventory;
};
