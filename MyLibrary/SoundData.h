#pragma once
#include<string>
#include<unordered_map>
#include<memory>
#include<fstream>
#include<xaudio2.h>

//サウンドのデータ
class SoundData
{
private:
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


	static std::unordered_map < std::string, std::unique_ptr<SoundData> > pSoundDatas;


	//波形データ
	WAVEFORMATEX wfex;
	//バッファ
	char* pBuffer = nullptr;
	
	UINT32 bytes = 0;

public:
	static bool Load(const std::string& path,const std::string& name);
	static SoundData* Get(const std::string& name) { return pSoundDatas[name].get(); }
	static void Delete(const std::string& name);

	bool LoadSound(const std::string& path, const std::string& name);

	WAVEFORMATEX GetWaveFormatEX()const { return wfex; }
	char* GetPBuffer() { return pBuffer; }
	UINT32 GetBytes() { return bytes; }
};

