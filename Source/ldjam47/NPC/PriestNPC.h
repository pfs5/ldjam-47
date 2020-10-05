// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "PriestNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API APriestNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void MoveToTarget(AActor* target) override;
};
/*----------------------------------------------------------------------------------------------------*/