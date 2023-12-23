// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Character/OptionalCharacter.h"
#include "RTSHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PA_CPP_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

public:
	void DrawRectangleToSelect(bool isLeftBottomPressed);
protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	APawn* Owner;
	FVector2D StartMousePosition;
	bool isLeftBottomHold = false;
	TArray<AOptionalCharacter*> OutActors;
};
