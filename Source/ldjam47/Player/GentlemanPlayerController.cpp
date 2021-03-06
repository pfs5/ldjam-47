// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayerController.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbook.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "../NPC/NPC.h"
#include "Components/BoxComponent.h"
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

	InputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnAttackPressed);
	InputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AGentlemanPlayerController::InputComponent_OnBlockPressed);
	InputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AGentlemanPlayerController::InputComponent_OnBlockReleased);

}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (_movementComponent == nullptr || _isInGame == false)
	{
		return;
	}

	if (_playerState != EMovablePawnState::Attacking && _playerState != EMovablePawnState::Blocking)
	{
		float currentAccelerationX = _movementComponent->GetCurrentAcceleration().X;
		float currentAccelerationZ = _movementComponent->GetCurrentAcceleration().Z;

		if (currentAccelerationX != 0.f || currentAccelerationZ != 0)
		{
			SetPlayerState(EMovablePawnState::Walking);
		}
		else if (currentAccelerationX == 0.f && currentAccelerationZ == 0)
		{
			SetPlayerState(EMovablePawnState::Idle);
		}
	}

	MovePlayer();

	_attackDelayTimer += deltaSeconds;
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
			if (UPaperFlipbookComponent* caneAttackFlipbook = player->GetUmbrellaAttackFlipbook())
			{
				caneAttackFlipbook->OnFinishedPlaying.AddDynamic(this, &AGentlemanPlayerController::OnAttackAnimationFinishedPlaying);
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

	SetPlayerState(EMovablePawnState::None);

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
void AGentlemanPlayerController::SetPlayerState(EMovablePawnState playerState)
{
	if (_playerState == playerState)
	{
		return;
	}

	_playerState = playerState;

	OnPlayerStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnState AGentlemanPlayerController::GetPlayerState() const
{
	return _playerState;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnPlayerStateChanged()
{
	SetFlipbook(_playerState, _playerDirection);

	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	switch (_playerState)
	{
		case EMovablePawnState::None:
			player->StopWalk();
			break;
		case EMovablePawnState::Idle:
			player->StopWalk();
			break;
		case EMovablePawnState::Walking:
			player->StartWalk();
			break;
		case EMovablePawnState::Attacking:
			player->StopWalk();
			break;
		case EMovablePawnState::Blocking:
			player->StopWalk();
			break;
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::SetPlayerDirection(EMovablePawnDirection playerDirection)
{
	if (_playerDirection == playerDirection)
	{
		return;
	}

	_playerDirection = playerDirection;

	OnPlayerDirectionChanged();
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnDirection AGentlemanPlayerController::GetPlayerDirection() const
{
	return _playerDirection;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnPlayerDirectionChanged()
{
	SetFlipbook(_playerState, _playerDirection);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::SetFlipbook(EMovablePawnState playerState, EMovablePawnDirection playerDirection)
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

	UPaperFlipbookComponent* umbrellaAttackFlipbook = player->GetUmbrellaAttackFlipbook();
	if (umbrellaAttackFlipbook == nullptr)
	{
		return;
	}

	UPaperFlipbookComponent* umbrellaBlockFlipbook = player->GetUmbrellaBlockFlipbook();
	if (umbrellaBlockFlipbook == nullptr)
	{
		return;
	}

	if (playerState == EMovablePawnState::Idle)
	{
		switch (_playerDirection)
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
	else if(_playerState == EMovablePawnState::Walking)
	{
		switch (_playerDirection)
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
	else if (_playerState == EMovablePawnState::Attacking)
	{
		if (_playerDirection == EMovablePawnDirection::Left)
		{
			umbrellaAttackFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
		else if (_playerDirection == EMovablePawnDirection::Right)
		{
			umbrellaAttackFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		}

		flipbook->SetHiddenInGame(true);
		umbrellaAttackFlipbook->SetHiddenInGame(false);
		umbrellaAttackFlipbook->PlayFromStart();
	}
	else if (_playerState == EMovablePawnState::Blocking)
	{
		if (_playerDirection == EMovablePawnDirection::Left)
		{
			umbrellaBlockFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
		else if (_playerDirection == EMovablePawnDirection::Right)
		{
			umbrellaBlockFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		}

		flipbook->SetHiddenInGame(true);
		umbrellaBlockFlipbook->SetHiddenInGame(false);
		umbrellaBlockFlipbook->PlayFromStart();
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::Reset()
{
	_playerDirection = EMovablePawnDirection::Left;
	_owningPlayer->SetActorRotation(FRotator::ZeroRotator);
	SetPlayerState(EMovablePawnState::Idle);

	_attackDelayTimer = _attackDelay;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::ResetMovement()
{
	_movementUp = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Up);
	_movementDown = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Down);
	_movementLeft = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Left);
	_movementRight = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Right);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::ApplyDamage(EMovablePawnDirection direction, float damage)
{
	if (_playerState == EMovablePawnState::Blocking && GetOppositeDirection(direction) == _playerDirection)
	{
		return;
	}

	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	player->ApplyDamage(damage);
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
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementUp = 1.f;
	AddVerticalMovementInput(EMovementInput::Up);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnDownPressed()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementDown = 1.f;
	AddVerticalMovementInput(EMovementInput::Down);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnLeftPressed()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementLeft = 1.f;
	AddHorizontalMovementInput(EMovementInput::Left);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnRightPressed()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementRight = 1.f;
	AddHorizontalMovementInput(EMovementInput::Right);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnUpReleased()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementUp = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Up);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnDownReleased()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementDown = 0.f;
	RemoveVerticalMovementInput(EMovementInput::Down);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnLeftReleased()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementLeft = 0.f;
	RemoveHorizontalMovementInput(EMovementInput::Left);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnRightReleased()
{
	if (_playerState == EMovablePawnState::Blocking/* || _playerState == EMovablePawnState::Attacking*/)
	{
		return;
	}

	_movementRight = 0.f;
	RemoveHorizontalMovementInput(EMovementInput::Right);

	UpdateMovementVector();
	UpdateFlipbook();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnAttackPressed()
{
	if (_playerState == EMovablePawnState::Attacking/* || _playerState == EMovablePawnState::Blocking*/)
	{
		return;
	}

	if (_attackDelayTimer < _attackDelay)
	{
		return;
	}

	//ResetMovement();
	SetPlayerState(EMovablePawnState::Attacking);
	AttackScan();
	
	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer))
	{
		player->Attack();
	}

	_attackDelayTimer = 0.0f;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnBlockPressed()
{
	ResetMovement();
	SetPlayerState(EMovablePawnState::Blocking);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::InputComponent_OnBlockReleased()
{
	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	if (UPaperFlipbookComponent* umbrellaBlockFlipbook = player->GetUmbrellaBlockFlipbook())
	{
		umbrellaBlockFlipbook->SetHiddenInGame(true);
	}

	if (UPaperFlipbookComponent* flipbook = player->GetSprite())
	{
		flipbook->SetHiddenInGame(false);
	}

	SetPlayerState(EMovablePawnState::Idle);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::AddVerticalMovementInput(const EMovementInput& input)
{
	RemoveVerticalMovementInput(input);
	_verticalMovementInputs.push_front(input);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::RemoveVerticalMovementInput(const EMovementInput& input)
{
	_verticalMovementInputs.erase(std::remove(_verticalMovementInputs.begin(), _verticalMovementInputs.end(), input), _verticalMovementInputs.end());
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::AddHorizontalMovementInput(const EMovementInput& input)
{
	RemoveHorizontalMovementInput(input);
	_horizontalMovementInputs.push_front(input);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::RemoveHorizontalMovementInput(const EMovementInput& input)
{
	_horizontalMovementInputs.erase(std::remove(_horizontalMovementInputs.begin(), _horizontalMovementInputs.end(), input), _horizontalMovementInputs.end());
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::UpdateMovementVector()
{
	EMovementInput lastVerticalInput = _verticalMovementInputs.empty() ? EMovementInput::None : _verticalMovementInputs.front();
	EMovementInput lastHorizontalInput = _horizontalMovementInputs.empty() ? EMovementInput::None : _horizontalMovementInputs.front();

	_movementVector.X = lastHorizontalInput == EMovementInput::Left || lastHorizontalInput == EMovementInput::Right ? _movementRight - _movementLeft : 0.f;
	_movementVector.Z = lastVerticalInput == EMovementInput::Up || lastVerticalInput == EMovementInput::Down ? _movementUp - _movementDown : 0.f;
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
	EMovementInput lastMovementInput = _horizontalMovementInputs.empty() ? EMovementInput::None : _horizontalMovementInputs.front();
	if (lastMovementInput == EMovementInput::None)
	{
		return;
	}

	switch (lastMovementInput)
	{
		case EMovementInput::Up:
		{
			SetPlayerDirection(EMovablePawnDirection::Back);
			break;
		}
		case EMovementInput::Down:
		{
			SetPlayerDirection(EMovablePawnDirection::Front);
			break;
		}
		case EMovementInput::Left:
		{
			SetPlayerDirection(EMovablePawnDirection::Left);
			break;
		}
		case EMovementInput::Right:
		{
			SetPlayerDirection(EMovablePawnDirection::Right);
			break;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::ShakeCamera()
{
	if (_verticalCameraShake == nullptr || PlayerCameraManager == nullptr)
	{
		return;
	}

	if (_playerDirection == EMovablePawnDirection::Left || _playerDirection == EMovablePawnDirection::Right)
	{
		PlayerCameraManager->PlayCameraShake(_horizontalCameraShake, 1.0f);
	}
	else
	{
		PlayerCameraManager->PlayCameraShake(_verticalCameraShake, 1.0f);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::AttackScan()
{
	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	UBoxComponent* attackHitBox = player->GetUmbrellaAttackHitBox();
	if (attackHitBox == nullptr)
	{
		return;
	}

	TArray<AActor*> overlappingActors;
	attackHitBox->GetOverlappingActors(overlappingActors);
	for (AActor* overlappingActor : overlappingActors)
	{
		if (ANPC* npc = Cast<ANPC>(overlappingActor))
		{
			if (npc->GetHealth() > 0.f)
			{
				npc->ApplyDamage(_playerDirection);
				ShakeCamera();
				OnPlayerAttackedTarget.Broadcast(npc);
				return;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, *overlappingActor->GetName());
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayerController::OnAttackAnimationFinishedPlaying()
{
	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(_owningPlayer);
	if (player == nullptr)
	{
		return;
	}

	if (UPaperFlipbookComponent* caneAttackFlipbook = player->GetUmbrellaAttackFlipbook())
	{
		caneAttackFlipbook->SetHiddenInGame(true);
	}

	if (UPaperFlipbookComponent* flipbook = player->GetSprite())
	{
		flipbook->SetHiddenInGame(false);
	}

	SetPlayerState(EMovablePawnState::Idle);
}
/*----------------------------------------------------------------------------------------------------*/
EMovablePawnDirection AGentlemanPlayerController::GetOppositeDirection(EMovablePawnDirection direction)
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