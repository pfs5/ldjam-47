// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "HudWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
/*----------------------------------------------------------------------------------------------------*/
UHudWidget::UHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
//--------------------------------------------------------------------------------------------------
void UHudWidget::SetHealthProgressBar(float value)
{
	if (_healthProgressBar != nullptr)
	{
		_healthProgressBar->SetPercent(value);
	}
}
/*----------------------------------------------------------------------------------------------------*/