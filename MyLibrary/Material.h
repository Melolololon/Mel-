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

		static ID3D12Device* device;

		//static std::unordered_map<std::string, Material<ADSAMaterialData>*>pAdsaMaterials;
		//static std::unordered_map<std::string, Material<PBRMaterialData>*>pPbrMaterials;
		

		//MTL materialData;

		//マテリアルの定数バッファ
		ComPtr<ID3D12Resource>materialBuffer = nullptr;

		//(マテリアルのカラー + AddColor + SubColor)*MulColor = 追加色
		Color color;
		//テクスチャ
		static const UINT TEXTURE_HANDLE_NUM = 0;
		Texture* pTexture = nullptr;
		static const UINT NORMAL_MAP_HANDLE_NUM = 1;
		Texture* pNormalMapTexture = nullptr;
		static const UINT TEXTURE_3D_HANDLE_NUM = 2;
		Texture3D* pTexture3D = nullptr;

		//テクスチャクラスに持たせるため、コメントアウト
		//ComPtr<ID3D12Resource>textureBuffer;
		
		//どこに持たせるのがベストかわからないため、保留
		//とりあえずマテリアルに持たせる(モデルごとに用意するのはもったいない)
		//UAVをテクスチャと同じヒープにまとめる必要が出てきたら、
		//コンピュートシェーダーを使うオブジェクトのみ専用のヒープ作る?
	    //ここに作ると複数のマテリアルのテクスチャをまとめられない
		//ファイルに複数モデルあってもレンダリングするタイミングが違うからまとめなくても問題ない
		ComPtr<ID3D12DescriptorHeap>textureHeap;

	protected:
		/// <summary>
		/// マテリアルのバッファとか生成。
		/// </summary>
		/// <param name="mtlByte">構造体のバイト数</param>
		void CreateBufferAndDescriptorHeap(const size_t& mtlByte);

		void MapMaterialData(void** pData);
		void UnmapMaterialData();
	public:

		static void Initialize(ID3D12Device* dev)
		{
			device = dev;
		}

		//materialDataの型チェックして、umapに格納
		/*static void Create();
		static Material<MTL>* Get(const std::string& name);
		static void Delete();*/

		virtual void Create() = 0;

		ID3D12DescriptorHeap* GetPTextureHeap() { return textureHeap.Get(); }
		ID3D12Resource* GetPConstBuffer()const { return materialBuffer.Get(); }
		Texture* GetTexture() { return pTexture; }
		Texture* GetNormalTexture() { return pNormalMapTexture; }
		Texture3D* GetTexture3D() { return pTexture3D; }
		//MTL GetMaterialData(MTL data) const {  return materialData; }

		void SetColor(const Color& color) { this->color = color; }
		void SetTexture(Texture* pTex);
		void SetNormalMapTexture(Texture* pNormalMapTex) { pNormalMapTex = pNormalMapTex; }
		void SetTexture3D(Texture3D* pTex) { pTexture3D = pTex; }

		//void SetMaterialData(MTL data) { materialData = data; }

	};


#pragma region ADSA



	struct ADSAMaterialData
	{
		//環境光
		Value4<float>ambient = 0.3f;
		//拡散反射光
		Value4<float>diffuse = 0.7f;
		//鏡面反射光
		Value4<float>specular = 0.0f;
		//アルファ値
		float alpha = 1.0f;
	};


	//Ambient、Diffuse、Supecular、モデルから読み取ったAlphaを所持したマテリアル
	class ADSAMaterial :public Material
	{
	private:
		//static std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>>pMaterials;

		ADSAMaterialData materialData;

	public:

		void Create()override;
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
		//アルベドとアルファ値
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
