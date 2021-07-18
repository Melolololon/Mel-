#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<string>
#include<unordered_map>
#include"SoundData.h"
using namespace Microsoft::WRL;



//サウンドの
struct PlaySoundData
{
	//ボリューム(パーセント)
	float volume = 50.0f;

};

//サブミックスボイスでソースボイスをまとめて管理できるらしい
//「方法：ソースボイスコールバックを使用する」にコールバックが呼ばれたときにソースボイス解放したりする処理書いてある?
//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--use-source-voice-callbacks

//ほぼすべてのSEを調整したいときとかはどうするか。
//PlayLoadSoundでどのサブミックスボイスに追加するかを選べるようにする?
//サブミックスボイスも作れるようにする?サブミックスボイスだと分かりにくいかもしれないからサウンドグループとかにする?

//SoundDataの音声を再生するクラス
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

		//指定したループ数分再生したら呼ばれる
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
	/// SoundDataを再生します。引数nameに名前を渡すとGet関数でアクセスできるようになり、音声のデータを後から設定したり取得したりすることができます。
	/// </summary>
	/// <param name="soundData"></param>
	/// <param name="loopNum"></param>
	/// <param name="playSoundData"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static bool PlayLoadSound(SoundData* soundData,const UINT32 loopNum,const PlaySoundData& playSoundData, const std::string& name = "");
	
	static Sound* Get(const std::string& name) { return pSounds[name].get(); }
	
	/// <summary>
	/// 再生を終了します。
	/// </summary>
	/// <param name="name"></param>
	static void StopSound(std::string& name);

#pragma region セット

	/// <summary>
	/// playSoundDataのデータを全てセットします。
	/// </summary>
	/// <param name="playSoundData"></param>
	void SetPlaySoundData(const PlaySoundData& playSoundData);

	/// <summary>
	/// 音量を設定します。
	/// </summary>
	/// <param name="volume">音量(0から100のパーセント)</param>
	void SetSoundVolume(const float volume);

	/// <summary>
	/// 一時停止フラグをセットします。
	/// </summary>
	/// <param name="flag"></param>
	void SetSoundPauseFlag(const bool flag);


#pragma endregion

#pragma region ゲット
	/// <summary>
	/// 再生が終了した瞬間にtrueを返します。
	/// </summary>
	/// <returns></returns>
	bool GetPlayEndMomentFlag() { return playEndMoment; }
#pragma endregion

};

