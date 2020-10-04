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
	DECLARE_EVENT_OneParam(ANPC, FOnNPCAttackedTarget, AActor*)
	FOnNPCAttackedTarget OnNPCAttackedTarget;

public:
	ANPC();

	UPaperFlipbookComponent* GetAttackFlipbook() const;
	APawn* GetTargetPlayer() const;

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
	virtual void OnArrivedToTarget(AActor* target);
	virtual void AttackTarget(AActor* target);

	virtual void ApplyDamage(EMovablePawnDirection direction);
	void SetHealth(float health);
	float GetHealth() const;
	void OnHealthChanged();

	void TurnTowardsTarget(AActor* target);

protected:
	virtual void BeginPlay() override;

private:
	void SnapLocation();

	void SetLastMovementVector(FVector movementVector);
	void OnLastMovementVectorChanged(FVector oldMovementVector);
	void SetLastMovementInput(EMovementInput movementInput);

	void UpdateFlipbook();

	void OnDamageTaken();
	void ApplyKnockback(EMovablePawnDirection direction);

	void TickHitEffects(float deltaTime);

	APawn* FindPlayerPawn() const;

protected:
	float _health = 1.0f;

	UPROPERTY(EditAnywhere)
	float _damageOnHit = 0.25f;

	UPROPERTY(EditAnywhere)
	bool _applyKnockback = true;

	UPROPERTY(EditAnywhere)
	float _attackDamage = 0.1f;

	UPROPERTY(EditAnywhere)
	float _attackDelay = 2.0f;

	float _attackDelayTimer = 2.0f;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* _spriteMaterial;
	UMaterialInstanceDynamic* _spriteMaterialInstance = nullptr;

	float _hitEffectValue = 0.f;

	UPROPERTY(EditAnywhere)
	float _knockbackOnHit = 5.0f;

private:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<APawn> _targetPlayer;

	UPROPERTY(EditAnywhere)
	float _targetRadius = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
	EMovablePawnState _npcState;

	UPROPERTY(VisibleAnywhere, Category = "NPC")
	EMovablePawnDirection _npcDirection;

	UPROPERTY(EditAnywhere)
	float _pixelsPerUnit = 1.f;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _attackHitBox;

	UPROPERTY(EditAnywhere, Category = "NPC|HitEffect")
	float _hitEffectStrength = 5.f;

	UPROPERTY(EditAnywhere, Category = "NPC|HitEffect")
	float _hitEffectDecaySpeed = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _attackFlipbook;

	FVector _lastMovementVector;

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