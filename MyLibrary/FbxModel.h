#pragma once
#include<vector>
#include<string>
#include<DirectXMath.h>

#include"DirectXStruct.h"
#include"Model.h"

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

class FbxModel : public Model
{
private:
	//モデル名
	std::string modelName;

	//モデルのノード
	std::vector<Node>nodes;

	//メッシュを持つノード
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;

	std::vector<USHORT>& getIndices() { return indices[0]; }
	std::vector<Material>& getMaterial() { return materials; }

	std::vector<Texture>textures;

public:
	//フレンドクラスは、privateの変数、関数を参照できる
	friend class FbxLoader;


	bool loadModel
	(
		const std::string& path,
		const int createNum,
		const size_t constDataSize = 0
	);
};
