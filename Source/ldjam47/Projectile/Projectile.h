// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "Projectile.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UPaperFlipbookComponent;
class UProjectileMovementComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

private:
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* _projectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UPaperFlipbookComponent* _flipbookComponent;
};
/*----------------------------------------------------------------------------------------------------*/