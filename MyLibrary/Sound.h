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

class Sound
{
private:

	static IXAudio2* iXAudio2;
	static std::unordered_map<std::string,std::unique_ptr<Sound>> pSounds;
	//ComPtr<IXAudio2SourceVoice> pSourceVoice;
	IXAudio2SourceVoice* pSourceVoice;

	bool Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name);
public:
	static const UINT32 LOOP_INFINITY = XAUDIO2_LOOP_INFINITE;
	static const UINT32 LOOP_MAX = XAUDIO2_MAX_LOOP_COUNT;

	static void Initialize(IXAudio2* pIXAudio2);
	static bool PlayLoadSound(SoundData* soundData,const UINT32 loopNum,const PlaySoundData& playSoundData, const std::string& name = "");
	static void StopPlaySound(std::string& name);

	/// <summary>
	/// 音量を設定します。
	/// </summary>
	/// <param name="volume">音量(0から100のパーセント)</param>
	void SetSoundVolume(const float volume);
};

