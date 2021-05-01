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
	/// ���f���̃e�N�X�`����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="texturePath"></param>
	/// <returns></returns>
	bool LoadModelTexture(const std::string& texturePath);

	bool LoadSpriteTexture(const std::string& texturePath);


#pragma region �f�[�^�擾

	Vector2 GetTextureSize()const{return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) };}

#pragma endregion

#pragma region �J���җp�֐�


	DirectX::TexMetadata GetMetadata()const{return metadata;}
	const DirectX::Image* GetImage()const{return image;}
	UINT GetTextureNumber() { return textureNumber; }
#pragma endregion
};

