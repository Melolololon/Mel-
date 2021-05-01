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
	//���f����
	std::string modelName;

	//���f���̃m�[�h
	std::vector<Node>nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;

	std::vector<USHORT>& getIndices() { return indices[0]; }
	std::vector<Material>& getMaterial() { return materials; }

	std::vector<Texture>textures;

public:
	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;


	bool loadModel
	(
		const std::string& path,
		const int createNum,
		const size_t constDataSize = 0
	);
};
