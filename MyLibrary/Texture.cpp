#include "Texture.h"
#include"CreateBuffer.h"
#include"Sprite.h"

UINT Texture::loadTextureNumber = 0;

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
	if (result != S_OK)
		return false;
	return true;
}

bool Texture::LoadModelTexture(const std::string& texturePath)
{
	auto result = LoadTexture(texturePath);
	//読み込み失敗
	if(!result)
	{
		OutputDebugString(L"モデルのテクスチャの読み込みに失敗しました。\n");
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);


	return true;
}

bool Texture::LoadSpriteTexture(const std::string& texturePath)
{
	auto result = LoadTexture(texturePath);
	//読み込み失敗
	if (!result)
	{
		OutputDebugString(L"スプライトのテクスチャの読み込みに失敗しました。\n");
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);

	textureNumber = loadTextureNumber;
	loadTextureNumber++;
	Sprite::CreateTextureBuffer(this);

	return true;
}
