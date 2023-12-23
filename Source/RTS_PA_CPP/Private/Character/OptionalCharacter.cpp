// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/OptionalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Tasks/AITask_MoveTo.h"
#include "AIController.h"
#include "Tasks/AITask.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Component/PAComponent.h"



// Sets default values
AOptionalCharacter::AOptionalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PAComponent = CreateDefaultSubobject<UPAComponent>(TEXT("PAComponent"));
	

}

// Called when the game starts or when spawned
void AOptionalCharacter::BeginPlay()
{
	Super::BeginPlay();
	Goal = GetActorLocation();
	this->SpawnDefaultController();
}



// Called every frame
void AOptionalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOptionalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	


}

void AOptionalCharacter::WhenSelected(bool isSelected)
{
	isSelectedByPawn = isSelected;
	TextComponet->SetVisibility(isSelected);
}

void AOptionalCharacter::MoveToLocation(FVector NewLocation)
{
	
	AAIController* aiCon = Cast<AAIController>(this->GetController());
	if (IsValid(aiCon)) {
		Goal = NewLocation;
		Goal.Set(Goal.X, Goal.Y, GetActorLocation().Z);
		aiCon->MoveToLocation(NewLocation);
	}
	//¼ì²âÊÇ·ñµÖ´ï 
	PAComponent->BeginToCheck();
}



