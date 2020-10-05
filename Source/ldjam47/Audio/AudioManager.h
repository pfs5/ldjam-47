// ----------------------------------------------------------------------------
//
// DWYW (Do What You Want) license 2020
// 
// ----------------------------------------------------------------------------
#pragma once
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"
// ----------------------------------------------------------------------------
class UAudioComponent;
// ----------------------------------------------------------------------------
/**
 * 
 */
UCLASS()
class LDJAM47_API AAudioManager : public AActor
{
	GENERATED_BODY()

public:
	AAudioManager();

	void StartRainSound();
	void StopRainSound();

	void RestartAudio();

private:
	UPROPERTY(EditAnywhere)
	UAudioComponent* _audioComponent;

	UPROPERTY(EditAnywhere)
	UAudioComponent* _rainAudioComponent;

};
// ----------------------------------------------------------------------------
AAudioManager* GetAudioManager(UObject* worldContextObject);
// ----------------------------------------------------------------------------