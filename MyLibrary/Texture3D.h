#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<vector>

namespace MelLib 
{
	//奥方向の情報を持つテクスチャ
	class Texture3D
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static std::unordered_map<std::string, std::unique_ptr<Texture3D>>pTextures;
		
		DirectX::TexMetadata metadata;
		std::vector< DirectX::ScratchImage> scratchImage;
		std::vector<const DirectX::Image*>image;

		ComPtr<ID3D12Resource>textureBuffer;
	public:

		static bool Load(const std::vector<std::string>& path, const std::string& name);
		static void Delete(const std::string& name);
		static Texture3D* Get(const std::string& name) { return pTextures[name].get(); }

	};
}

