// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "OptionalCharacter.generated.h"

class UPAComponent;
UCLASS()
class RTS_PA_CPP_API AOptionalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOptionalCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UPAComponent* PAComponent;
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool isSelectedByPawn;

	UPROPERTY(BlueprintReadWrite)
	UTextRenderComponent* TextComponet; //组件无法再编辑栏中修改?

	void WhenSelected(bool isSelected);

	void MoveToLocation(FVector NewLocation);

	UPROPERTY(BlueprintReadOnly)
	FVector Goal;
private:
	
};
