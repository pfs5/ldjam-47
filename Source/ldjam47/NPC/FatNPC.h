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
	virtual void Tick(float DeltaTime) override;

	virtual void MoveToTarget(AActor* target) override;
	virtual void AttackTarget(AActor* target) override;

private:
	void ShakeCamera();
	void SpecialAttack(AActor* target);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _walkingCameraShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> _projectileClass;

private:
	UPROPERTY(EditAnywhere)
	float _specialAttackDelay = 20.0f;

	float _specialAttackDelayTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	int32 _numberOfSpecialAttackProjectiles = 12;
};
/*----------------------------------------------------------------------------------------------------*/