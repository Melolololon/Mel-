#include "SoundData.h"


bool SoundData::Load(const std::string& path,const std::string& name)
{
	pSoundDatas.emplace(name, std::make_unique<SoundData>());
	return pSoundDatas[name]->LoadSound(path, name);
}

void SoundData::Delete(const std::string& name)
{
}


bool SoundData::LoadSound(const std::string& path, const std::string& name)
{
#pragma region �t�@�C���I�[�v��
	std::ifstream file;
	file.open(path, std::ios_base::binary);

	if (file.fail())
	{

#ifdef _DEBUG
		OutputDebugStringA(path.c_str());
		OutputDebugStringW(L"�̓ǂݍ��݂Ɏ��s���܂����Bpath�̓��̓~�X�̉\��������܂��B\n");
		return false;
#endif // _DEBUG
	}
#pragma endregion

#pragma region �ǂݍ���
	WAVRiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		OutputDebugStringA(path.c_str());
		OutputDebugStringW(L"�̓ǂݍ��݂Ɏ��s���܂����B�t�@�C����RIFF�̕����񂪂���܂���ł����B\n");
		return false;
	}

	WAVFormatChunk format;
	file.read((char*)&format, sizeof(format));

	WAVChunk chunk;
	file.read((char*)&chunk, sizeof(chunk));
	bytes = chunk.size;
	pBuffer = new char[bytes];
	file.read(pBuffer, chunk.size);

	file.close();
#pragma endregion

#pragma region �g�`�t�H�[�}�b�g�ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
#pragma endregion

}
