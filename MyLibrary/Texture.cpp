#include "Texture.h"
#include"CreateBuffer.h"
#include"Sprite.h"

std::unordered_map<std::string, std::unique_ptr<Texture*>>Texture::pTextures;
UINT Texture::loadTextureNumber = 1;

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

bool Texture::Load(const std::string& path, const std::string& name)
{
	return false;
}

void Texture::Delete(const std::string& name)
{
	pTextures.erase(name);
}

bool Texture::LoadModelTexture(const std::string& texturePath)
{
	auto result = LoadTexture(texturePath);
	//�ǂݍ��ݎ��s
	if(!result)
	{
		OutputDebugString(L"���f���̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n");
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);


	return true;
}

bool Texture::LoadSpriteTexture(const std::string& texturePath)
{
	auto result = LoadTexture(texturePath);
	//�ǂݍ��ݎ��s
	if (!result)
	{
		OutputDebugString(L"�X�v���C�g�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n");
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);

	textureNumber = loadTextureNumber;
	loadTextureNumber++;
	Sprite::CreateTextureBuffer(this);

	return true;
}
