// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "CultistNPC.h"
#include "../Projectile/Projectile.h"
#include "Components/ArrowComponent.h"
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
void ACultistNPC::OnArrivedToTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
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

	if (UArrowComponent* shootProjectileDirection = GetShootProjectileDirection())
	{
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectileClass, shootProjectileDirection->GetComponentTransform());
	}

	_attackDelayTimer = 0.0f;

	OnNPCAttackedTarget.Broadcast(target);
}
/*----------------------------------------------------------------------------------------------------*/
