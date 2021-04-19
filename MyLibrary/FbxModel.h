#pragma once
#include<vector>
#include<string>
#include<DirectXMath.h>
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
public:
	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;

};

