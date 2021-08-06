#include "Texture.h"
#include"CreateBuffer.h"
#include"Sprite.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Texture>>Texture::pTextures;
std::vector<UINT>Texture::eraseTextureNumber;
UINT Texture::loadTextureNumber = 1;


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


	if (result != S_OK) return false;
	return true;
}

bool Texture::Load(const std::string& path, const std::string& name)
{
	pTextures.emplace(name, std::make_unique<Texture>());
	return pTextures[name]->LoadSpriteTexture(path);

}

void Texture::Delete(const std::string& name)
{
	eraseTextureNumber.push_back(pTextures[name]->GetTextureNumber());
	pTextures.erase(name);
}

bool Texture::LoadModelTexture(const std::string& texturePath)
{
	auto result = LoadTexture(texturePath);
	//�ǂݍ��ݎ��s
	if (!result)
	{
#ifdef _DEBUG
		OutputDebugString(L"���f���̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n");
		return false;
#endif // _DEBUG
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
#ifdef _DEBUG
		OutputDebugStringA(texturePath.c_str());
		OutputDebugString(L"�̓ǂݍ��݂Ɏ��s���܂����B\n");
#endif // _DEBUG
		return false;
	}

	image = scratchImage.GetImage(0, 0, 0);

	if (eraseTextureNumber.size() == 0)
	{
		textureNumber = loadTextureNumber;
		loadTextureNumber++;
	}
	else
	{
		textureNumber = eraseTextureNumber[eraseTextureNumber.size() - 1];
		eraseTextureNumber.pop_back();
	}

	Sprite::CreateTextureBuffer(this);

	return true;
}
