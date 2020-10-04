// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "MonkNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AMonkNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void MoveToTarget(AActor* target) override;
};
/*----------------------------------------------------------------------------------------------------*/