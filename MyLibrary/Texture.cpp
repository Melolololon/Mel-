#include "Texture.h"
#include"CreateBuffer.h"
#include"Sprite.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Texture>>Texture::pTextures;
std::vector<UINT>Texture::eraseTextureNumber;
UINT Texture::loadTextureNumber = 1;


bool Texture::LoadTexture(const std::vector<std::string>& path)
{
	size_t pathStrNum = path.size();


	scratchImage.resize(pathStrNum);
	image.resize(pathStrNum);
	
	for (size_t i = 0; i < pathStrNum; i++)
	{
		std::wstring texturePathW;
		texturePathW.resize(path[i].size());
		MultiByteToWideChar
		(
			CP_ACP,
			0,
			path[i].c_str(),
			-1,
			texturePathW.data(),
			texturePathW.size()
		);


		auto result = LoadFromWICFile
		(
			texturePathW.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&metadata,
			scratchImage[i]
		); 
		
		if (result != S_OK) return false;
	}

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
	std::vector<std::string>path(1, texturePath);
	auto result = LoadTexture(path);
	//“Ç‚İ‚İ¸”s
	if (!result)
	{
#ifdef _DEBUG
		OutputDebugString(L"ƒ‚ƒfƒ‹‚ÌƒeƒNƒXƒ`ƒƒ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½B\n");
		return false;
#endif // _DEBUG
	}

	image[0] = scratchImage[0].GetImage(0, 0, 0);


	return true;
}

bool Texture::LoadSpriteTexture(const std::string& texturePath)
{
	std::vector<std::string>path(1, texturePath);
	auto result = LoadTexture(path);
	//“Ç‚İ‚İ¸”s
	if (!result)
	{
#ifdef _DEBUG
		OutputDebugStringA(texturePath.c_str());
		OutputDebugString(L"‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½B\n");
#endif // _DEBUG
		return false;
	}

	image[0] = scratchImage[0].GetImage(0, 0, 0);

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
