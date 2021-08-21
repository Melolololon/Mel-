#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"
#include"CollisionType.h"
#include"Material.h"
//AddColorとかまとめよう!

namespace MelLib
{
	
	//モデルの座標などをまとめたもの
	class ModelObject
	{

	private:

		static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;
	
		//[モデル内のオブジェクトごと]
		std::vector<Material*>materials;

#pragma region ボーンとアニメーションの情報
		static const UINT BONE_MAX = 64;
		struct SkinConstBufferData
		{
			DirectX::XMMATRIX bones[BONE_MAX];
		};

		//fbxモデルのアニメーション用の情報をまとめたもの
		struct FbxAnimationData
		{
			ModelData::FbxAnimationTimes animationTimes;
			FbxTime currentTime;
			int timeMag = 1;
		};


		//[ボーンごと]
		std::vector<BoneData>boneDatas;
		std::vector<ParentBoneData> parentBoneDatas;//親ボーンと影響度

		FbxAnimationData fbxAnimationData;
		bool isAnimation = false;
#pragma endregion

		//定数バッファ
		static const int CONST_BUFFER_REGISTER = 0;
		std::vector<ComPtr<ID3D12Resource>> constBuffer;//メイン(基本的な情報)

		static const int MATERIAL_BUFFER_REGISTER = 2;

		static const int COLOR_MATERIAL_BUFFER_REGISTER = 4;

		static const int USER_BUFFER_REGISTER = 1;
		std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//ユーザー
		ConstBufferData userConstBufferData;

		static const int MODEL_BUFFER_REGISTER = 3;
		std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//モデル特有(アニメーション関係の情報)
		ConstBufferData modelConstBufferData;

		static const int TEXURE_ROOTPARAM_NUM = 5;

		//定数にセットする座標などの値
		//[モデル内のオブジェクト数]
		std::vector<ModelConstData> modelConstDatas;

		//モデル(クリエイトしたら割り当てる)
		ModelData* pModelData = nullptr;
		UINT modelFileObjectNum = 0;


	private:

		void CreateConstBuffer();

		void DrawCommonProcessing(const std::string& rtName);
		void MapConstData(const Camera* camera);
		void SetCmdList();

		void FbxAnimation();
	public:

		//nullptr渡される可能性を考えると、boolをreturnできるようにしたほうがいい?
		ModelObject() {}

		~ModelObject() {}

		static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);


		static ModelObject* Get(const std::string& name) { return pModelObjects[name].get(); }
		static void Delete(const std::string& name);

		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

#pragma region 生成

#pragma region モデルで生成
		ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData);
		static bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name);
		bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData);
#pragma endregion モデルで生成


#pragma region プリミティブモデル生成
	

#pragma endregion プリミティブモデル生成


#pragma endregion 生成

#pragma region メッシュカット
		/// <summary>
		/// モデルの切断(仮実装用の関数)
		/// </summary>
		void MeshCat();
#pragma endregion


#pragma region セット

#pragma region 操作見た目変更

#pragma region 操作


		void SetPosition(const Vector3& position);
		void SetScale(const Vector3& scale);
		void SetAngle(const Vector3& angle);


#pragma endregion

#pragma region アニメーション

		void SetAnimationFlag(const bool flag) { isAnimation = flag; }

		/// <summary>
	/// アニメーションをリセットします。
	/// </summary>
		void ResetAnimation();

		void SetCurrentFream(const UINT fream);

		void SetAnimationSpeedMagnification(const int magnification) { fbxAnimationData.timeMag = magnification; }
#pragma endregion


#pragma endregion

		void SetMaterial(Material* mtl, const int index);
#pragma endregion

#pragma region ゲット

		//コンピュートシェーダーで計算したほうがいい。
		//できそうなら描画時に頂点シェーダーで計算した結果を持ってきたほうがいい?
		/// <summary>
		/// 頂点座標を取得します。
		/// </summary>
		/// <param name="scaleImpact"></param>
		/// <param name="angleImpact"></param>
		/// <param name="transformImpact"></param>
		/// <param name=""></param>
		/// <returns></returns>
		std::vector<std::vector<Vector3>>GetVerticesData
		(
			const bool scaleImpact,
			const bool angleImpact,
			const bool transformImpact
		);


#pragma endregion


	};
}
