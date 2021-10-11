#pragma once

#include<memory>
#include<vector>
#include<array>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"
#include"CollisionType.h"

#include"Material.h"

#include<fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")
#else
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")
#endif // _DEBUG

namespace MelLib
{
	struct Node
	{
		std::string nodeName;

		DirectX::XMVECTOR scaling = { 1,1,1,0 };
		DirectX::XMVECTOR rotation = { 1,1,1,0 };
		DirectX::XMVECTOR translation = { 1,1,1,0 };

		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX globalTransform;

		Node* parentNode;
	};


	//objとかがfbxのデータ持ってるのメモリもったいないから、
	//構造体でまとめて、fbxの時だけnewでメモリ確保するようにすればもったいなくない

	//モデルの頂点、インデックス、テクスチャをまとめたクラス
	class ModelData
	{
	public:
	
		enum class ModelFormat
		{
			MODEL_FORMAT_NONE,
			MODEL_FORMAT_PRIMITIVE,
			MODEL_FORMAT_OBJ,
			MODEL_FORMAT_FBX,
		};


#pragma region fbx構造体
		//fbxのボーン情報をまとめたもの
		struct FbxBone
		{
			//ボーン名
			std::string boneName;
			//初期姿勢の逆行列
			DirectX::XMMATRIX invInitialPose;
			//ボーン情報
			FbxCluster* fbxCluster;

			FbxBone(const std::string& name)
			{
				this->boneName = name;
			}
		};

		//fbxのアニメーションに必要なFbxTimeをまとめた構造体
		struct FbxAnimationTimes
		{
			FbxTime startTime;
			FbxTime endTime;
			//1フレームの時間
			FbxTime freamTime;
		};

#pragma endregion


	private:

#pragma region obj構造体

		//objのボーン情報をまとめたもの
		struct ObjBone
		{
			std::vector<Vector3>bonePosition;
		};

#pragma endregion

#pragma region fbx構造体


		//fbxのデータをまとめたもの
		struct FbxData
		{
			FbxScene* fbxScene = nullptr;

			//モデルのノード
			std::vector<Node>nodes;

			//メッシュを持つノード
			Node* meshNode = nullptr;

			std::vector<FbxBone>bones;

			FbxAnimationTimes animationTimes;

		};


#pragma endregion

		//モデルを格納する配列
		static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;
		//プリミティブモデルを格納する配列
		static std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>pPrimitiveModelDatas;

		//プリミティブモデル、マテリアルがnullptrのモデルデータに割り当てられるマテリアル
		static std::unique_ptr<ADSAMaterial>defaultMaterial;


		static ID3D12Device* device;

#pragma region バッファ_ヒープ

		//頂点
		std::vector<VertexBufferSet> vertexBufferSet;
		//インデックス
		std::vector <IndexBufferSet> indexBufferSet;
		ComPtr<ID3D12DescriptorHeap>textureDescHeap;
		std::vector<ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

		//一斉削除対象フラグ
		bool batchDeletionFlag = false;

#pragma region モデル情報
		ModelFormat modelFormat = ModelFormat::MODEL_FORMAT_NONE;

		//モデル名
		std::string modelName;

		//マテリアルにセットするテクスチャ
		std::vector<std::unique_ptr<Texture>>pTexture;
		//マテリアル
		std::vector<std::unique_ptr<ADSAMaterial>> material;

		//テクスチャを使わない場合の色
		Color color;

		//モデルファイルに何個モデルがあるか
		UINT modelFileObjectNum = 1;
		std::vector<std::vector<FbxVertex>>vertices;
		//スムーズシェーディング用法線
		std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;
		//上下左右前後の最高(最低)座標の値(上下左右前後の順に格納)
		std::vector<std::array<float, 6>>directionMaxPos;

		std::vector<std::vector<USHORT>> indices;

		UINT boneNum = 0;

		ObjBone objData;
		FbxData fbxData;

#pragma endregion

#pragma region 頂点

		/// <summary>
		/// 頂点バッファ、ビューの生成を行います。
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertices"></param>
		void CreateVertexBufferSet
		(
			const size_t vertexSize,
			const  std::vector<size_t>& vertexNum
		);

		void MapVertices(void** data, const int bufferNum);
		void UnmapVertices(const int bufferNum);
#pragma endregion

#pragma region インデックス


		/// <summary>
		/// インデックスバッファ、ビューの生成を行います。
		/// </summary>
		/// <param name="indices"></param>
		void CreateIndexBufferSet
		(
			const std::vector<std::vector<USHORT>>& indices
		);

		void MapIndices(const std::vector<std::vector<USHORT>>& indices);
#pragma endregion



		static void CreatePrimitiveModel();
		static void CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices,const std::vector<USHORT>& indices);
		
		void CreateModel();

		/// <summary>
		/// モデルの頂点データ、マテリアルを読み込みます。
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		bool LoadModel(const std::string& path, const std::string& name);


