// ----------------------------------------------------------------------------
//
// DWYW (Do What You Want) license 2020
// 
// ----------------------------------------------------------------------------
#include "AudioManager.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
// ----------------------------------------------------------------------------
AAudioManager::AAudioManager()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	_audioComponent->SetupAttachment(RootComponent);
}
// ----------------------------------------------------------------------------
void AAudioManager::RestartAudio()
{
	_audioComponent->Stop();
	_audioComponent->Play();
}
// ----------------------------------------------------------------------------