// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/Actor.h"
#include "../Misc/MovablePawnsShared.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperCharacter.h"
#include "NPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class APawn;
class UBoxComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ANPC : public APaperCharacter
{
	GENERATED_BODY()

public:
	ANPC();

	UPaperFlipbookComponent* GetAttackFlipbook() const;

public:
	virtual void Tick(float DeltaTime) override;

	void SetNPCState(EMovablePawnState npcState);
	EMovablePawnState GetNPCState() const;
	void OnNPCStateChanged();

	void SetNPCDirection(EMovablePawnDirection npcDirection);
	EMovablePawnDirection GetNPCDirection() const;
	void OnNPCDirectionChanged();

	void SetFlipbook(EMovablePawnState npcState, EMovablePawnDirection npcDirection);

	void Reset();

	virtual void MoveToTarget(AActor* target);
	virtual void OnArrivedToTarget();

protected:
	virtual void BeginPlay() override;

private:
	void SnapLocation();

	void SetLastMovementInput(FVector movementVector);

	void UpdateFlipbook();

private:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<APawn> _targetPlayer;

	UPROPERTY(EditAnywhere)
	float _targetRadius = 15.0f;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
	EMovablePawnState _npcState;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
	EMovablePawnDirection _npcDirection;

	UPROPERTY(EditAnywhere)
	float _pixelsPerUnit = 1.f;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _attackHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _attackFlipbook;

	EMovementInput _lastMovementInput;

private:
	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _idleDownFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _idleUpFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _idleLeftFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _idleRightFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _walkDownFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _walkUpFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _walkLeftFlipbook;

	UPROPERTY(EditAnywhere, Category = "NPC|Flipbook")
	UPaperFlipbook* _walkRightFlipbook;
};
/*----------------------------------------------------------------------------------------------------*/