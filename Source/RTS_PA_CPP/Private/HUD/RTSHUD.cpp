// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RTSHUD.h"
#include "Pawn/RTSFlowViewPawn.h"
#include "ClassViewerFilter.h"
#include "Character/OptionalCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void ARTSHUD::DrawRectangleToSelect(bool isTriggered)
{

	//获取当前鼠标位置
	GetOwningPlayerController()->GetMousePosition(StartMousePosition.X, StartMousePosition.Y);
	//绘制矩形,并选择其中的Character
	isLeftBottomHold = isTriggered;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), isLeftBottomHold);


}
void ARTSHUD::BeginPlay()
{
	APlayerController* playerController = GetOwningPlayerController();
	ARTSFlowViewPawn* flowViewPawn = Cast<ARTSFlowViewPawn>(playerController->GetPawn());
	flowViewPawn->SelectCharacterDelegate.AddUObject(this, &ARTSHUD::DrawRectangleToSelect);
	Owner = playerController->GetPawn();
}

void ARTSHUD::DrawHUD()
{
	//UE_LOG(LogTemp, Warning, TEXT("%d"), isLeftBottomHold);
	if (isLeftBottomHold) {
		FVector2D CurrentMousePosition;
		GetOwningPlayerController()->GetMousePosition(CurrentMousePosition.X,CurrentMousePosition.Y);
		//绘制矩形
		DrawRect(FLinearColor::Blue, StartMousePosition.X, StartMousePosition.Y , CurrentMousePosition.X - StartMousePosition.X, CurrentMousePosition.Y - StartMousePosition.Y);
		//选择矩形中的Character
		//清空之前的Character
		int len = OutActors.Num();
		for (int i = 0; i < len; i++)
		{
			if (OutActors[i] != nullptr)
			OutActors[i]->WhenSelected(false);
		}
		OutActors.Empty();
		//进行选择Character
		GetActorsInSelectionRectangle<AOptionalCharacter>(StartMousePosition,CurrentMousePosition,OutActors);
		len = OutActors.Num();
		for (int i = 0; i < len; i++)
		{
			if (OutActors[i] != nullptr)
			OutActors[i]->WhenSelected(true);
		}
		ARTSFlowViewPawn* flowViewPawn = Cast<ARTSFlowViewPawn>(Owner);
		flowViewPawn->OutActors = OutActors; //Ref
	}


}

