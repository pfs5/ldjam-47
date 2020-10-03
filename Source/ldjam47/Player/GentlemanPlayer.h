// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/Actor.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperCharacter.h"
#include "GentlemanPlayer.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UBoxComponent;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AGentlemanPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	AGentlemanPlayer();

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float _pixelsPerUnit = 1.f;

	UPROPERTY(EditAnywhere)
	UBoxComponent* _caneHitBox;
};
/*----------------------------------------------------------------------------------------------------*/