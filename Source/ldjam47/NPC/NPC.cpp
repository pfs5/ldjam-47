// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "NPC.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
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
void ANPC::ApplyKnockback(EMovablePawnDirection direction)
{
	if (direction == EMovablePawnDirection::Left)
	{
		LaunchCharacter(FVector(-5.0f, 0.0f, 0.0f), false, false);
	}
	else if (direction == EMovablePawnDirection::Right)
	{
		LaunchCharacter(FVector(5.0f, 0.0f, 0.0f), false, false);
	}
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* ANPC::GetAttackFlipbook() const
{
	return _attackFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ANPC::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (_targetPlayer != nullptr)
	{
		MoveToTarget(_targetPlayer.Get());
	}

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
					flipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Left:
			{
				if (_idleLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleLeftFlipbook);
					flipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
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
					flipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
				}
				break;
			}
			case EMovablePawnDirection::Left:
			{
				if (_walkLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkLeftFlipbook);
					flipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
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
	_npcDirection = EMovablePawnDirection::Left;
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

	FVector npcLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	FVector direction = targetLocation - npcLocation;

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::SanitizeFloat(direction.X));

	float distance = direction.Size();
	if (distance < _targetRadius)
	{
		OnArrivedToTarget();
		return;
	}

	AddMovementInput(direction);
	SetLastMovementVector(direction);
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
/*virtual*/
void ANPC::OnArrivedToTarget()
{
	
}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::ApplyDamage(EMovablePawnDirection direction)
{
	SetHealth(_health - _damageOnHit);
	
	if (_applyKnockback)
	{
		//ApplyKnockback(direction);
	}
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
		Destroy();
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (APawn* targetPlayer = playerController->GetPawn())
		{
			_targetPlayer = targetPlayer;
		}
	}

	Reset();
}
/*----------------------------------------------------------------------------------------------------*/
