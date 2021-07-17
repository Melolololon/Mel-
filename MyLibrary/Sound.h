#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<string>
#include<unordered_map>
#include"SoundData.h"
using namespace Microsoft::WRL;

static const BYTE PLAY_SOUND_LOOP_INFINITY = XAUDIO2_LOOP_INFINITE;
struct PlaySoundData
{
	//ボリューム(パーセント)
	float volume = 50.0f;

};

class Sound
{
private:

	static IXAudio2* iXAudio2;
	static std::unordered_map<std::string, ComPtr<IXAudio2SourceVoice>> pSourceVoice;
public:

	static void Initialize(IXAudio2* pIXAudio2);
	static void PlayLoadSound(SoundData* soundData,const UINT32 loopNum,const PlaySoundData& playSoundData, const std::string& name = "");
	static void StopPlaySound(std::string& name);
};

