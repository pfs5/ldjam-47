// ----------------------------------------------------------------------------
//
// DWYW (Do What You Want) license 2020
// 
// ----------------------------------------------------------------------------
#include "AudioManager.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "../GentlemansWorldSettings.h"
// ----------------------------------------------------------------------------
AAudioManager::AAudioManager()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Music Audio Component"));
	_audioComponent->SetupAttachment(RootComponent);

	_rainAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Rain Audio Component"));
	_rainAudioComponent->SetupAttachment(RootComponent);
}
// ----------------------------------------------------------------------------
void AAudioManager::StartRainSound()
{

}
// ----------------------------------------------------------------------------
void AAudioManager::StopRainSound()
{

}
// ----------------------------------------------------------------------------
void AAudioManager::RestartAudio()
{
	_audioComponent->Stop();
	_audioComponent->Play();
}
// ----------------------------------------------------------------------------
AAudioManager* GetAudioManager(AActor* worldContextObject)
{
	if (worldContextObject == nullptr)
	{
		return nullptr;
	}

	AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(worldContextObject->GetWorldSettings());
	if (ws == nullptr)
	{
		return nullptr;
	}

	return ws->GetAudioManager();
}
// ----------------------------------------------------------------------------