		/// <summary>
		/// 頂点インデックスバッファテクスチャバッファの生成とインデックスとテクスチャのMapを行います。
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertexNum"></param>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		void BufferPreparationSetTexture
		(
			const size_t vertexSize,
			const std::vector<size_t>& vertexNum,
			const std::vector<std::vector<USHORT>>& indices
		);

		void BufferPreparationSetColor
		(
			const size_t vertexSize,
			const  std::vector<size_t>& vertexNum,
			const std::vector<std::vector<USHORT>>& indices
		);


		static std::vector<std::array<float, 6>>CalcDirectionMaxPosition
		(
			std::vector<std::vector<FbxVertex>>vertices
		);
	public:

		ModelData() {}
		~ModelData() {}

		/// <summary>
		/// 頂点情報を元にモデルデータを生成します。
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		/// <param name=""></param>
		/// <param name="batchDeletionFlag"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		static bool Create(std::vector<std::vector<FbxVertex>>vertices, std::vector<std::vector<USHORT>>indices, const bool batchDeletionFlag, const std::string& name);

		void Create(std::vector<std::vector<FbxVertex>>vertices, std::vector<std::vector<USHORT>>indices);

		/// <summary>
		/// モデルを読み込みます。
		/// </summary>
		/// <param name="path">パス</param>
		/// <param name="batchDeletionFlag">一括削除対象かどうか</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		static bool Load(const std::string& path,const bool batchDeletionFlag, const std::string& name);

		/// <summary>
		///	モデルデータを取得します。
		/// </summary>
		/// <param name="name">モデルの登録名</param>
		/// <returns></returns>
		static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

		/// <summary>
		/// モデルデータを取得します。
		/// </summary>
		/// <param name="type">取得したいモデルの形状</param>
		/// <returns></returns>
		static ModelData* Get(const ShapeType3D type) { return pPrimitiveModelDatas[type].get(); }

		/// <summary>
		/// モデルデータを削除します。
		/// </summary>
		/// <param name="name"></param>
		static void Delete(const std::string& name);
		
		/// <summary>
		/// 一括削除の対象になっているモデルデータを削除します。
		/// </summary>
		static void BatchDeletion();



#pragma region 開発者用関数
		static void Initialize(ID3D12Device* pDevice);

		/// <summary>
		/// インデックスを取得。
		/// </summary>
		/// <returns></returns>
		const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }

		/// <summary>
		/// 頂点バッファを取得。
		/// </summary>
		/// <returns></returns>
		const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }

		/// <summary>
		/// インデックスバッファの取得。
		/// </summary>
		/// <returns></returns>
		const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }

		/// <summary>
		/// ディスクリプタヒープの取得。
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }


#pragma region fbx関係


		const std::vector<FbxBone>& GetFbxBone() const { return fbxData.bones; }

		/// <summary>
		/// モデルのFbxAnimationTimesを返します。
		/// </summary>
		/// <returns></returns>
		const FbxAnimationTimes& GetFbxAnimationTimes()const { return fbxData.animationTimes; }

#pragma endregion

#pragma region obj関係

		const std::vector<Vector3>& GetObjBonePosition()const { return objData.bonePosition; }
#pragma endregion


#pragma endregion

#pragma region セット

#pragma endregion

#pragma region ゲット
		/// <summary>
		/// モデルのフォーマットを取得します。
		/// </summary>
		/// <returns></returns>
		ModelFormat GetModelFormat() const { return modelFormat; }

		/// <summary>
		/// ボーン数を取得します。
		/// </summary>
		/// <returns></returns>
		UINT GetBoneNumber() const { return boneNum; }

		/// <summary>
		/// モデルファイルに含まれているオブジェクト(モデル)の数を取得します。
		/// </summary>
		/// <returns></returns>
		UINT GetModelFileObjectNumber()const { return modelFileObjectNum; }

		//ポインタ返さないと、2回目呼び出した時にmetadataがちゃんと代入できてないのか初期化してないときと同じになるからポインタ返してる
		/// <summary>
		/// 読み込んだモデルのマテリアルのポインタを取得します。
		/// </summary> 
		/// <returns></returns>
		std::vector<ADSAMaterial*> GetPMaterial();

		static ADSAMaterial* GetDefaultMaterial() { return defaultMaterial.get(); }

		/// <summary>
		/// 上下左右前後の最高値を取得します。
		/// </summary>
		/// <returns></returns>
		std::vector<std::array<float, 6>>GetDirectionMaxPosition() const{ return directionMaxPos; }

		/// <summary>
		/// 頂点を取得します。
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<FbxVertex>>GetVertices()const { return vertices; }

		/// <summary>
		/// 頂点座標を取得します。
		/// </summary>
		/// <returns>頂点座標</returns>
		std::vector<std::vector<Vector3>>GetVerticesPosition()const;

#pragma endregion

		//フレンドクラスは、privateの変数、関数にアクセスできる
		friend class FbxLoader;
	};

}