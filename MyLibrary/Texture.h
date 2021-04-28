#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>
#include"Vector.h"


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

	bool LoadTexture(const std::string& texturePath);

#pragma region �f�[�^�擾

	Vector2 GetTextureSize()const{return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) };}

#pragma endregion

#pragma region �J���җp�֐�


	DirectX::TexMetadata GetMetadata()const{return metadata;}
	const DirectX::Image* GetImage()const{return image;}
	ID3D12Resource* GetPTextureBuffer() { return textureBuffer.Get(); }

#pragma endregion
};

