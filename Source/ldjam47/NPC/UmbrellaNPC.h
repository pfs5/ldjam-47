// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "NPC.h"
#include "UmbrellaNPC.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AProjectile;
class UProjectileMovementComponent;
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EUmbrellaAttackState : uint8
{
	None,
	Open,
	Closed
};
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API AUmbrellaNPC : public ANPC
{
	GENERATED_BODY()

public:
	AUmbrellaNPC();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void MoveToTarget(AActor* target) override;
	virtual void OnArrivedToTarget(AActor* target) override;
	virtual void AttackTarget(AActor* target) override;

	void SetUmbrellaAttackState(EUmbrellaAttackState umbrellaAttackState);
	EUmbrellaAttackState GetUmbrellaAttackState() const;
	void OnUmbrellaAttackStateChanged();

private:
	void StartBouncing();
	void StopBouncing();

	void Rain();
	void StopRaining();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> _projectileClass;

	UPROPERTY(EditAnywhere)
	float _openUmbrellaAttackStateDuration = 10.0f;

	FTimerHandle _openUmbrellaAttackStateDurationTimerHandle;

	UPROPERTY(EditAnywhere)
	float _closedUmbrellaAttackStateDuration = 10.0f;

	FTimerHandle _closedUmbrellaAttackStateDurationTimerHandle;

	UPROPERTY(EditAnywhere)
	float _launchSpeed = 4000.0f;

	EUmbrellaAttackState _umbrellaAttackState = EUmbrellaAttackState::None;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* _projectileMovementComponent;

	UPROPERTY(EditAnywhere)
	FVector _bounceOffset = FVector(10.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	float _rainMaxXPosition = 80.0f;

	UPROPERTY(EditAnywhere)
	float _rainStartingZPosition = 100.0f;

	UPROPERTY(EditAnywhere)
	float _delayBetweenRaindrop = 0.1f;

	float _raindropTimer = 0.0f;
};
/*----------------------------------------------------------------------------------------------------*/