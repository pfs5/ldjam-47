// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/PlayerController.h"
#include "../Misc/MovablePawnsShared.h"
#include "GentlemanPlayerController.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UCharacterMovementComponent;
class UPaperFlipbook;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AGentlemanPlayerController : public APlayerController
{
	GENERATED_BODY()

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

	void AddMovementInput(const EMovementInput& input);
	void RemoveMovementInput(const EMovementInput& input);

	void UpdateMovementVector();
	void MovePlayer();

	void UpdateFlipbook();

	void ShakeCamera();

	UFUNCTION()
	void OnAttackAnimationFinishedPlaying();

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

	std::deque<EMovementInput> _movementInputs;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _verticalCameraShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> _horizontalCameraShake;
};
/*----------------------------------------------------------------------------------------------------*/