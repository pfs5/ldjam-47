// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ldjam47HUD.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UQuest;
class UHudWidget;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API Aldjam47HUD : public AHUD
{
	GENERATED_BODY()

public:
	Aldjam47HUD();
	
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UHudWidget* GetHudWidget() const;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> _hudWidgetClass;
	
	UHudWidget* _hudWidget;
};
/*----------------------------------------------------------------------------------------------------*/