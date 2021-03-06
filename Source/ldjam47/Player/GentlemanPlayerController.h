// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/PlayerController.h"
#include "../Misc/MovablePawnsShared.h"
#include "GentlemanPlayerController.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UCharacterMovementComponent;
class UPaperFlipbook;
class UPaperSprite;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AGentlemanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(AGentlemanPlayer, FOnPlayerAttackedTarget, AActor*)
	FOnPlayerAttackedTarget OnPlayerAttackedTarget;

public:
	virtual void SetupInputComponent() override;

	virtual void Tick(float deltaSeconds) override;

	virtual void OnPossess(APawn* possesedPawn) override;

	virtual void OnUnPossess() override;

public:
	APawn* GetOwningPlayer() const;

	void SetPlayerState(EMovablePawnState playerState);
	EMovablePawnState GetPlayerState() const;
	void OnPlayerStateChanged();

	void SetPlayerDirection(EMovablePawnDirection playerDirection);
	EMovablePawnDirection GetPlayerDirection() const;
	void OnPlayerDirectionChanged();

	void SetFlipbook(EMovablePawnState playerState, EMovablePawnDirection playerDirection);

	void Reset();
	void ResetMovement();

	void ApplyDamage(EMovablePawnDirection direction, float damage);

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	void InputComponent_OnUpPressed();
	void InputComponent_OnDownPressed();
	void InputComponent_OnLeftPressed();
	void InputComponent_OnRightPressed();

	void InputComponent_OnUpReleased();
	void InputComponent_OnDownReleased();
	void InputComponent_OnLeftReleased();
	void InputComponent_OnRightReleased();

	void InputComponent_OnAttackPressed();
	void InputComponent_OnBlockPressed();
	void InputComponent_OnBlockReleased();

	void AddVerticalMovementInput(const EMovementInput& input);
	void RemoveVerticalMovementInput(const EMovementInput& input);
	void AddHorizontalMovementInput(const EMovementInput& input);
	void RemoveHorizontalMovementInput(const EMovementInput& input);

	void UpdateMovementVector();
	void MovePlayer();

	void UpdateFlipbook();

	void ShakeCamera();

	void AttackScan();
	UFUNCTION()
	void OnAttackAnimationFinishedPlaying();

	EMovablePawnDirection GetOppositeDirection(EMovablePawnDirection direction);

private:
	UPROPERTY(EditAnywhere)
	float _attackDelay = 0.25f;

	float _attackDelayTimer = 0.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	EMovablePawnState _playerState;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	EMovablePawnDirection _playerDirection;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _idleDownFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _idleUpFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _idleLeftFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _idleRightFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _walkDownFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _walkUpFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _walkLeftFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _walkRightFlipbook;

	UPROPERTY(EditAnywhere, Category = "Player|Flipbook")
	UPaperFlipbook* _deathFlipbook;

private:
	UPROPERTY()
	APawn* _owningPlayer;

	UPROPERTY()
	UCharacterMovementComponent* _movementComponent;

	bool _isInGame = false;

	FVector _movementVector = FVector::ZeroVector;

	float _movementUp = 0.f;
	float _movementDown = 0.f;
	float _movementLeft = 0.f;
	float _movementRight = 0.f;

	std::deque<EMovementInput> _verticalMovementInputs;
	std::deque<EMovementInput> _horizontalMovementInputs;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _verticalCameraShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _horizontalCameraShake;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TArray<UPaperSprite*> _startCutScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<UPaperSprite*> _endCutScene;
};
/*----------------------------------------------------------------------------------------------------*/