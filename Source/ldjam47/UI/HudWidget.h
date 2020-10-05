// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class UProgressBar;
class UOverlay;
class UImage;
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHudWidget(const FObjectInitializer& ObjectInitializer);

	void SetHealthProgressBar(float value);

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* _healthProgressBar;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UOverlay* _healthOverlay;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* _hpBar_0;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* _hpBar_1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* _hpBar_2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* _hpBar_3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* _hpBar_4;

private:
	void SetHpValue(int32 value);
};
/*----------------------------------------------------------------------------------------------------*/