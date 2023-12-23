// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Character/OptionalCharacter.h"
#include "Camera/CameraComponent.h"
#include "Tasks/AITask_MoveTo.h"

#include "RTSFlowViewPawn.generated.h"
//DECLARE_MULTICAST_DELEGATE(FSelectCharacterDelegate,bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FSelectCharacterDelegate, bool);

UCLASS()
class RTS_PA_CPP_API ARTSFlowViewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSFlowViewPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FlowViewMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveViewAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ResizeViewAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EnableRotateViewAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateViewAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectCharacterAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TriggerToMoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "View")
	UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere,Category = "View")
	FVector2D LimitedHighValue = {2000,500};

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Input)
	int MoveSpeed = 50;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Input)
	int ResizeAndRotateSpeed = 50;

	UPROPERTY(EditAnyWhere,Category = "MakeLocations")
	FVector standard = {100,100,10};



	FSelectCharacterDelegate SelectCharacterDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveView(const FInputActionValue& Value);
	void ResizeView(const FInputActionValue& Value);
	void EnableRotateView(const FInputActionValue& Value);
	void RotateView(const FInputActionValue& Value);
	void SelectCharacter(const FInputActionValue& Value);
	void UnSelectCharacter(const FInputActionValue& Value);
	void TriggerToMove(const FInputActionValue& Value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TArray<AOptionalCharacter*> OutActors;
private:
	//viewPort size
	int32 ViewPort_X;
	int32 ViewPort_Y;
	//Current Mouse Position
	FVector2D CurrentMousePosition;
	bool isEnableRotateView = false;
	//
	void MoveViewWhenMouseOverScreenSize();
	
};
