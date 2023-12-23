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
	//获取胶囊体的Size
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
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // 检测 pawn
	bool IsHit = UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false,ActorsToIgnore,EDrawDebugTrace::Type::None, HitResult,true);
	return IsHit;
}

bool UPAComponent::CheckHaveHinderByCapsule(FHitResult& HitResult) const
{
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	TArray< AActor* > ActorsToIgnore;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);//检测pawn
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);//检测pawn
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);//检测pawn
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
	//限定最大记忆时间
	GEngine->AddOnScreenDebugMessage(-1, TimerInRate, FColor::Blue, std::to_string(CurrentRememberTime).c_str());
	if (CurrentRememberTime >= MaxRememberTime) {

		//就地停止
		IsTraceCheck = false;
		//Owner->MoveToLocation(Owner->GetActorLocation());
		//清除Timer
		*Goal = Owner->GetActorLocation();
		GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer);
		return;
	}
	//继续执行
	CurrentRememberTime += TimerInRate;
	FVector CurrentLocation = Owner->GetActorLocation();
	//判断是否在目的地附近
	std::string str = std::to_string((*Goal - CurrentLocation).Length());
	GEngine->AddOnScreenDebugMessage(-1, TimerInRate, FColor::Blue, FString(str.c_str()));
	if ((*Goal - CurrentLocation).Length() <=40.0f){
		//在附近停止移动
		//Owner->MoveToLocation(CurrentLoction);
		IsTraceCheck = false; 
		*Goal = Owner->GetActorLocation();
		GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer); //执行成功
	}
	else {
		// 不在附近 
		// 判断是否有阻碍
		FHitResult result;
		if (CheckHaveHinderByCapsule(result)) {
			//有阻碍则不行动
			IsTraceCheck = false;
			*Goal = Owner->GetActorLocation();
			GetWorld()->GetTimerManager().ClearTimer(CheckIsArriveTimer);

		}
		else {
			//无阻碍则行动
			if (!isPassiveAvoidence) {
				//没有在规避移动的话
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
	// 计算方向 和 长度
	 
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
	//被hit的pawn行动的方向
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