// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "ldjam47HUD.h"
#include "HudWidget.h"
/*----------------------------------------------------------------------------------------------------*/
Aldjam47HUD::Aldjam47HUD()
{
}
/*----------------------------------------------------------------------------------------------------*/
void Aldjam47HUD::DrawHUD()
{
	Super::DrawHUD();
}
/*----------------------------------------------------------------------------------------------------*/
void Aldjam47HUD::BeginPlay()
{
	Super::BeginPlay();

	if (_hudWidgetClass != nullptr)
	{
		_hudWidget = CreateWidget<UHudWidget>(GetWorld(), _hudWidgetClass);
		if (_hudWidget != nullptr)
		{
			_hudWidget->AddToViewport();
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void Aldjam47HUD::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (_hudWidget == nullptr)
	{
		return;
	}
}
/*----------------------------------------------------------------------------------------------------*/
UHudWidget* Aldjam47HUD::GetHudWidget() const
{
	return _hudWidget;
}
/*----------------------------------------------------------------------------------------------------*/