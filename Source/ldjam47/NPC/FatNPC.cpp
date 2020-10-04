// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "FatNPC.h"
#include "../Misc/MovablePawnsShared.h"
#include "../Player/GentlemanPlayerController.h"
#include "../Projectile/Projectile.h"
#include "Camera/CameraShake.h"
#include "UObject/NoExportTypes.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AFatNPC::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);

	if (GetNPCState() == EMovablePawnState::Walking)
	{
		ShakeCamera();
	}

	AttackTarget(target);
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AFatNPC::AttackTarget(AActor* target)
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

	OnNPCAttackedTarget.Broadcast(target);
}
/*----------------------------------------------------------------------------------------------------*/
void AFatNPC::ShakeCamera()
{
	if (_walkingCameraShake == nullptr)
	{
		return;
	}

	if (AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		playerController->PlayerCameraManager->PlayCameraShake(_walkingCameraShake, 1.0f);
	}
}
/*----------------------------------------------------------------------------------------------------*/
