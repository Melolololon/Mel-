#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>

#include"Vector.h"
#include"Color.h"

using namespace Microsoft::WRL;
class Texture
{
private:
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;
	const DirectX::Image* image;


	UINT textureNumber;
	static UINT loadTextureNumber;

	bool LoadTexture(const std::string& texturePath);

public:
	Texture();
	~Texture();

	/// <summary>
	/// モデルのテクスチャを読み込みます。
	/// </summary>
	/// <param name="texturePath"></param>
	/// <returns></returns>
	bool LoadModelTexture(const std::string& texturePath);

	bool LoadSpriteTexture(const std::string& texturePath);


#pragma region データ取得

	Vector2 GetTextureSize()const{return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) };}

#pragma endregion

#pragma region 開発者用関数


	DirectX::TexMetadata GetMetadata()const{return metadata;}
	const DirectX::Image* GetImage()const{return image;}
	UINT GetTextureNumber() { return textureNumber; }
#pragma endregion
};

