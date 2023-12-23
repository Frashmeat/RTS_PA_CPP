// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PA_CPP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Input)
	int MoveSpeed = 10;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
private:
	
};
