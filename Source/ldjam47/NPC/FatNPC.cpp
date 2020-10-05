// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "FatNPC.h"
#include "../Misc/MovablePawnsShared.h"
#include "../Player/GentlemanPlayerController.h"
#include "../Projectile/Projectile.h"
#include "Camera/CameraShake.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "UObject/NoExportTypes.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AFatNPC::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	_specialAttackDelayTimer += deltaTime;
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AFatNPC::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);

	if (GetNPCState() == EMovablePawnState::Walking)
	{
		ShakeCamera();
	}

	if (_attackDelayTimer >= _attackDelay)
	{
		AttackTarget(target);
	}
	else if (_specialAttackDelayTimer >= _specialAttackDelay)
	{
		SpecialAttack(target);
	}
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

	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectileClass, location, rotationMatrix.Rotator(), spawnParameters);
	if (UPaperFlipbookComponent* flipbook = projectile->GetFlipbook())
	{
		flipbook->SetFlipbook(GetRandomFoodFlipbook());
	}

	_attackDelayTimer = 0.0f;
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
void AFatNPC::SpecialAttack(AActor* target)
{
	if (_specialAttackDelayTimer < _specialAttackDelay)
	{
		return;
	}

	if (target == nullptr)
	{
		return;
	}

	float angle = 360.0f / _numberOfSpecialAttackProjectiles;
	for (int32 i = 0; i < _numberOfSpecialAttackProjectiles; ++i)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectileClass, GetActorLocation(), FRotator(angle * i, 0.0f, 0.0f), spawnParameters);
		if (UPaperFlipbookComponent* flipbook = projectile->GetFlipbook())
		{
			flipbook->SetFlipbook(GetRandomFoodFlipbook());
		}
	}

	_specialAttackDelayTimer = 0.0f;
	_attackDelayTimer = 0.0f;
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbook* AFatNPC::GetRandomFoodFlipbook()
{
	int32 random = UKismetMathLibrary::RandomIntegerInRange(1, 3);

	switch (random)
	{
		case 1:
		{
			return _friesFlipbook;
		}
		case 2:
		{
			return _hamburgerFlipbook;
		}
		case 3:
		{
			return _shakeFlipbook;
		}
	default:
		break;
	}

	return _hamburgerFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
