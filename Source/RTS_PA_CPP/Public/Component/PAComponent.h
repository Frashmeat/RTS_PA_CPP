// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAComponent.generated.h"

class AOptionalCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PA_CPP_API UPAComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPAComponent();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Timer")
	float TimerInRate = 0.2f;


	//������ʱ��
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Remember")
	float MaxRememberTime = 5.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FTimerHandle CheckIsArriveTimer;
	//ӵ�д�Component��Character
	AOptionalCharacter* Owner;
	//Character ��Ҫ�����λ��
	FVector * Goal;
	//��ǰ����ʱ��
	float CurrentRememberTime = 0.0f;
	//��ǰCapsule��size
	FVector2D CapsuleSize;
	//ͨ�������
	//FHitResult result;
	//�Ƿ��ڹ��
	bool isPassiveAvoidence = false;
	//Trace Check �ĳ���
	float TraceCheckLength = 15;
	//�Ƿ��� Trace Check 
	bool CheckHaveHinderByLine(const FVector& Start, const FVector& End,FHitResult& HitResult) const;
	bool CheckHaveHinderByCapsule(FHitResult& HitResult) const;
	void CheckIsArrive();
	void TraceCheckForceToAvoidence() const;
	void TraceCheckByDegree(float RotationDegree) const;
public:	
	// Called every frame
	bool IsTraceCheck = false;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginToCheck();
	void PassiveMove(const FVector& Direction);
};
