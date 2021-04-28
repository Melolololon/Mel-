#pragma once

#include"DirectXStruct.h"
#include"Model.h"
#include<string>
#include"Texture.h"
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

class FbxModel:public Model
{
private:
	//モデル名
	std::string modelName;

	//モデルのノード
	std::vector<Node>nodes;

	//メッシュを持つノード
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;

	std::vector<std::unique_ptr<Texture>>textures;

	//読み込み時にセットされるパイプライン
	static PipelineState defaultFbxPipeline;

	std::vector<std::vector<USHORT>>& getIndices() { return indices; }
	std::vector<Material>& getMaterial() { return materials; }
public:
	FbxModel();
	~FbxModel();

	bool LoadModel
	(
		const std::string& path,
		const int createNum,
		const size_t constDataSize = 0
	);

	static bool Initialize();

	//フレンドクラスは、privateの変数、関数を参照できる
	friend class FbxLoader;

};
