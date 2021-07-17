#include "Sound.h"
IXAudio2* Sound::iXAudio2;

std::unordered_map<std::string, std::unique_ptr<Sound>> Sound::pSounds;

class XAudio2VoiceCallback :public IXAudio2VoiceCallback
{
public:
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT) {};

};
XAudio2VoiceCallback voiceCallback;

bool Sound::Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	WAVEFORMATEX wfex = soundData->GetWaveFormatEX();

	auto result = iXAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result)
	{
#ifdef _DEBUG
		if (name == "")
		{
			OutputDebugStringW(L"Ä¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
		}
		else
		{
			OutputDebugStringA(name.c_str());
			OutputDebugStringW(L"‚ÌÄ¶‚ÉŽ¸”s‚µ‚Ü‚µ‚½B\n");
		}
#endif // _DEBUG
		return false;
	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData->GetPBuffer();
	buf.pContext = soundData->GetPBuffer();
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData->GetBytes();
	buf.LoopCount = loopNum;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();


	return true;
}

void Sound::Initialize(IXAudio2* pIXAudio2)
{
	iXAudio2 = pIXAudio2;
}

bool Sound::PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	pSounds.emplace(name, std::make_unique<Sound>());
	if (!pSounds[name]->Play(soundData, loopNum, playSoundData, name))return false;

	//PlaySoundData‚ÌÝ’è
	pSounds[name]->SetSoundVolume(playSoundData.volume);

	//""‚¾‚Á‚½‚ç“o˜^‚µ‚È‚¢‚©‚çÁ‚·
	if (name == "")pSounds.erase(name);
	
	return true;
}

void Sound::SetSoundVolume(const float volume)
{
	float volumePar = volume;
	if (volumePar > 100.0f)volumePar = 100.0f;
	else if (volumePar < 0.0f)volumePar = 0.0f;

	pSourceVoice->SetVolume(volumePar / 100.0f);
	
}

