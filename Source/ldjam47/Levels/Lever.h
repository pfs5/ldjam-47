// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "LevelShared.h"
#include <Engine/EngineTypes.h>
#include "Lever.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UBoxComponent;
class UPaperSpriteComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ALever : public AActor
{
	GENERATED_BODY()

public:
	ALever();

public:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	void OnStateChanged();
	void Toggle();

	bool IsLocked() const;
	void SetLocked(bool value);

	UFUNCTION(BlueprintCallable)
	void SetLeverActive(bool value);

	UFUNCTION(BlueprintCallable)
	bool IsLeverActive() const;

private:
	UPROPERTY(EditDefaultsOnly)
	ELevelName _targetLevel;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* _spriteLeft;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* _spriteRight;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _boxComponent;

private:
	bool _isRight = false;
	bool _isActive = false;
};
/*----------------------------------------------------------------------------------------------------*/