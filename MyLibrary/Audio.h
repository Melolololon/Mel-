#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<unordered_map>
using namespace Microsoft::WRL;

struct WAVChunk
{
	char id[4];
	int32_t size;
};

struct WAVRiffHeader
{
	WAVChunk chunk;
	char type[4];
};

struct WAVFormatChunk
{
	WAVChunk chunk;
	WAVEFORMAT fmt;
};

class Audio
{
private:


	ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//�T�E���h��u_map
	std::unordered_map<std::string, IXAudio2SourceVoice*> pLoadSourceVoices;


public:
	Audio();
	~Audio();

	void playWave(const char* path);


	void loadSound(const char* path, std::string name, bool loop);
	void playLoadSound(std::string name);
	void stopLoadSound(std::string name,bool resetFlag);
};

