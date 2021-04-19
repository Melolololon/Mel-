#pragma once
#include<vector>
#include<string>
#include<DirectXMath.h>

#include"DirectXStruct.h"
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

class FbxModel
{
private:
	//���f����
	std::string modelName;

	//���f���̃m�[�h
	std::vector<Node>nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;

	std::vector<Vertex>vertices;
	std::vector<USHORT>indices;

public:
	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;

};

