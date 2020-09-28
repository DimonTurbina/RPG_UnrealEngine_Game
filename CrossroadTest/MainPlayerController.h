// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CROSSROADTEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    TSubclassOf<class UFullInventoryWidget> HUDOverlayAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    UFullInventoryWidget* HUDOverlay;

    void HandleInventoryInput();
protected:
    virtual void BeginPlay() override;
private:
    bool bIsInventoryOpen;

};
