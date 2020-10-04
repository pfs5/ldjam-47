// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "CultistNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AProjectile;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ACultistNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void MoveToTarget(AActor* target) override;
	virtual void OnArrivedToTarget(AActor* target) override;
	virtual void AttackTarget(AActor* target) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> _projectileClass;
};
/*----------------------------------------------------------------------------------------------------*/