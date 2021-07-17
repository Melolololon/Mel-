#include "Sound.h"
IXAudio2* Sound::iXAudio2;

std::unordered_map<std::string, std::unique_ptr<Sound>> Sound::pSounds;
std::vector<std::unique_ptr<Sound>>Sound::pNoneNameSounds;
//
//class XAudio2VoiceCallback :public IXAudio2VoiceCallback
//{
//public:
//	HANDLE streamEndEvent;
//	XAudio2VoiceCallback() :streamEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)){}
//	~XAudio2VoiceCallback() { CloseHandle(streamEndEvent); }
//
//	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
//	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
//	
//	//�w�肵�����[�v�����Đ�������Ă΂��
//	STDMETHOD_(void, OnStreamEnd)(THIS) 
//	{
//		SetEvent(streamEndEvent); };
//	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
//	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT errow) {};
//
//};
//XAudio2VoiceCallback voiceCallback;

bool Sound::Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	WAVEFORMATEX wfex = soundData->GetWaveFormatEX();

	auto result = iXAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result)
	{
#ifdef _DEBUG
		if (name == "")
		{
			OutputDebugStringW(L"�Đ��Ɏ��s���܂����B\n");
		}
		else
		{
			OutputDebugStringA(name.c_str());
			OutputDebugStringW(L"�̍Đ��Ɏ��s���܂����B\n");
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

void Sound::Update()
{

#pragma region �Đ��I�����̏���


	//std::vector<std::string>eraseSoundName;
	for(auto& s: pSounds)
	{
		s.second->playEndMoment = false;
		s.second->CheckPlayEnd();
		if(s.second->playEndMoment)
		{
			s.second->pSourceVoice->Stop();
			s.second->pSourceVoice->DestroyVoice();
			//eraseSoundName.push_back(s.first);
		}
	}
	//for(const auto& s : eraseSoundName)
	//{
	//	pSounds.erase(s);
	//}


	for (int i = 0, size = pNoneNameSounds.size(); i < size; i++)
	{
		pNoneNameSounds[i]->playEndMoment = false;
		pNoneNameSounds[i]->CheckPlayEnd();
		if(pNoneNameSounds[i]->playEndMoment)
		{
			pNoneNameSounds[i]->pSourceVoice->Stop();
			pNoneNameSounds[i]->pSourceVoice->DestroyVoice();
			pNoneNameSounds.erase(pNoneNameSounds.begin() + i);
		}
	}
#pragma endregion

}

void Sound::CheckPlayEnd()
{
	//WaitForSingleObjectEx(�C�x���g,����1�̃C�x���g��SetEvent�ɓn�����܂łɑҋ@���鎞��,)
	DWORD res = WaitForSingleObjectEx(voiceCallback.streamEndEvent, 0, TRUE);
	if (res == WAIT_OBJECT_0)playEndMoment = true;
	else playEndMoment = false;

	
}

bool Sound::PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name)
{
	Sound* pSound = nullptr;
	if (name == "")
	{
		pNoneNameSounds.push_back(std::make_unique<Sound>());
		pSound = pNoneNameSounds[pNoneNameSounds.size() - 1].get();
	}
	else 
	{
		pSounds.emplace(name, std::make_unique<Sound>());
		pSound = pSounds[name].get();
	}

	
	if (!pSound->Play(soundData, loopNum, playSoundData, name))return false;

	//PlaySoundData�̐ݒ�
	pSound->SetSoundVolume(playSoundData.volume);


	
	return true;
}

void Sound::SetSoundVolume(const float volume)
{
	float volumePar = volume;
	if (volumePar > 100.0f)volumePar = 100.0f;
	else if (volumePar < 0.0f)volumePar = 0.0f;

	pSourceVoice->SetVolume(volumePar / 100.0f);
	
}

void Sound::SetSoundPauseFlag(const bool flag)
{
	if (flag)pSourceVoice->Stop();
	else pSourceVoice->Start();
}

Sound::XAudio2VoiceCallback::XAudio2VoiceCallback() :streamEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL))
{
}

Sound::XAudio2VoiceCallback::~XAudio2VoiceCallback()
{
	CloseHandle(streamEndEvent);
}

STDMETHODIMP_(void __stdcall) Sound::XAudio2VoiceCallback::OnStreamEnd(void)
{
	SetEvent(streamEndEvent);
}
