#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>

#include"Vector.h"
#include"Color.h"

//sprite�Ƀq�[�v��������?

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

	static std::unordered_map<std::string, std::unique_ptr<Texture*>>pTextures;

	static bool Load(const std::string& path, const std::string& name);
	static void Delete(const std::string& name);
	static Texture* Get(const std::string& name) { pTextures[name].get(); }

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

