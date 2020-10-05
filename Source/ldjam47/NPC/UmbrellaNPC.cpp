// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "UmbrellaNPC.h"
#include "../Projectile/Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AUmbrellaNPC::AUmbrellaNPC(): Super()
{
	_projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	_projectileMovementComponent->bAutoActivate = false;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_umbrellaAttackState == EUmbrellaAttackState::Open)
	{
		if (_raindropTimer > _delayBetweenRaindrop)
		{
			Rain();
		}

		_raindropTimer += DeltaTime;
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AUmbrellaNPC::BeginPlay()
{
	Super::BeginPlay();

	SetUmbrellaAttackState(EUmbrellaAttackState::Closed);
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::MoveToTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::OnArrivedToTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::AttackTarget(AActor* target)
{
	return;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::SetUmbrellaAttackState(EUmbrellaAttackState umbrellaAttackState)
{
	if (_umbrellaAttackState == umbrellaAttackState)
	{
		return;
	}

	_umbrellaAttackState = umbrellaAttackState;

	OnUmbrellaAttackStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EUmbrellaAttackState AUmbrellaNPC::GetUmbrellaAttackState() const
{
	return _umbrellaAttackState;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::OnUmbrellaAttackStateChanged()
{
	switch (_umbrellaAttackState)
	{
		case EUmbrellaAttackState::Closed:
		{
			StartBouncing();
			GetWorldTimerManager().SetTimer(_closedUmbrellaAttackStateDurationTimerHandle, this, &AUmbrellaNPC::StopBouncing, _closedUmbrellaAttackStateDuration);
			break;
		}
		case EUmbrellaAttackState::Open:
		{
			Rain();
			GetWorldTimerManager().SetTimer(_openUmbrellaAttackStateDurationTimerHandle, this, &AUmbrellaNPC::StopRaining, _openUmbrellaAttackStateDuration);
			break;
		}
		default:
		{
			break;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::StartBouncing()
{
	if (GetTargetPlayer() == nullptr || _projectileMovementComponent == nullptr)
	{
		return;
	}

	FVector targetLocation = GetTargetPlayer()->GetActorLocation();
	FVector location = GetActorLocation();
	FVector direction = targetLocation - location;

	_projectileMovementComponent->SetVelocityInLocalSpace((direction + _bounceOffset) * _launchSpeed);
	_projectileMovementComponent->Activate();
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::StopBouncing()
{
	if (_projectileMovementComponent == nullptr)
	{
		return;
	}

	_projectileMovementComponent->SetVelocityInLocalSpace(FVector::ZeroVector);

	SetUmbrellaAttackState(EUmbrellaAttackState::Open);
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::Rain()
{
	float randomXPosition = UKismetMathLibrary::RandomFloatInRange(-_rainMaxXPosition, _rainMaxXPosition);
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;
	FVector rainLocation = FVector(randomXPosition, 5.0f, _rainStartingZPosition);
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(_projectileClass, rainLocation, FRotator(-90.0f, 0.0f, 0.0f), spawnParameters);

	_raindropTimer = 0.0f;
}
/*----------------------------------------------------------------------------------------------------*/
void AUmbrellaNPC::StopRaining()
{
	SetUmbrellaAttackState(EUmbrellaAttackState::Closed);

	_raindropTimer = 0.0f;
}
/*----------------------------------------------------------------------------------------------------*/
