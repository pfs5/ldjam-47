// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "Projectile.h"
#include "../NPC/NPC.h"
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
UPaperFlipbookComponent* AProjectile::GetFlipbook() const
{
	return _flipbookComponent;
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

	if (ANPC* owner = Cast<ANPC>(GetOwner()))
	{
		owner->OnDestroyed.AddUObject(this, &AProjectile::OnOwnerDestroyed);
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
			if (playerController->GetPlayerState() == EMovablePawnState::Blocking)
			{
				
			}
			else
			{
				playerController->ApplyDamage(EMovablePawnDirection::Left, _damage);
				if (ANPC* owner = Cast<ANPC>(GetOwner()))
				{
					owner->OnNPCAttackedTarget.Broadcast(player);
				}
				Destroy();
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
void AProjectile::OnOwnerDestroyed()
{
	Destroy();
}
/*----------------------------------------------------------------------------------------------------*/