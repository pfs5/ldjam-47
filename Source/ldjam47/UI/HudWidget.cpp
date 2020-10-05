// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "HudWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
/*----------------------------------------------------------------------------------------------------*/
UHudWidget::UHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
//--------------------------------------------------------------------------------------------------
void UHudWidget::SetHealthProgressBar(float value)
{
	int32 hpInt = FMath::RoundToInt(value * 4.f);
	SetHpValue(hpInt);
/*
	if (_healthProgressBar != nullptr)
	{
		_healthProgressBar->SetPercent(value);
	}*/
}
/*----------------------------------------------------------------------------------------------------*/
void UHudWidget::SetHpValue(int32 value)
{
	_hpBar_0->SetVisibility(value == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden );
	_hpBar_1->SetVisibility(value == 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden );
	_hpBar_2->SetVisibility(value == 2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden );
	_hpBar_3->SetVisibility(value == 3 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden );
	_hpBar_4->SetVisibility(value == 4 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden );
}
/*----------------------------------------------------------------------------------------------------*/