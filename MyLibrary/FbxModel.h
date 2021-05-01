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

	static PipelineState defaultFbxPipeline;


	//���f����
	std::string modelName;

	//���f���̃m�[�h
	std::vector<Node>nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;

	std::vector<std::vector<USHORT>>& getIndices() { return indices; }
	std::vector<Material>& getMaterial() { return materials; }

	std::vector<std::unique_ptr<Texture>>textures;

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

	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;

};
