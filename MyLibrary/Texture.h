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

	ComPtr<ID3D12Resource>textureBuffer;
public:
	Texture();
	~Texture();

	/// <summary>
	/// テクスチャを読み込みます。
	/// </summary>
	/// <param name="texturePath"></param>
	/// <returns></returns>
	bool LoadTexture(const std::string& texturePath);

	/// <summary>
	/// 単色べた塗りテクスチャを生成します。
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	bool CreateOneColorTexture(const Color& color);

#pragma region データ取得

	Vector2 GetTextureSize()const{return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) };}

#pragma endregion

#pragma region 開発者用関数


	DirectX::TexMetadata GetMetadata()const{return metadata;}
	const DirectX::Image* GetImage()const{return image;}
	ID3D12Resource* GetPTextureBuffer() { return textureBuffer.Get(); }

#pragma endregion
};

