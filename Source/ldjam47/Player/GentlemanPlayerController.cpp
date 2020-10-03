// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayerController.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbook.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GentlemanPlayer.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AGentlemanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent == nullptr)
	{
		return;
	}

	InputComponent->BindAction("Up", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnUpPressed);
	InputComponent->BindAction("Down", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnDownPressed);
	InputComponent->BindAction("Left", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnLeftPressed);
	InputComponent->BindAction("Right", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnRightPressed);

	InputComponent->BindAction("Up", EInputEvent::IE_Released, this, &AGentlemanPlayerController::InputComponent_OnUpReleased);
	InputComponent->BindAction("Down", EInputEvent::IE_Released, this, &AGentlemanPlayerController::InputComponent_OnDownReleased);
	InputComponent->BindAction("Left", EInputEvent::IE_Released, this, &AGentlemanPlayerController::InputComponent_OnLeftReleased);
	InputComponent->BindAction("Right", EInputEvent::IE_Released, this, &AGentlemanPlayerController::InputComponent_OnRightReleased);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (_movementComponent == nullptr || _isInGame == false)
	{
		return;
	}

	float currentAccelerationX = _movementComponent->GetCurrentAcceleration().X;
	float currentAccelerationZ = _movementComponent->GetCurrentAcceleration().Z;

	if (currentAccelerationX != 0.f || currentAccelerationZ != 0)
	{
		SetPlayerState(EPlayerState::Walking);
	}
	else if (currentAccelerationX == 0.f && currentAccelerationZ == 0)
	{
		SetPlayerState(EPlayerState::Idle);
	}

	MovePlayer();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnPossess(APawn* possesedPawn)
{
	Super::OnPossess(possesedPawn);

	if (possesedPawn != nullptr)
	{
		_owningPlayer = possesedPawn;

		_movementComponent = Cast<UCharacterMovementComponent>(_owningPlayer->GetMovementComponent());

		if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer))
		{
			if (UCapsuleComponent* capsuleComponent = player->GetCapsuleComponent())
			{
				capsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AGentlemanPlayerController::OnOverlapBegin);
			}
		}

		Reset();

		_isInGame = true;
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	_isInGame = false;

	SetPlayerState(EPlayerState::None);

	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer))
	{
		if (UCapsuleComponent* capsuleComponent = player->GetCapsuleComponent())
		{
			capsuleComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AGentlemanPlayerController::OnOverlapBegin);
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
APawn* AGentlemanPlayerController::GetOwningPlayer() const
{
	return _owningPlayer;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::SetPlayerState(EPlayerState playerState)
{
	if (_playerState == playerState)
	{
		return;
	}

	_playerState = playerState;

	OnPlayerStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EPlayerState AGentlemanPlayerController::GetPlayerState() const
{
	return _playerState;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnPlayerStateChanged()
{
	SetFlipbook(_playerState, _playerDirection);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::SetPlayerDirection(EPlayerDirection playerDirection)
{
	if (_playerDirection == playerDirection)
	{
		return;
	}

	_playerDirection = playerDirection;

	OnPlayerDirectionChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EPlayerDirection AGentlemanPlayerController::GetPlayerDirection() const
{
	return _playerDirection;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnPlayerDirectionChanged()
{
	SetFlipbook(_playerState, _playerDirection);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::SetFlipbook(EPlayerState playerState, EPlayerDirection playerDirection)
{
	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	UPaperFlipbookComponent* flipbook = player->GetSprite();
	if (flipbook == nullptr)
	{
		return;
	}

	if (playerState == EPlayerState::Idle)
	{
		switch (_playerDirection)
		{
			case EPlayerDirection::Right:
			{
				if (_idleRightFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleRightFlipbook);
				}
				break;
			}
			case EPlayerDirection::Left:
			{
				if (_idleLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleLeftFlipbook);
				}
				break;
			}
			case EPlayerDirection::Back:
			{
				if (_idleUpFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleUpFlipbook);
				}
				break;
			}
			case EPlayerDirection::Front:
			{
				if (_idleDownFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_idleDownFlipbook);
				}
				break;
			}
		}
	}
	else
	{
		switch (_playerDirection)
		{
			case EPlayerDirection::Right:
			{
				if (_walkRightFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkRightFlipbook);
				}
				break;
			}
			case EPlayerDirection::Left:
			{
				if (_walkLeftFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkLeftFlipbook);
				}
				break;
			}
			case EPlayerDirection::Back:
			{
				if (_walkUpFlipbook != nullptr)
				{
					flipbook->SetFlipbook(_walkUpFlipbook);
				}
				break;
			}
			case EPlayerDirection::Front:
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
void AGentlemanPlayerController::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr && otherActor == this && otherComp == nullptr)
	{
		return;
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnUpPressed()
{
	_movementUp = 1.f;
	AddMovementInput(EMovementInput::Up);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnDownPressed()
{
	_movementDown = 1.f;
	AddMovementInput(EMovementInput::Down);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnLeftPressed()
{
	_movementLeft = 1.f;
	AddMovementInput(EMovementInput::Left);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnRightPressed()
{
	_movementRight = 1.f;
	AddMovementInput(EMovementInput::Right);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnUpReleased()
{
	_movementUp = 0.f;
	RemoveMovementInput(EMovementInput::Up);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnDownReleased()
{
	_movementDown = 0.f;
	RemoveMovementInput(EMovementInput::Down);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnLeftReleased()
{
	_movementLeft = 0.f;
	RemoveMovementInput(EMovementInput::Left);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnRightReleased()
{
	_movementRight = 0.f;
	RemoveMovementInput(EMovementInput::Right);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::AddMovementInput(const EMovementInput& input)
{
	RemoveMovementInput(input);
	_movementInputs.push_front(input);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::RemoveMovementInput(const EMovementInput& input)
{
	_movementInputs.erase(std::remove(_movementInputs.begin(), _movementInputs.end(), input), _movementInputs.end());
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::UpdateMovementVector()
{
	EMovementInput lastInput = _movementInputs.empty() ? EMovementInput::None : _movementInputs.front();

	_movementVector.X = lastInput == EMovementInput::Left || lastInput == EMovementInput::Right ? _movementRight - _movementLeft : 0.f;
	_movementVector.Z = lastInput == EMovementInput::Up || lastInput == EMovementInput::Down ? _movementUp - _movementDown : 0.f;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::MovePlayer()
{
	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer))
	{
		player->AddMovementInput(_movementVector);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::UpdateFlipbook()
{
	// Get player direction
	EMovementInput lastMovementInput = _movementInputs.empty() ? EMovementInput::None : _movementInputs.front();
	if (lastMovementInput == EMovementInput::None)
	{
		return;
	}

	switch (lastMovementInput)
	{
		case EMovementInput::Up:
		{
			SetPlayerDirection(EPlayerDirection::Back);
			break;
		}
		case EMovementInput::Down:
		{
			SetPlayerDirection(EPlayerDirection::Front);
			break;
		}
		case EMovementInput::Left:
		{
			SetPlayerDirection(EPlayerDirection::Left);
			break;
		}
		case EMovementInput::Right:
		{
			SetPlayerDirection(EPlayerDirection::Right);
			break;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/