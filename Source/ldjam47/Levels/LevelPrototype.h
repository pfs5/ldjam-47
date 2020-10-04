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

private:
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
};
/*----------------------------------------------------------------------------------------------------*/