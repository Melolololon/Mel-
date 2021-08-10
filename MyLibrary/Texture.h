#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>

#include"Vector.h"
#include"Color.h"


//Texture�Ƀe�N�X�`���o�b�t�@��������?
//�X�v���C�g�Ƃ��œ����e�N�X�`���Ȃ̂Ƀo�b�t�@������̂��������Ȃ�
//

namespace MelLib
{

	class Texture
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static std::unordered_map<std::string, std::unique_ptr<Texture>>pTextures;
		//DirectX::TexMetadata metadata;
		//DirectX::ScratchImage scratchImage;
		//const DirectX::Image* image;


		DirectX::TexMetadata metadata;
		std::vector< DirectX::ScratchImage> scratchImage;
		std::vector<const DirectX::Image*>image;


		UINT textureNumber = 0;
		static UINT loadTextureNumber;

	private:

		bool LoadSpriteTexture(const std::string& texturePath);

	protected:

		ComPtr<ID3D12Resource>textureBuffer;

	protected:
		/// <summary>
		/// DirectXTex�̊֐��Ń��^�f�[�^��ǂݍ��ރN���X�B
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool LoadTexture(const std::vector<std::string>& path);
	
	public:
		Texture() {}
		~Texture() {}


		//�󂢂��q�[�v���g�p���邽�߂Ɏg�p
		static std::vector<UINT>eraseTextureNumber;

		static bool Load(const std::string& path, const std::string& name);
		static void Delete(const std::string& name);
		static Texture* Get(const std::string& name) { return pTextures[name].get(); }

		/// <summary>
		/// ���f���̃e�N�X�`����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="texturePath"></param>
		/// <returns></returns>
		bool LoadModelTexture(const std::string& texturePath);



#pragma region �f�[�^�擾

		Vector2 GetTextureSize()const { return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) }; }

#pragma endregion

#pragma region �J���җp�֐�


		DirectX::TexMetadata GetMetadata()const { return metadata; }
		std::vector <const DirectX::Image*> GetImage()const { return image; }
		UINT GetTextureNumber() { return textureNumber; }
		ID3D12Resource* GetTextureBuffer() { return textureBuffer.Get(); }
#pragma endregion
	};

}
