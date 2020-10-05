// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "NPC.h"
#include "../Player/GentlemanPlayer.h"
#include "../Player/GentlemanPlayerController.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
#include "../GentlemansWorldSettings.h"
#include "../Levels/LevelManager.h"
/*----------------------------------------------------------------------------------------------------*/
ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	_attackFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackFlipbook"));
	_attackFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_attackFlipbook->Stop();
	_attackFlipbook->SetLooping(false);
	_attackFlipbook->SetHiddenInGame(true);

	_attackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	_attackHitBox->SetupAttachment(_attackFlipbook);

	_deathFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DeathFlipbookComponent"));
	_deathFlipbookComponent->SetupAttachment(RootComponent);
	_deathFlipbookComponent->Stop();
	_deathFlipbookComponent->SetLooping(false);
	_deathFlipbookComponent->SetHiddenInGame(true);

	if (UCharacterMovementComponent* characterMovement = GetCharacterMovement())
	{
		characterMovement->bRunPhysicsWithNoController = true;
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SnapLocation()
{
	const float unitsPerPixel = 1.f / _pixelsPerUnit;

	FVector location = GetActorLocation();
	location.X = FMath::GridSnap(location.X, unitsPerPixel);
	location.Z = FMath::GridSnap(location.Z, unitsPerPixel);
	SetActorLocation(location);
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetLastMovementVector(FVector movementVector)
{
	if (_lastMovementVector == movementVector)
	{
		return;
	}

	FVector oldMovementVector = _lastMovementVector;
	_lastMovementVector = movementVector;

	OnLastMovementVectorChanged(oldMovementVector);
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnLastMovementVectorChanged(FVector oldMovementVector)
{
	if (_lastMovementVector.X < 0.0f)
	{
		SetLastMovementInput(EMovementInput::Left);
	}
	else if (_lastMovementVector.X > 0.0f)
	{
		SetLastMovementInput(EMovementInput::Right);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetLastMovementInput(EMovementInput movementInput)
{
	if (_lastMovementInput == movementInput)
	{
		return;
	}

	_lastMovementInput = movementInput;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::UpdateFlipbook()
{
	if (_lastMovementInput == EMovementInput::None)
	{
		return;
	}

	switch (_lastMovementInput)
	{
		case EMovementInput::Up:
		{
			SetNPCDirection(EMovablePawnDirection::Back);
			break;
		}
		case EMovementInput::Down:
		{
			SetNPCDirection(EMovablePawnDirection::Front);
			break;
		}
		case EMovementInput::Left:
		{
			SetNPCDirection(EMovablePawnDirection::Left);
			break;
		}
		case EMovementInput::Right:
		{
			SetNPCDirection(EMovablePawnDirection::Right);
			break;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnDamageTaken()
{
	_hitEffectValue = _hitEffectStrength;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::ApplyKnockback(EMovablePawnDirection direction)
{
	FVector newLocation = GetActorLocation();
	if (direction == EMovablePawnDirection::Left)
	{
		newLocation.X = newLocation.X - _knockbackOnHit;
		SetActorLocation(newLocation);
	}
	else if (direction == EMovablePawnDirection::Right)
	{
		newLocation.X = newLocation.X + _knockbackOnHit;
		SetActorLocation(newLocation);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::TickHitEffects(float deltaTime)
{
	if (_spriteMaterialInstance != nullptr)
	{
		_spriteMaterialInstance->SetScalarParameterValue(TEXT("AdditiveBrightness"), _hitEffectValue);
	}

	_hitEffectValue = FMath::Max(0.f, _hitEffectValue - _hitEffectDecaySpeed * deltaTime);
}
/*----------------------------------------------------------------------------------------------------*/
APawn* ANPC::FindPlayerPawn() const
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (pc == nullptr)
	{
		return nullptr;
	}

	return pc->GetPawn();
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnDeathAnimationFinishedPlaying()
{
	Destroy();
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* ANPC::GetAttackFlipbook() const
{
	return _attackFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
APawn* ANPC::GetTargetPlayer() const
{
	return _targetPlayer.Get();
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ANPC::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	TickHitEffects(deltaTime);

	if (_targetPlayer != nullptr)
	{
		MoveToTarget(_targetPlayer.Get());
	}

	if (_npcState != EMovablePawnState::Attacking)
	{
		float currentAccelerationX = GetCharacterMovement()->GetCurrentAcceleration().X;
		float currentAccelerationZ = GetCharacterMovement()->GetCurrentAcceleration().Z;

		if (currentAccelerationX != 0.f || currentAccelerationZ != 0)
		{
			SetNPCState(EMovablePawnState::Walking);
		}
		else if (currentAccelerationX == 0.f && currentAccelerationZ == 0)
		{
			SetNPCState(EMovablePawnState::Idle);
		}
	}

	_attackDelayTimer += deltaTime;

	//SnapLocation();
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetNPCState(EMovablePawnState npcState)
{
	if (_npcState == npcState)
	{
		return;
	}

	_npcState = npcState;

	OnNPCStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnState ANPC::GetNPCState() const
{
	return _npcState;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnNPCStateChanged()
{
	SetFlipbook(_npcState, _npcDirection);
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetNPCDirection(EMovablePawnDirection npcDirection)
{
	if (_npcDirection == npcDirection)
	{
		return;
	}

	_npcDirection = npcDirection;

	OnNPCDirectionChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnDirection ANPC::GetNPCDirection() const
{
	return _npcDirection;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnNPCDirectionChanged()
{
	SetFlipbook(_npcState, _npcDirection);
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnDirection ANPC::GetStartingNPCDirection() const
{
	return _npcStartingDirection;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetFlipbook(EMovablePawnState npcState, EMovablePawnDirection npcDirection)
{
	UPaperFlipbookComponent* flipbook = GetSprite();
	if (flipbook == nullptr)
	{
		return;
	}

	if (_npcState == EMovablePawnState::Idle)
	{
		switch (_npcDirection)
		{
			case EMovablePawnDirection::Right:
			{
				if (_idleRightFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleRightFlipbook);
					flipbook->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
					//flipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Left:
			{
				if (_idleLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleLeftFlipbook);
					flipbook->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
					//flipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Back:
			{
				if (_idleUpFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleUpFlipbook);
				}
				break;
			}
			case EMovablePawnDirection::Front:
			{
				if (_idleDownFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleDownFlipbook);
				}
				break;
			}
		}
	}
	else if (_npcState == EMovablePawnState::Walking)
	{
		switch (_npcDirection)
		{
			case EMovablePawnDirection::Right:
			{
				if (_walkRightFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkRightFlipbook);
					flipbook->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
					//flipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Left:
			{
				if (_walkLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkLeftFlipbook);
					flipbook->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
					//flipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Back:
			{
				if (_walkUpFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkUpFlipbook);
				}
				break;
			}
			case EMovablePawnDirection::Front:
			{
				if (_walkDownFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkDownFlipbook);
				}
				break;
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::Reset()
{
	SetLastMovementVector(GetActorLocation());
	SetNPCDirection(_npcStartingDirection);
	SetActorRotation(FRotator::ZeroRotator);
	SetNPCState(EMovablePawnState::Idle);
}
/*----------------------------------------------------------------------------------------------------*/
/*virtual*/
void ANPC::MoveToTarget(AActor* target)
{
	if (target == nullptr)
	{
		return;
	}
	
	APaperCharacter* targetCharacter = Cast<APaperCharacter>(target);
	if (targetCharacter == nullptr)
	{
		return;
	}

	FVector npcLocation = GetActorLocation();
	float npcCollisionRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();

	FVector targetLocation = targetCharacter->GetActorLocation();
	float targetCollisionRadius = targetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	
	FVector direction = targetLocation - npcLocation;

	float distance = direction.Size();
	if (distance < npcCollisionRadius + targetCollisionRadius + _targetRadius)
	{
		OnArrivedToTarget(target);
		return;
	}

	AddMovementInput(direction);
	SetLastMovementVector(direction);
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
/*virtual*/
void ANPC::OnArrivedToTarget(AActor* target)
{
	if (target == nullptr)
	{
		return;
	}

	AttackTarget(target);
}
/*----------------------------------------------------------------------------------------------------*/
/*virtual*/
void ANPC::AttackTarget(AActor* target)
{
	if (_attackDelayTimer < _attackDelay)
	{
		return;
	}

	if (target == nullptr)
	{
		return;
	}

	if (AGentlemanPlayer* targetCharacter = Cast<AGentlemanPlayer>(target))
	{
		if (AGentlemanPlayerController* targetController = Cast<AGentlemanPlayerController>(targetCharacter->GetController()))
		{
			targetController->ApplyDamage(_npcDirection, _attackDamage);
		}
	}

	_attackDelayTimer = 0.0f;

	OnNPCAttackedTarget.Broadcast(target);
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::ApplyDamage(EMovablePawnDirection direction)
{
	if (_health <= 0.0f)
	{
		return;
	}

	SetHealth(_health - _damageOnHit);
	
	if (_applyKnockback)
	{
		ApplyKnockback(direction);
	}

	OnDamageTaken();
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::SetHealth(float health)
{
	if (_health == health)
	{
		return;
	}

	_health = health;

	OnHealthChanged();
}
/*----------------------------------------------------------------------------------------------------*/
float ANPC::GetHealth() const
{
	return _health;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::OnHealthChanged()
{
	if (_health <= 0.0f)
	{
		if (_deathFlipbookComponent != nullptr && GetSprite() != nullptr)
		{
			GetSprite()->SetHiddenInGame(true);
			_deathFlipbookComponent->SetHiddenInGame(false);
			_deathFlipbookComponent->PlayFromStart();
		}

		if (AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
		{
			if (ALevelManager* levelManager = ws->GetLevelManager())
			{
				levelManager->Event_OnNPCDied();
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
float ANPC::GetDamageOnHit() const
{
	return _damageOnHit;
}
/*----------------------------------------------------------------------------------------------------*/
float ANPC::GetAttackDamage() const
{
	return _attackDamage;
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::TurnTowardsTarget(AActor* target)
{
	FVector targetLocation = target->GetActorLocation();
	FVector location = GetActorLocation();
	FVector direction = targetLocation - location;

	if (direction.X > 0.f)
	{
		SetNPCDirection(EMovablePawnDirection::Right);
	}
	else if (direction.X < 0.f)
	{
		SetNPCDirection(EMovablePawnDirection::Left);
	}
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnDirection ANPC::GetOppositeDirection(EMovablePawnDirection direction)
{
	switch (direction)
	{
		case EMovablePawnDirection::Back:
		{
			return EMovablePawnDirection::Front;
		}
		case EMovablePawnDirection::Front:
		{
			return EMovablePawnDirection::Back;
		}
		case EMovablePawnDirection::Left:
		{
			return EMovablePawnDirection::Right;
		}
		case EMovablePawnDirection::Right:
		{
			return EMovablePawnDirection::Left;
		}
		default:
		{
			break;
		}
	}

	return EMovablePawnDirection::Left;
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	_targetPlayer = FindPlayerPawn();

	Reset();

	if (_spriteMaterial != nullptr)
	{
		_spriteMaterialInstance = UMaterialInstanceDynamic::Create(_spriteMaterial, this);
		if (UPaperFlipbookComponent* flipbookComp = GetSprite())
		{
			for (int32 i = 0; i < flipbookComp->GetNumMaterials(); ++i)
			{
				flipbookComp->SetMaterial(i, _spriteMaterialInstance);
			}
		}
	}

	if (_deathFlipbookComponent)
	{
		_deathFlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ANPC::OnDeathAnimationFinishedPlaying);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	OnDestroyed.Broadcast();
}
/*----------------------------------------------------------------------------------------------------*/
