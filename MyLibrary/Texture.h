#pragma once
#include<DirectXTex.h>
#include"Vector.h"
class Texture
{
private:
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;
	const DirectX::Image* image;
public:
	Texture();
	~Texture();

	bool LoadTexture(const std::string& texturePath);

	Vector2 GetTextureSize()const
	{
		return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) };
	}

	DirectX::TexMetadata GetMetadata()const
	{
		return metadata;
	}
	const DirectX::Image* GetImage()const
	{
		return image;
	}

};

