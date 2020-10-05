// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/Actor.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperCharacter.h"
#include "GentlemanPlayer.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UAudioComponent;
class UBoxComponent;
class UPaperFlipbookComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AGentlemanPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	DECLARE_EVENT(AGentlemanPlayer, FOnPlayerDeath)
	FOnPlayerDeath OnPlayerDeath;

public:
	AGentlemanPlayer();

	UBoxComponent* GetUmbrellaAttackHitBox() const;
	UPaperFlipbookComponent* GetUmbrellaAttackFlipbook() const;
	UPaperFlipbookComponent* GetUmbrellaBlockFlipbook() const;

	void ApplyDamage(float damage);
	void SetHealth(float health);
	float GetHealth() const;
	void OnHealthChanged();

	void UpdateHealthProgressBar();

	void Attack();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnDeathAnimationFinishedPlaying();

	void Reset();

	void OnDamageTaken();
	void OnInvincibilityTimer();
	void SetInvincible(bool value);

private:
	float _health = 1.0f;
	bool _isInvincible = false;
	FTimerHandle _invincibilityTimerHandle;
	UMaterialInstanceDynamic* _spriteMaterialInstance;

private:
	UPROPERTY(EditAnywhere, Category = "Gentleman")
	float _pixelsPerUnit = 1.f;

	UPROPERTY(EditAnywhere, Category = "Gentleman")
	float _hitInvincibilityTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "Gentleman")
	UMaterialInterface* _spriteMaterial;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _umbrellaAttackHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _umbrellaAttackFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _umbrellaBlockFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _deathFlipbook;

	UPROPERTY(EditAnywhere)
	UAudioComponent* _attackAudioComponent;
};
/*----------------------------------------------------------------------------------------------------*/