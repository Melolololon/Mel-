#pragma once

#include<memory>
#include<vector>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"


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


//obj�Ƃ���fbx�̃f�[�^�����Ă�̃��������������Ȃ�����A
//�\���̂ł܂Ƃ߂āAfbx�̎�����new�Ń������m�ۂ���悤�ɂ���΂��������Ȃ��Ȃ�

//���f���̒��_�A�C���f�b�N�X�A�e�N�X�`�����܂Ƃ߂��N���X
class ModelData
{
private:

	static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;


	
#pragma region obj�\����

	//obj�̃{�[�������܂Ƃ߂�����
	struct ObjBone
	{
		int boneNum = 0;
		std::vector<Vector3>bonePosition;
	};

#pragma endregion

#pragma region fbx�\����

	//fbx�̃{�[�������܂Ƃ߂�����
	struct FbxBone
	{
		//�{�[����
		std::string boneName;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�{�[�����
		FbxCluster* fbxCluster;

		FbxBone(const std::string& name)
		{
			this->boneName = name;
		}
	};


	//fbx�̃f�[�^���܂Ƃ߂�����
	struct FbxData
	{
		FbxScene* fbxScene = nullptr;

		//���f���̃m�[�h
		std::vector<Node>nodes;

		//���b�V�������m�[�h
		Node* meshNode = nullptr;

		std::vector<FbxBone>bones;
		std::vector<FbxBone>& GetBones() { return bones; }
	};

#pragma endregion

	static ID3D12Device* device;

#pragma region �o�b�t�@_�q�[�v


	//���_
	std::vector<VertexBufferSet> vertexBufferSet;
	//�C���f�b�N�X
	std::vector <IndexBufferSet> indexBufferSet;
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

#pragma region ���f�����
	//���f����
	std::string modelName;

	//�ǂݎ�����}�e���A��
	std::vector<Material> materials;

	//�e�N�X�`�����g��Ȃ��ꍇ�̐F
	Color color;

	//���f���t�@�C���ɉ����f�������邩
	int modelFileObjectNum = 0;
	std::vector<std::vector<FbxVertex>>vertices;
	//�X���[�Y�V�F�[�f�B���O�p�@��
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	std::vector<std::vector<USHORT>> indices;
	std::vector<std::unique_ptr<Texture>>pTextures;



	std::unique_ptr<ObjBone>objBone;
	std::unique_ptr<FbxData>fbxData;

#pragma endregion







#pragma region ���_

	/// <summary>
	/// ���_�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertices"></param>
	void CreateVertexBufferSet
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum
	);

	void MapVertices(void** data, const int bufferNum);
	void UnmapVertices(const int bufferNum);
#pragma endregion

#pragma region �C���f�b�N�X


	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBufferSet
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	void MapIndices(const std::vector<std::vector<USHORT>>& indices);
#pragma endregion

#pragma region �e�N�X�`��

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̐���
	/// </summary>
	/// <param name="textureNum"></param>
	void CreateDescriptorHeap(const UINT textureNum);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	void CteateTextureBuffer();

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferAndViewSetColor();

#pragma endregion


	/// <summary>
	/// ���_�C���f�b�N�X�o�b�t�@�e�N�X�`���o�b�t�@�̐����ƃC���f�b�N�X�ƃe�N�X�`����Map���s���܂��B
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertexNum"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	void BufferPreparationSetTexture
	(
		const size_t vertexSize,
		const std::vector<size_t>& vertexNum,
		const std::vector<std::vector<USHORT>>& indices
	);

	void BufferPreparationSetColor
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<FbxVertex>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);


public:

	ModelData() {}
	~ModelData() {}

	/// <summary>
	/// ���f����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	/// <returns></returns>
	static bool Load(const std::string& path, const std::string& name);

	static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

	static void Delete(const std::string& name);

	/// <summary>
	/// ���f���̒��_�f�[�^�A�}�e���A����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	bool LoadModel(const std::string& path, const std::string& name);

#pragma region �R�}���h�Z�b�g�p�֐�

	/// <summary>
	/// �C���f�b�N�X���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }
	
	/// <summary>
	/// ���_�o�b�t�@���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }
	
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̎擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̎擾�B
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

#pragma endregion

#pragma region �Z�b�g

#pragma endregion

#pragma region �Q�b�g

	/// <summary>
	/// ���f���t�@�C���Ɋ܂܂�Ă���I�u�W�F�N�g(���f��)�̐����擾���܂��B
	/// </summary>
	/// <returns></returns>
	int GetModelFileObjectNumber()const { return modelFileObjectNum; }

	/// <summary>
	/// �}�e���A�����擾���܂��B
	/// </summary>
	/// <param name="objectNum">���f���t�@�C�����̃��f�����w�肷��l</param>
	/// <returns></returns>
	const Material& GetMaterial(const int objectNum)const { return materials[objectNum]; }


#pragma endregion

	//�t�����h�N���X�́Aprivate�̕ϐ��A�֐����Q�Ƃł���
	friend class FbxLoader;
};

