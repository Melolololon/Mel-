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
	//読み込み失敗
	if(result)
	{
		OutputDebugString(L"テクスチャの読み込みに失敗しました。\n");
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

