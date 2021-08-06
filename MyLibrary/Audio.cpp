#include "Audio.h"
#include"Sound.h"


using namespace MelLib;

Audio* Audio::GetInstance()
{
	static Audio a;
	return &a;
}

void Audio::Initialize()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudio2->CreateMasteringVoice(&masterVoice);

	Sound::Initialize(xAudio2.Get());
}
