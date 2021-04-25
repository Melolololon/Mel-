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

	bool loadTexture(const std::string& texturePath);

	Vector2 getTextureSize()const
	{
		return { metadata.width ,metadata.height };
	}

	DirectX::TexMetadata getMetadata()const
	{
		return metadata;
	}
	const DirectX::Image* getImage()const
	{
		return image;
	}

};

