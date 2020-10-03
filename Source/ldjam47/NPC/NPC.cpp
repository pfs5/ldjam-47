// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "NPC.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
void ANPC::SetLastMovementInput(FVector movementVector)
{
	if (movementVector.Z <= 0.0f)
	{
		_lastMovementInput = EMovementInput::Left;
	}
	else if (movementVector.Z > 0.0f)
	{
		_lastMovementInput = EMovementInput::Right;
	}
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


}
/*----------------------------------------------------------------------------------------------------*/
void ANPC::Reset()
{
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
	float distance = direction.Size();
	if (distance < _targetRadius)
	{
		OnArrivedToTarget();
		return;
	}

	AddMovementInput(direction);
	SetLastMovementInput(direction);
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
/*virtual*/
void ANPC::OnArrivedToTarget()
{

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
