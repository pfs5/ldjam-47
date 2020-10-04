// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "LevelPrototype.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UBoxComponent;
class UPaperSpriteComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ALevelPrototype : public AActor
{
	GENERATED_BODY()

public:
	ALevelPrototype();

	void BeginPlay() override;
	void Tick(float deltaTime) override;

	const FVector& GetPlayerStartLocation() const;

// Events
public:
	void Event_OnMonkHit();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Event_OnMonkHit();

	void Event_OnTwoSecondsPassed();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Event_OnTwoSecondsPassed();

	void Event_OnNPCDied();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Event_OnNPCDied();

private:
	UPROPERTY(EditAnywhere, Category = "LevelPrototype")
	FVector _playerStartLocation = FVector(0.f, 5.f, 0.f);

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* _background;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* _foreground;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _boxLeft;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _boxRight;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _boxTop;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _boxBottom;

private:
	float _timeCounter;
};
/*----------------------------------------------------------------------------------------------------*/