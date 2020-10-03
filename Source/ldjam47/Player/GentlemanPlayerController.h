// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/PlayerController.h"
#include "GentlemanPlayerController.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UCharacterMovementComponent;
class UPaperFlipbook;
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	None,
	Idle,
	Walking,
	Attacking,
	Blocking
};
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EPlayerDirection : uint8
{
	Right,
	Left,
	Back,
	Front
};
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EMovementInput : uint8
{
	None,
	Up,
	Down,
	Left,
	Right
};
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

	void SetPlayerState(EPlayerState playerState);
	EPlayerState GetPlayerState() const;
	void OnPlayerStateChanged();

	void SetPlayerDirection(EPlayerDirection playerDirection);
	EPlayerDirection GetPlayerDirection() const;
	void OnPlayerDirectionChanged();

	void SetFlipbook(EPlayerState playerState, EPlayerDirection playerDirection);

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
	EPlayerState _playerState;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	EPlayerDirection _playerDirection;

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