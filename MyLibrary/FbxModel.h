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
	//���f����
	std::string modelName;

	//���f���̃m�[�h
	std::vector<Node>nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;

	std::vector<std::unique_ptr<Texture>>textures;

	//�ǂݍ��ݎ��ɃZ�b�g�����p�C�v���C��
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

	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;

};
