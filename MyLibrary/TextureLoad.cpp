#include "TextureLoad.h"



BMPData TextureLoad::loadBMP(const char* texturepath)
{
	FILE* texture;
	fopen_s(&texture, texturepath, "rb");
	//�p�X�Ńt�@�C���`�����m�F����
	int size = strlen(texturepath);

	if (texturepath[size - 1] != 'p')
	{
		//�߂�l��void�ɂ��āA�\���͈̂����Ŏ󂯎��悤�ɂ���
		BMPData null;
		return null;
	}

	BMPData bmpData;
	fread(&bmpData, sizeof(bmpData) - 16, 1, texture);

	if (bmpData.bit == 32)
	{
		unsigned char sig[84];
		fread(&sig, sizeof(sig), 1, texture);
	}

	bmpData.rgbaData.resize(bmpData.height * bmpData.width);
	fread(bmpData.rgbaData.data(), bmpData.rgbaData.size() * 4, 1, texture);

	std::reverse(bmpData.rgbaData.begin(), bmpData.rgbaData.end());
	for (int i = 0; i < (int)bmpData.height; i++)
	{
		std::reverse(bmpData.rgbaData.begin() + bmpData.width * i, bmpData.rgbaData.begin() + bmpData.width * i + bmpData.width);
	}

	fclose(texture);


	return bmpData;
	
}
