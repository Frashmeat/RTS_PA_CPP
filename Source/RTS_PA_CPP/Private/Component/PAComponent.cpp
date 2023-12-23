// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PAComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/OptionalCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"
#include "GameFramework/PawnMovementComponent.h"
#include <iostream>
#include <Kismet/KismetSystemLibrary.h>
//#include <Kismet/KismetSystemLibrary.h>


// Sets default values for this component's properties
UPAComponent::UPAComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Owner = Cast<AOptionalCharacter>(GetOwner());
	if (!IsValid(Owner)) {
		UE_LOG(LogTemp, Warning, TEXT("PA cast fail"));
		return;
	}
	//��ȡ�������Size
	Goal = &Owner->Goal;
	UCapsuleComponent* CapsuleComponent = Owner->GetCapsuleComponent();
	float CapsuleX,CapsuleY;
	CapsuleComponent->GetScaledCapsuleSize(CapsuleX,CapsuleY);
	CapsuleSize.Set(CapsuleX, CapsuleY);



		
}
// Called every frame
void UPAComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FString string;
	if (IsTraceCheck)
		string.Append("True");
	else
		string.Append("False");
	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, string);
	if (IsTraceCheck) {
		TraceCheckForceToAvoidence();
	}

}

// Called when the game starts
void UPAComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Test
	//BeginToCheck();

}


bool UPAComponent::CheckHaveHinderByLine(const FVector& Start, const FVector& End,FHitResult& HitResult) const
{

	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	TArray< AActor* > ActorsToIgnore;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // ��� pawn
	bool IsHit = UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false,ActorsToIgnore,EDrawDebugTrace::Type::None, HitResult,true);
	return IsHit;
}

bool UPAComponent::CheckHaveHinderByCapsule(FHitResult& HitResult) const
{
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	TArray< AActor* > ActorsToIgnore;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//���pawn
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);//���pawn
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);//���pawn
	bool IsHit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, *Goal, *Goal, CapsuleSize.X, CapsuleSize.Y, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::Type::None, HitResult, true);
	return IsHit;
}


void UPAComponent::BeginToCheck()
{
	IsTraceCheck = true;
	Goal = &(Owner->Goal);
	if(!isPassiveAvoidence && !GetWorld()->GetTimerManager().IsTimerActive(CheckIsArriveTimer)) CurrentRememberTime = 0;
	GetWorld()->GetTimerManager().SetTimer(CheckIsArriveTimer, this, &UPAComponent::CheckIsArrive, TimerInRate, true);
}

void UPAComponent::CheckIsArrive()
{
	//UE_LOG(LogTemp, Warning, TEXT("Check is Arrive"));
	//�޶�������ʱ��
	GEngine->AddOnScreenDebugMessage(-1, TimerInRate, FColor::Blue, std::to_string(CurrentRememberTime).c_str());
	if (CurrentRememberTime >= MaxRememberTime) {

		//�͵�ֹͣ
		IsTraceCheck = false;
		//Owner->MoveToLocation(Owner->GetActorLocation());
		//���Timer
		*Goal = Owner->GetActorLocation();
		GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer);
		return;
	}
	//����ִ��
	CurrentRememberTime += TimerInRate;
	FVector CurrentLocation = Owner->GetActorLocation();
	//�ж��Ƿ���Ŀ�ĵظ���
	std::string str = std::to_string((*Goal - CurrentLocation).Length());
	GEngine->AddOnScreenDebugMessage(-1, TimerInRate, FColor::Blue, FString(str.c_str()));
	if ((*Goal - CurrentLocation).Length() <=40.0f){
		//�ڸ���ֹͣ�ƶ�
		//Owner->MoveToLocation(CurrentLoction);
		IsTraceCheck = false; 
		*Goal = Owner->GetActorLocation();
		GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer); //ִ�гɹ�
	}
	else {
		// ���ڸ��� 
		// �ж��Ƿ����谭
		FHitResult result;
		if (CheckHaveHinderByCapsule(result)) {
			//���谭���ж�
			IsTraceCheck = false;
			*Goal = Owner->GetActorLocation();
			GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer);

		}
		else {
			//���谭���ж�
			if (!isPassiveAvoidence) {
				//û���ڹ���ƶ��Ļ�
				Owner->MoveToLocation(*Goal);
			}
		}
	}
}

void UPAComponent::TraceCheckForceToAvoidence() const
{
	if (!IsTraceCheck) return;
	TraceCheckByDegree(0);
	TraceCheckByDegree(30);
	TraceCheckByDegree(60);
	TraceCheckByDegree(90);
	TraceCheckByDegree(120);
	TraceCheckByDegree(150);
	TraceCheckByDegree(180);
	TraceCheckByDegree(-30);
	TraceCheckByDegree(-60);
	TraceCheckByDegree(-90);
	TraceCheckByDegree(-120);
	TraceCheckByDegree(-150);
}

void UPAComponent::TraceCheckByDegree(float RotationDegree) const
{
	// ���㷽�� �� ����
	 
	FRotator degree;
	degree.Yaw = RotationDegree;
	FVector Direction =  degree.RotateVector(Owner->GetActorForwardVector());
	Direction *= (TraceCheckLength + CapsuleSize.X);

	FVector TraceStart = Owner->GetActorLocation();
	FVector TraceEnd = TraceStart + Direction;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue);

	/*FString string;
	string.Append(TraceStart.ToString());
	string += " ,";
	string.Append(TraceEnd.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, string);*/

	
	FHitResult HitResult;
	bool IsHit = CheckHaveHinderByLine(TraceStart, TraceEnd, HitResult);
	if (!IsHit) {
		return;
	}
	//��hit��pawn�ж��ķ���
	FVector HitDirection = HitResult.Normal * -1;
	FRotator Rotator;
	
	Rotator.Yaw = 10;
	HitDirection = Rotator.RotateVector(HitDirection);
	AOptionalCharacter* HitedCharacter = Cast<AOptionalCharacter>(HitResult.GetActor());
	if (!IsValid(HitedCharacter))
	{
		return;
	}
	UPAComponent * HitPA = HitedCharacter->GetComponentByClass<UPAComponent>();
	
	HitPA->PassiveMove(HitDirection);
}

void UPAComponent::PassiveMove(const FVector& Direction)
{
	isPassiveAvoidence = true;
	Owner->GetMovementComponent()->AddInputVector(Direction);
	BeginToCheck();
	FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(this, 0.5f, LatentActionInfo);
	isPassiveAvoidence = false;

}