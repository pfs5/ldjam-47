// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "FatNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AProjectile;
class UCameraShake;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AFatNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void MoveToTarget(AActor* target) override;
	virtual void AttackTarget(AActor* target) override;

private:
	void ShakeCamera();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _walkingCameraShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> _projectileClass;
};
/*----------------------------------------------------------------------------------------------------*/