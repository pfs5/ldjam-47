// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "CultistNPC.h"
#include "../Projectile/Projectile.h"
#include "Math/RotationMatrix.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
/*----------------------------------------------------------------------------------------------------*/
void ACultistNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APawn* targetPlayer = GetTargetPlayer())
	{
		TurnTowardsTarget(targetPlayer);
		AttackTarget(targetPlayer);
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ACultistNPC::MoveToTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ACultistNPC::OnArrivedToTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ACultistNPC::AttackTarget(AActor* target)
{
	if (_attackDelayTimer < _attackDelay)
	{
		return;
	}

	if (target == nullptr)
	{
		return;
	}
	
	FVector location = GetActorLocation();
	FVector direction = target->GetActorLocation() - location;
	FMatrix rotationMatrix = FRotationMatrix::MakeFromXY(direction, FVector::RightVector);
		
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectileClass, location, rotationMatrix.Rotator());

	_attackDelayTimer = 0.0f;
}
/*----------------------------------------------------------------------------------------------------*/
