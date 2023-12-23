// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/RTSFlowViewPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "MathUtil.h"
#include "Tasks/AITask_MoveTo.h"
#include "AIController.h"
#include "Tasks/AITask.h"

// Sets default values
ARTSFlowViewPawn::ARTSFlowViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARTSFlowViewPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(FlowViewMappingContext, 0);
		}
	} 
}



// Called every frame
void ARTSFlowViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//MoveViewWhenMouseOverScreenSize();
}

// Called to bind functionality to input
void ARTSFlowViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponet = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponet->BindAction(MoveViewAction, ETriggerEvent::Triggered, this, &ARTSFlowViewPawn::MoveView);
		EnhancedInputComponet->BindAction(ResizeViewAction, ETriggerEvent::Triggered, this, &ARTSFlowViewPawn::ResizeView);
		EnhancedInputComponet->BindAction(EnableRotateViewAction, ETriggerEvent::Triggered, this, &ARTSFlowViewPawn::EnableRotateView);
		EnhancedInputComponet->BindAction(SelectCharacterAction, ETriggerEvent::Started, this, &ARTSFlowViewPawn::SelectCharacter);
		EnhancedInputComponet->BindAction(SelectCharacterAction, ETriggerEvent::Completed, this, &ARTSFlowViewPawn::UnSelectCharacter);
		EnhancedInputComponet->BindAction(TriggerToMoveAction, ETriggerEvent::Triggered, this, &ARTSFlowViewPawn::TriggerToMove);
		EnhancedInputComponet->BindAction(RotateViewAction, ETriggerEvent::Triggered, this, &ARTSFlowViewPawn::RotateView);

	}
}


void ARTSFlowViewPawn::MoveView(const FInputActionValue& Value)
{
	FVector2D value2D = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		FVector forward = GetActorForwardVector();
		FVector right = GetActorRightVector();
		FVector RightLocation = GetActorLocation();
		FVector newLocaltion = (forward * value2D.X * MoveSpeed + right * value2D.Y * MoveSpeed) + RightLocation;
		SetActorLocation(newLocaltion);
		//UE_LOG(LogTemp, Warning, TEXT("ADD"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("x = %f, y = %f,z = %f"), value3D.X, value3D.Y, value3D.Z);
}
void ARTSFlowViewPawn::ResizeView(const FInputActionValue& Value)
{
	//修改Z轴值
	if (IsValid(MainCamera)) {
		float ResizeValue = Value.Get<float>();
		FVector zValue(0, 0, 1);
		FVector newLocation = MainCamera->GetRelativeLocation() + (zValue * ResizeAndRotateSpeed * ResizeValue * -1);
		if (newLocation.Z <= LimitedHighValue.X && newLocation.Z >= LimitedHighValue.Y) {
			MainCamera->SetRelativeLocation(newLocation);
		}
	}

}
void ARTSFlowViewPawn::EnableRotateView(const FInputActionValue& Value)
{
	isEnableRotateView = isEnableRotateView ? false : true;
}
void ARTSFlowViewPawn::RotateView(const FInputActionValue& Value)
{
	//旋转yaw轴
	if (isEnableRotateView) {
		float x = Value.Get<float>();
		FRotator CurrentRotation = MainCamera->GetRelativeRotation();
		CurrentRotation += {0, 0, x * 4};
		MainCamera->SetRelativeRotation(CurrentRotation);
	}

}



void ARTSFlowViewPawn::SelectCharacter(const FInputActionValue& Value)
{
	SelectCharacterDelegate.Broadcast(true);
}
void ARTSFlowViewPawn::UnSelectCharacter(const FInputActionValue& Value)
{
	SelectCharacterDelegate.Broadcast(false);
}
void ARTSFlowViewPawn::TriggerToMove(const FInputActionValue& Value)
{

	//UE_LOG(LogTemp, Warning, TEXT("TriggerToMove"));
	//获取鼠标点击的世界位置
	APlayerController* playerController = Cast<APlayerController>(Controller);
	FHitResult Result;
	playerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, Result);
	//计算新位置
	FVector location = Result.Location;
	//location.Set(location.X, location.Y, 0);
	//UE_LOG(LogTemp, Warning, TEXT("x = %f, y = %f, z = %f"), location.X,location.Y,location.Z);
	TArray<FVector> newLocations;
	int len = OutActors.Num();
	if (len != 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d"), len);
		int Column = sqrt(OutActors.Num());
		int Row = len / Column;
		//FVector standard(100, 100, 0);
		for (int i = 0; i < len; i++)
		{
			FVector newLocation;
			int RowIndex = i / Column;
			int ColumnIndex = i % Column;
			newLocation.Set(standard.X * (RowIndex - Row / 2), standard.Y * (ColumnIndex - Column / 2), standard.Z);
			newLocation += location;
			newLocations.Add(newLocation);
			//UE_LOG(LogTemp, Warning, TEXT("x = %f, y = %f, z = %f"), newLocation.X, newLocation.Y, newLocation.Z);
		}
		//移动到新位置
		//UE_LOG(LogTemp, Warning, TEXT("Move"));
		for (int i = 0; i < OutActors.Num(); i++)
		{
			if (OutActors[i] != nullptr) {

				OutActors[i]->MoveToLocation(newLocations[i]);
			}
		}
	}
	
	

	 
}
void ARTSFlowViewPawn::MoveViewWhenMouseOverScreenSize()
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	playerController->GetViewportSize(ViewPort_X, ViewPort_Y);
	if (CurrentMousePosition.X <= 0 || CurrentMousePosition.X >= ViewPort_X || CurrentMousePosition.Y <= 0 || CurrentMousePosition.Y >= ViewPort_Y) {
		bool isForward = CurrentMousePosition.Y <= 0;
		bool isBack = CurrentMousePosition.Y >= ViewPort_Y;
		bool isRight = CurrentMousePosition.X >= ViewPort_X;
		bool isLeft = CurrentMousePosition.X <= 0;
		if (isForward) {
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MoveSpeed);

		}
		if (isBack) {
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * MoveSpeed*-1);

		}
		if (isRight) {
			SetActorLocation(GetActorLocation() + GetActorRightVector() * MoveSpeed);

		}
		if (isLeft) {
			SetActorLocation(GetActorLocation() + GetActorRightVector() * MoveSpeed*-1);
			
		}
	}
}