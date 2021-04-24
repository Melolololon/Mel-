#pragma once
#include<vector>
#include<string>
#include<DirectXMath.h>

#include"DirectXStruct.h"
#include"Model.h"
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
	//std::vector<USHORT>indices;

	std::vector<USHORT>& getIndices() { return indices[0]; }
public:
	//フレンドクラスは、privateの変数、関数を参照できる
	friend class FbxLoader;

};
