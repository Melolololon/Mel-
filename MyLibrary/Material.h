#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>

#include"Values.h"
#include"Texture.h"

namespace MelLib
{
	//Materialを継承してパラメータを増やせるようにする
	//継承してもどうやってシェーダーに渡す?利用者にMapしてもらうしかない?
	//定数バッファを自由に作れるようにしたほうがいい?

	//マテリアル情報をまとめたクラス。
	struct Material
	{
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//環境光
		Value3<float>ambient = 0.2f;
		//拡散反射光
		Value3<float>diffuse = 0.7f;
		//鏡面反射光
		Value3<float>specular = 0.0f;
		//アルファ値
		float alpha = 1.0f;

	private:
		//テクスチャ
		std::vector<Texture*>pTextures;
		ComPtr<ID3D12DescriptorHeap>textureHeap;
	};
}
