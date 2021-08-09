#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>
#include<tuple>

#include"Color.h"
#include"Values.h"
#include"Texture3D.h"

namespace MelLib
{


	//Materialを継承してパラメータを増やせるようにする
	//継承してもどうやってシェーダーに渡す?利用者にMapしてもらうしかない?
	//定数バッファを自由に作れるようにしたほうがいい?

	//マテリアル情報の定数バッファをマテリアルクラスに持たせる。
	//そうすれば、モデルに複数のマテリアル情報持たせる必要がない。
	//マテリアルの基底クラス作ってそこに定数バッファ持たせないといけない
	
	//ヒープはどこに持たせる?
	//テクスチャバッファ以外に持たせるものありそうだったらモデルに

	//ModelData生成したり読み込んだら、自動で初期マテリアル作る。

	//マテリアルのデータの構造体を作ったから、
	//マテリアル構造体にテンプレート使ったマテリアル情報持たせる?読み取れる?マップは読み取れなくてもできる?
	//配列にまとめられないから分ける
	//ライブラリで定義されてるものだけ内部で管理する仕様にする?
	//利用者が用意したマテリアルは自分で管理してもらう
	//テンプレート無理そうだからわける
	
	//マテリアルの型に合わせてパイプライン生成時に使用するシェーダー変える?
	//マテリアルにパイプライン持たせる?
	//パイプラインにレンダリングの情報(カリングとかレンダーターゲット数)とかあって、
	//マテリアルはあくまで見た目の情報を持つやつだからいらん。
	
	//マテリアルのクラス指定すると、そのマテリアルに適したシェーダーでパイプラインを作る機能はあってもいいかも
	//マテリアルごとに定数バッファとテクスチャバッファの数変わるから、ルートシグネチャ持たせちゃう? 
	//テンプレートのやつCreate呼べない。
	//継承して作ってもらう?

	//とりあえず作るの進めよう

	//マテリアル情報をまとめたクラス。
	//template<class MTL>
	class Material
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//static std::unordered_map<std::string, Material<ADSAMaterialData>*>pAdsaMaterials;
		//static std::unordered_map<std::string, Material<PBRMaterialData>*>pPbrMaterials;
		

		//MTL materialData;

		//マテリアルの定数バッファ
		ComPtr<ID3D12Resource>materialBuffer;

		//(マテリアルのカラー + AddColor + SubColor)*MulColor = 追加色
		Color color;
		//テクスチャ
		Texture* pTextures;
		Texture* pNormalMapTextures;
		Texture3D* pTexture3Ds;

		//テクスチャクラスに持たせるため、コメントアウト
		//ComPtr<ID3D12Resource>textureBuffer;
		
		//どこに持たせるのがベストかわからないため、保留
		//ComPtr<ID3D12DescriptorHeap>textureHeap;

	protected:
		/// <summary>
		/// マテリアルのバッファとか生成。
		/// </summary>
		/// <param name="mtlByte">構造体のバイト数</param>
		void Create(const size_t& mtlByte);

		void MapMaterialData(void** pData);
		void UnmapMaterialData();
	public:

		//materialDataの型チェックして、umapに格納
		/*static void Create();
		static Material<MTL>* Get(const std::string& name);
		static void Delete();*/

		ID3D12Resource* GetPConstBuffer()const { return materialBuffer.Get(); }
		//MTL GetMaterialData(MTL data) const {  return materialData; }

		void SetColor(const Color& color) { this->color = color; }
		void SetTexture(Texture* pTex) { pTextures = pTex; }
		void SetNormalMapTexture(Texture* pNormalMapTex) { pNormalMapTex = pNormalMapTex; }
		void SetTexture3D(Texture3D* pTex) { pTexture3Ds = pTex; }

		//void SetMaterialData(MTL data) { materialData = data; }
	};

#pragma region Template

	//Create関数呼べない

//	//利用者マテリアルクラス
//	template<class MTL>
//	class TmpMaterial :public Material
//	{
//	private:
//		MTL materialData;
//
//	public:
//#pragma region セット
//		void SetMaterialData(const MTL& data) { materialData = data; }
//#pragma endregion
//
//#pragma region ゲット
//		MTL GetMaterialData() { return materialData; }
//#pragma endregion
//
//	};
//#pragma endregion


#pragma region ADSA



	struct ADSAMaterialData
	{
		//環境光
		Value3<float>ambient = 0.2f;
		//拡散反射光
		Value3<float>diffuse = 0.7f;
		//鏡面反射光
		Value3<float>specular = 0.0f;
		//アルファ値
		float alpha = 1.0f;
	};


	//Ambient、Diffuse、Supecular、モデルから読み取ったAlphaを所持したマテリアル
	class ADSAMaterial :public Material
	{
	private:
		ADSAMaterialData materialData;

	public:

		void Map();

#pragma region セット
		void SetMaterialData(const ADSAMaterialData& data) { materialData = data; }
#pragma endregion

#pragma region ゲット
		ADSAMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};
#pragma endregion

#pragma region PBR

	struct PBRMaterialData
	{
		Color baseColor;

		//金属度(0または1)
		float metalness = 0.0f;
		//鏡面反射光(float 0から1)
		float fSpecular = 0.5f;
		//粗さ
		float roughness = 0.0f;
	};

	//PBRマテリアル
	class PBRMaterial:public Material
	{
	private:
		PBRMaterialData materialData;

	public:
		void Map();
#pragma region セット
		void SetMaterialData(const PBRMaterialData& data) { materialData = data; }
#pragma endregion

#pragma region ゲット
		PBRMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};

#pragma endregion


}
