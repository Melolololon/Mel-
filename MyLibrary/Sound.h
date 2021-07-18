#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<string>
#include<unordered_map>
#include"SoundData.h"
using namespace Microsoft::WRL;



//�T�E���h��
struct PlaySoundData
{
	//�{�����[��(�p�[�Z���g)
	float volume = 50.0f;

};

//�T�u�~�b�N�X�{�C�X�Ń\�[�X�{�C�X���܂Ƃ߂ĊǗ��ł���炵��
//�u���@�F�\�[�X�{�C�X�R�[���o�b�N���g�p����v�ɃR�[���o�b�N���Ă΂ꂽ�Ƃ��Ƀ\�[�X�{�C�X��������肷�鏈�������Ă���?
//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--use-source-voice-callbacks

//�قڂ��ׂĂ�SE�𒲐��������Ƃ��Ƃ��͂ǂ����邩�B
//PlayLoadSound�łǂ̃T�u�~�b�N�X�{�C�X�ɒǉ����邩��I�ׂ�悤�ɂ���?
//�T�u�~�b�N�X�{�C�X������悤�ɂ���?�T�u�~�b�N�X�{�C�X���ƕ�����ɂ�����������Ȃ�����T�E���h�O���[�v�Ƃ��ɂ���?

//SoundData�̉������Đ�����N���X
class Sound
{
private:
	class XAudio2VoiceCallback :public IXAudio2VoiceCallback
	{
	public:
		HANDLE streamEndEvent;
		XAudio2VoiceCallback();
		~XAudio2VoiceCallback();

		STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

		//�w�肵�����[�v�����Đ�������Ă΂��
		STDMETHOD_(void, OnStreamEnd)(THIS);
		STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
		STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
		STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
		STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT errow) {};

	};
	XAudio2VoiceCallback voiceCallback;

	static std::unordered_map<std::string, std::unique_ptr<Sound>> pSounds;
	static std::vector<std::unique_ptr<Sound>>pNoneNameSounds;

	static IXAudio2* iXAudio2;
	//ComPtr<IXAudio2SourceVoice> pSourceVoice;
	IXAudio2SourceVoice* pSourceVoice;

	bool playEndMoment = false;

	bool Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name);
	void CheckPlayEnd();
public: 
	static const UINT32 LOOP_INFINITY = XAUDIO2_LOOP_INFINITE;
	static const UINT32 LOOP_MAX = XAUDIO2_MAX_LOOP_COUNT;

	static void Initialize(IXAudio2* pIXAudio2);
	static void Update();
	
	
	/// <summary>
	/// SoundData���Đ����܂��B����name�ɖ��O��n����Get�֐��ŃA�N�Z�X�ł���悤�ɂȂ�A�����̃f�[�^���ォ��ݒ肵����擾�����肷�邱�Ƃ��ł��܂��B
	/// </summary>
	/// <param name="soundData"></param>
	/// <param name="loopNum"></param>
	/// <param name="playSoundData"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static bool PlayLoadSound(SoundData* soundData,const UINT32 loopNum,const PlaySoundData& playSoundData, const std::string& name = "");
	
	static Sound* Get(const std::string& name) { return pSounds[name].get(); }
	
	/// <summary>
	/// �Đ����I�����܂��B
	/// </summary>
	/// <param name="name"></param>
	static void StopSound(std::string& name);

#pragma region �Z�b�g

	/// <summary>
	/// playSoundData�̃f�[�^��S�ăZ�b�g���܂��B
	/// </summary>
	/// <param name="playSoundData"></param>
	void SetPlaySoundData(const PlaySoundData& playSoundData);

	/// <summary>
	/// ���ʂ�ݒ肵�܂��B
	/// </summary>
	/// <param name="volume">����(0����100�̃p�[�Z���g)</param>
	void SetSoundVolume(const float volume);

	/// <summary>
	/// �ꎞ��~�t���O���Z�b�g���܂��B
	/// </summary>
	/// <param name="flag"></param>
	void SetSoundPauseFlag(const bool flag);


#pragma endregion

#pragma region �Q�b�g
	/// <summary>
	/// �Đ����I�������u�Ԃ�true��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	bool GetPlayEndMomentFlag() { return playEndMoment; }
#pragma endregion

};

