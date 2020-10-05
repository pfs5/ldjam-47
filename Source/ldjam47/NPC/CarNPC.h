// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "CarNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ACarNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void MoveToTarget(AActor* target) override;
	virtual void OnArrivedToTarget(AActor* target);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

private:
	UPROPERTY(EditAnywhere)
	float _distanceBeforeTurningAround = 100.0f;

	FVector _startingLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float _arrivingRadius = 2.0f;
};
/*----------------------------------------------------------------------------------------------------*/