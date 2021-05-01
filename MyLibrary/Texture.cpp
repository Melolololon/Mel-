#include "Texture.h"
#include"CreateBuffer.h"
Texture::Texture(){}

Texture::~Texture(){}

bool Texture::LoadTexture(const std::string& texturePath)
{
	std::wstring texturePathW;
	texturePathW.resize(texturePath.size());

	MultiByteToWideChar
	(
		CP_ACP,
		0,
		texturePath.c_str(),
		-1,
		texturePathW.data(),
		texturePathW.size()
	);

	auto result = LoadFromWICFile
	(
		texturePathW.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&metadata,
		scratchImage
	);
	//�ǂݍ��ݎ��s
	if(result)
	{
		OutputDebugString(L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n");
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);


	CreateBuffer::GetInstance()->CreateTextureBuffer
	(
		metadata,
		image,
		&textureBuffer
	);
	return true;
}

