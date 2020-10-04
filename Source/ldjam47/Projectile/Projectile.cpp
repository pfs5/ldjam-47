// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "Projectile.h"
#include "../Player/GentlemanPlayer.h"
#include "../Player/GentlemanPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AProjectile::AProjectile()
{
	_projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	_sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = _sphereComponent;

	_flipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	_flipbookComponent->SetupAttachment(RootComponent);
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (_sphereComponent)
	{
		_sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AProjectile::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr && otherActor == this && otherComp == nullptr)
	{
		return;
	}

	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(otherActor))
	{
		if (AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(player->GetController()))
		{
			playerController->ApplyDamage(EMovablePawnDirection::Left, _damage);
			Destroy();
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/