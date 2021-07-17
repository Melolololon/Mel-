#include "Sound.h"
IXAudio2* Sound::iXAudio2;

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

void Sound::Initialize(IXAudio2* pIXAudio2)
{
	iXAudio2 = pIXAudio2;
}

void Sound::PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	WAVEFORMATEX wfex = soundData->GetWaveFormatEX();
	pSourceVoice.emplace(name, std::make_unique<Sound>());
	auto result = iXAudio2->CreateSourceVoice(&pSourceVoice[name], &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result)
	{
#ifdef _DEBUG
		if (name == "") 
		{
			OutputDebugStringW(L"çƒê∂Ç…é∏îsÇµÇ‹ÇµÇΩÅB\n");
		}
		else 
		{
			OutputDebugStringA(name.c_str());
			OutputDebugStringW(L"ÇÃçƒê∂Ç…é∏îsÇµÇ‹ÇµÇΩÅB\n");
		}
#endif // _DEBUG

	}

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData->GetPBuffer();
	buf.pContext = soundData->GetPBuffer();
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData->GetBytes();
    buf.LoopCount = loopNum;

	result = pSourceVoice[name]->SubmitSourceBuffer(&buf);
	result = pSourceVoice[name]->Start();

	//PlaySoundDataÇÃê›íË
	pSourceVoice[name]->SetVolume(playSoundData.volume);

	//""ÇæÇ¡ÇΩÇÁìoò^ÇµÇ»Ç¢Ç©ÇÁè¡Ç∑
	if (name == "")pSourceVoice.erase(name);

	
}

