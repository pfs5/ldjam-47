// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayer.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "../UI/HudWidget.h"
#include "../UI/ldjam47HUD.h"
#include "Components/BoxComponent.h"
#include "GentlemanPlayerController.h"
#include "../GentlemansWorldSettings.h"
#include "../Levels/LevelManager.h"
#include "../Audio/AudioManager.h"
#include <Components/AudioComponent.h>
/*----------------------------------------------------------------------------------------------------*/
AGentlemanPlayer::AGentlemanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	_umbrellaAttackFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("UmbrellaAttackFlipbook"));
	_umbrellaAttackFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_umbrellaAttackFlipbook->Stop();
	_umbrellaAttackFlipbook->SetLooping(false);
	_umbrellaAttackFlipbook->SetHiddenInGame(true);

	_umbrellaAttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UmbrellaHitBox"));
	_umbrellaAttackHitBox->SetupAttachment(_umbrellaAttackFlipbook);

	_umbrellaBlockFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("UmbrellaShieldFlipbook"));
	_umbrellaBlockFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_umbrellaBlockFlipbook->Stop();
	_umbrellaBlockFlipbook->SetLooping(false);
	_umbrellaBlockFlipbook->SetHiddenInGame(true);

	_deathFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DeathFlipbook"));
	_deathFlipbook->SetupAttachment(RootComponent);
	_deathFlipbook->Stop();
	_deathFlipbook->SetLooping(false);
	_deathFlipbook->SetHiddenInGame(true);

	_attackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	_attackAudioComponent->SetupAttachment(RootComponent);

	_walkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Walk Audio Component"));
	_walkAudioComponent->SetupAttachment(RootComponent);
}
/*----------------------------------------------------------------------------------------------------*/
UBoxComponent* AGentlemanPlayer::GetUmbrellaAttackHitBox() const
{
	return _umbrellaAttackHitBox;
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* AGentlemanPlayer::GetUmbrellaAttackFlipbook() const
{
	return _umbrellaAttackFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* AGentlemanPlayer::GetUmbrellaBlockFlipbook() const
{
	return _umbrellaBlockFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::ApplyDamage(float damage)
{
	if (_isInvincible)
	{
		return;
	}

	SetHealth(_health - damage);

	OnDamageTaken();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::SetHealth(float health)
{
	if (_health == health)
	{
		return;
	}

	_health = health;
	_health = FMath::Clamp(_health, 0.0f, 1.0f);

	OnHealthChanged();
}
/*----------------------------------------------------------------------------------------------------*/
float AGentlemanPlayer::GetHealth() const
{
	return _health;
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::OnHealthChanged()
{
	UpdateHealthProgressBar();

	if (_health <= 0.0f)
	{
		if (_deathFlipbook != nullptr && GetUmbrellaBlockFlipbook() != nullptr && GetUmbrellaAttackFlipbook() != nullptr && GetSprite() != nullptr)
		{
			GetSprite()->SetHiddenInGame(true);
			GetUmbrellaAttackFlipbook()->SetHiddenInGame(true);
			GetUmbrellaBlockFlipbook()->SetHiddenInGame(true);
			_deathFlipbook->SetHiddenInGame(false);

			_deathFlipbook->PlayFromStart();
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::UpdateHealthProgressBar()
{
	AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(GetController());
	if (playerController == nullptr)
	{
		return;
	}

	Aldjam47HUD* hud = Cast<Aldjam47HUD>(playerController->GetHUD());
	if (hud != nullptr)
	{
		UHudWidget* hudWidget = hud->GetHudWidget();
		if (hudWidget != nullptr)
		{
			hudWidget->SetHealthProgressBar(_health);
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::Attack()
{
	_attackAudioComponent->Play();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::StartWalk()
{
	_walkAudioComponent->Play();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::StopWalk()
{
	_walkAudioComponent->Stop();
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AGentlemanPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (_deathFlipbook)
	{
		_deathFlipbook->OnFinishedPlaying.AddDynamic(this, &AGentlemanPlayer::OnDeathAnimationFinishedPlaying);
	}

	if (_spriteMaterial != nullptr)
	{
		_spriteMaterialInstance = UMaterialInstanceDynamic::Create(_spriteMaterial, this);
		if (UPaperFlipbookComponent* flipbookComp = GetSprite())
		{
			for (int32 i = 0; i < flipbookComp->GetNumMaterials(); ++i)
			{
				flipbookComp->SetMaterial(i, _spriteMaterialInstance);
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AGentlemanPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(_invincibilityTimerHandle);

	Super::EndPlay(EndPlayReason);
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AGentlemanPlayer::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	const float unitsPerPixel = 1.f / _pixelsPerUnit;

	FVector location = GetActorLocation();
	location.X = FMath::GridSnap(location.X, unitsPerPixel);
	location.Z = FMath::GridSnap(location.Z, unitsPerPixel);
	SetActorLocation(location);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::OnDeathAnimationFinishedPlaying()
{
	Reset();

	OnPlayerDeath.Broadcast();
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::Reset()
{
	SetHealth(1.0f);

	_deathFlipbook->SetHiddenInGame(true);
	GetSprite()->SetHiddenInGame(false);
	
	if (AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(GetController()))
	{
		playerController->Reset();
	}

	if(AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (ALevelManager* levelManager = ws->GetLevelManager())
		{
			levelManager->ResetLevels();
		}
	}

	if (AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (AAudioManager* audioManager = ws->GetAudioManager())
		{
			audioManager->RestartAudio();
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::OnDamageTaken()
{
	if (!_isInvincible)
	{
		SetInvincible(true);
		GetWorldTimerManager().SetTimer(_invincibilityTimerHandle, this, &AGentlemanPlayer::OnInvincibilityTimer, _hitInvincibilityTime, false);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::OnInvincibilityTimer()
{
	SetInvincible(false);
}
/*----------------------------------------------------------------------------------------------------*/
void AGentlemanPlayer::SetInvincible(bool value)
{
	_isInvincible = value;

	if (_spriteMaterialInstance != nullptr)
	{
		int32 blinkIntensity = value ? 1 : 0;
		_spriteMaterialInstance->SetScalarParameterValue("BlinkIntensity", blinkIntensity);
	}
}
/*----------------------------------------------------------------------------------------------------*/