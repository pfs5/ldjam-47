// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/Actor.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperCharacter.h"
#include "GentlemanPlayer.generated.h"
/*----------------------------------------------------------------------------------------------------*/
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

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnDeathAnimationFinishedPlaying();

private:
	float _health = 1.0f;

private:
	UPROPERTY(EditAnywhere)
	float _pixelsPerUnit = 1.f;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _umbrellaAttackHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _umbrellaAttackFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _umbrellaBlockFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* _deathFlipbook;
};
/*----------------------------------------------------------------------------------------------------*/