// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "UmbrellaNPC.h"
#include "../Audio/AudioManager.h"
#include "../Player/GentlemanPlayer.h"
#include "../Player/GentlemanPlayerController.h"
#include "../Projectile/Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/WorldSettings.h"
#include "../GentlemansWorldSettings.h"
/*----------------------------------------------------------------------------------------------------*/
AUmbrellaNPC::AUmbrellaNPC(): Super()
{
	_projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	_projectileMovementComponent->bAutoActivate = false;

	_openUmbrellaFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("OpenUmbrellaFlipbookComponent"));
	_openUmbrellaFlipbookComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_openUmbrellaFlipbookComponent->Stop();
	_openUmbrellaFlipbookComponent->SetLooping(false);
	_openUmbrellaFlipbookComponent->SetHiddenInGame(true);
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

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AUmbrellaNPC::OnOverlapBegin);
	}

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
	UPaperFlipbookComponent* flipbook = GetSprite();
	if (flipbook == nullptr)
	{
		return;
	}

	if (_openUmbrellaFlipbookComponent == nullptr)
	{
		return;
	}

	switch (_umbrellaAttackState)
	{
		case EUmbrellaAttackState::Closed:
		{
			if (AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
			{
				if (AAudioManager* audioManager = ws->GetAudioManager())
				{
					audioManager->StopRainSound();
				}
			}

			_openUmbrellaFlipbookComponent->SetHiddenInGame(true);
			flipbook->SetHiddenInGame(false);
			StartBouncing();
			GetWorldTimerManager().SetTimer(_closedUmbrellaAttackStateDurationTimerHandle, this, &AUmbrellaNPC::StopBouncing, _closedUmbrellaAttackStateDuration);
			break;
		}
		case EUmbrellaAttackState::Open:
		{
			if (AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
			{
				if (AAudioManager* audioManager = ws->GetAudioManager())
				{
					audioManager->StartRainSound();
				}
			}

			flipbook->SetHiddenInGame(true);
			_openUmbrellaFlipbookComponent->SetHiddenInGame(false);
			_openUmbrellaFlipbookComponent->PlayFromStart();
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
void AUmbrellaNPC::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr && otherActor == this && otherComp == nullptr)
	{
		return;
	}

	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(otherActor))
	{
		if (AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(player->GetController()))
		{
			playerController->ApplyDamage(GetNPCDirection(), GetAttackDamage());
			if (ANPC* owner = Cast<ANPC>(GetOwner()))
			{
				owner->OnNPCAttackedTarget.Broadcast(player);
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
