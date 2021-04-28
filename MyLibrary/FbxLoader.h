#pragma once

#include"FbxModel.h"

#include<string>
#include<d3dx12.h>
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


class FbxLoader final
{
private:
	FbxLoader();
	~FbxLoader();


	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

	std::string modelDirectryPath;

	/// <summary>
	/// ノード構造解析用関数
	/// </summary>
	/// <param name="model">モデルのポインタ</param>
	/// <param name="fbxNode">解析するノード</param>
	/// <param name="parentNode">親ノード</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode,Node* parentNode = nullptr);

#pragma region メッシュ読み込み


	/// <summary>
	/// メッシュ読み込み関数
	/// </summary>
	/// <param name="">モデル</param>
	/// <param name="node">ノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* node);

	/// <summary>
	/// 頂点読み込み
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報(インデックス、UV、法線)読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseMaterial(FbxModel* fbxModel,FbxNode* fbxNode);
	
	std::string ExtractFileName
	(
		const std::string& path, 
		std::string* directry, 
		std::string* fileName
	);

#pragma endregion
public:
	FbxLoader(const FbxLoader& moder) = delete;
	FbxLoader& operator =(const FbxLoader& moder) = delete;
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化します
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);
	
	void Finalize();

	/// <summary>
	/// fbxを読み込みます
	/// </summary>
	/// <param name="modelPath">モデルのパス</param>
	/// <param name="fbxModel">モデルのポインタ</param>
	void LoadFbxModel(const std::string& modelPath,FbxModel* fbxModel);
};

