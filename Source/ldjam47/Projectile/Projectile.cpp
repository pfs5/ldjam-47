// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AProjectile::AProjectile()
{
	_projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	_flipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	_flipbookComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
/*----------------------------------------------------------------------------------------------------*/