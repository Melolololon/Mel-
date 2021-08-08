#pragma once
#include<d3d12.h>
#include<DirectXTex.h>

#include"Vector.h"
#include"Color.h"


//Textureにテクスチャバッファ持たせる?
//スプライトとかで同じテクスチャなのにバッファ毎回作るのもったいない
//

namespace MelLib
{

	class Texture
	{
	private:
		static std::unordered_map<std::string, std::unique_ptr<Texture>>pTextures;
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage scratchImage;
		const DirectX::Image* image;


		UINT textureNumber = 0;
		static UINT loadTextureNumber;

		bool LoadTexture(const std::string& texturePath);

		bool LoadSpriteTexture(const std::string& texturePath);
	public:
		Texture() {}
		~Texture() {}


		//空いたヒープを使用するために使用
		static std::vector<UINT>eraseTextureNumber;

		static bool Load(const std::string& path, const std::string& name);
		static void Delete(const std::string& name);
		static Texture* Get(const std::string& name) { return pTextures[name].get(); }

		/// <summary>
		/// モデルのテクスチャを読み込みます。
		/// </summary>
		/// <param name="texturePath"></param>
		/// <returns></returns>
		bool LoadModelTexture(const std::string& texturePath);



#pragma region データ取得

		Vector2 GetTextureSize()const { return { static_cast<float>(metadata.width) , static_cast<float>(metadata.height) }; }

#pragma endregion

#pragma region 開発者用関数


		DirectX::TexMetadata GetMetadata()const { return metadata; }
		const DirectX::Image* GetImage()const { return image; }
		UINT GetTextureNumber() { return textureNumber; }
#pragma endregion
	};

}
