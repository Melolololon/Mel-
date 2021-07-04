#pragma once

#include<memory>
#include<vector>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"


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
private:

	static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;


	
#pragma region obj構造体

	//objのボーン情報をまとめたもの
	struct ObjBone
	{
		int boneNum = 0;
		std::vector<Vector3>bonePosition;
	};

#pragma endregion

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


	//fbxのデータをまとめたもの
	struct FbxData
	{
		FbxScene* fbxScene = nullptr;

		//モデルのノード
		std::vector<Node>nodes;

		//メッシュを持つノード
		Node* meshNode = nullptr;

		std::vector<FbxBone>bones;
		std::vector<FbxBone>& GetBones() { return bones; }
	};

#pragma endregion

	static ID3D12Device* device;

#pragma region バッファ_ヒープ


	//頂点
	std::vector<VertexBufferSet> vertexBufferSet;
	//インデックス
	std::vector <IndexBufferSet> indexBufferSet;
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

#pragma region モデル情報
	//モデル名
	std::string modelName;

	//読み取ったマテリアル
	std::vector<Material> materials;

	//テクスチャを使わない場合の色
	Color color;

	//モデルファイルに何個モデルがあるか
	int modelFileObjectNum = 0;
	std::vector<std::vector<FbxVertex>>vertices;
	//スムーズシェーディング用法線
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	std::vector<std::vector<USHORT>> indices;
	std::vector<std::unique_ptr<Texture>>pTextures;



	std::unique_ptr<ObjBone>objBone;
	std::unique_ptr<FbxData>fbxData;

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

#pragma region テクスチャ

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	/// <param name="textureNum"></param>
	void CreateDescriptorHeap(const UINT textureNum);

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	void CteateTextureBuffer();

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferAndViewSetColor();

#pragma endregion


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
		const std::vector<std::vector<FbxVertex>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);


public:

	ModelData() {}
	~ModelData() {}

	/// <summary>
	/// モデルを読み込みます。
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static bool Load(const std::string& path, const std::string& name);

	static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

	static void Delete(const std::string& name);

	/// <summary>
	/// モデルの頂点データ、マテリアルを読み込みます。
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	bool LoadModel(const std::string& path, const std::string& name);

#pragma region コマンドセット用関数

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

#pragma endregion

#pragma region セット

#pragma endregion

#pragma region ゲット

	/// <summary>
	/// モデルファイルに含まれているオブジェクト(モデル)の数を取得します。
	/// </summary>
	/// <returns></returns>
	int GetModelFileObjectNumber()const { return modelFileObjectNum; }

	/// <summary>
	/// マテリアルを取得します。
	/// </summary>
	/// <param name="objectNum">モデルファイル内のモデルを指定する値</param>
	/// <returns></returns>
	const Material& GetMaterial(const int objectNum)const { return materials[objectNum]; }


#pragma endregion

	//フレンドクラスは、privateの変数、関数を参照できる
	friend class FbxLoader;
};

