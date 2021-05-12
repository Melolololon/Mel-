#pragma once

#include"DirectXStruct.h"
#include"Model.h"
#include<string>
#include"Texture.h"

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



class FbxModel:public Model
{
public:
	

	struct Bone
	{
		//ボーン名
		std::string boneName;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//ボーン情報
		FbxCluster* fbxCluster;
		
		Bone(const std::string& name)
		{
			this->boneName = name;
		}
	};

	static const int MAX_BONE_INDICES = 4;
	struct FbxVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

private:
	FbxScene* fbxScene = nullptr;

	//モデル名
	std::string modelName;

	//モデルのノード
	std::vector<Node>nodes;

	//メッシュを持つノード
	Node* meshNode = nullptr;

	std::vector<Bone>bones;
	std::vector<Bone>& GetBones() { return bones; }
	


	std::vector<FbxVertex>vertices;
	std::vector<std::unique_ptr<Texture>>textures;

	//読み込み時にセットされるパイプライン
	static PipelineState defaultPipeline;

	std::vector<std::vector<USHORT>>& getIndices() { return indices; }
	std::vector<Material>& getMaterial() { return materials; }
public:



	FbxModel();
	~FbxModel();

	bool modelNum
	(
		const std::string& path,
		const int modelNum,
		//const size_t vertexSize,
		const size_t constDataSize
	);

	static bool Initialize();

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }

	//フレンドクラスは、privateの変数、関数を参照できる
	friend class FbxLoader;

};
