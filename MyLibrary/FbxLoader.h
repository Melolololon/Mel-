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


class FbxLoader
{
private:
	FbxLoader();
	~FbxLoader();


	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

	/// <summary>
	/// ノード構造解析用関数
	/// </summary>
	/// <param name="model">モデルのポインタ</param>
	/// <param name="fbxNode">解析するノード</param>
	/// <param name="parentNode">親ノード</param>
	void parseNodeRecursive(FbxModel* model, FbxNode* fbxNode,Node* parentNode = nullptr);

#pragma region メッシュ読み込み


	/// <summary>
	/// メッシュ読み込み関数
	/// </summary>
	/// <param name="">モデル</param>
	/// <param name="node">ノード</param>
	void parseMesh(FbxModel* model, FbxNode* node);

	/// <summary>
	/// 頂点読み込み
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMaterial(FbxModel* model, FbxMesh* fbxMesh);
	
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="model"></param>
	/// <param name="path"></param>
	void parseTexture(FbxModel* model,const std::string& path);
#pragma endregion
public:
	FbxLoader(const FbxLoader& moder) = delete;
	FbxLoader& operator =(const FbxLoader& moder) = delete;
	static FbxLoader* getInstance();

	/// <summary>
	/// 初期化します
	/// </summary>
	/// <param name="device">デバイス</param>
	void initialize(ID3D12Device* device);
	
	void end();

	/// <summary>
	/// fbxを読み込みます
	/// </summary>
	/// <param name="modelPath">モデルのパス</param>
	void loadFbxModel(const std::string& modelPath);
};

