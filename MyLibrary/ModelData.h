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
#include"CollisionType.h"


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

namespace melLib
{
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
	public:
		enum class ModelFormat
		{
			MODEL_FORMAT_NONE,
			MODEL_FORMAT_PRIMITIVE,
			MODEL_FORMAT_OBJ,
			MODEL_FORMAT_FBX,
		};


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

		//fbx�̃A�j���[�V�����ɕK�v��FbxTime���܂Ƃ߂��\����
		struct FbxAnimationTimes
		{
			FbxTime startTime;
			FbxTime endTime;
			FbxTime freamTime;
		};

#pragma endregion


	private:

#pragma region obj�\����

		//obj�̃{�[�������܂Ƃ߂�����
		struct ObjBone
		{
			std::vector<Vector3>bonePosition;
		};

#pragma endregion


#pragma region fbx�\����


		//fbx�̃f�[�^���܂Ƃ߂�����
		struct FbxData
		{
			FbxScene* fbxScene = nullptr;

			//���f���̃m�[�h
			std::vector<Node>nodes;

			//���b�V�������m�[�h
			Node* meshNode = nullptr;

			std::vector<FbxBone>bones;

			FbxAnimationTimes animationTimes;

		};


#pragma endregion

		//���f�����i�[����z��
		static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;
		//�v���~�e�B�u���f�����i�[����z��
		static std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>pPrimitiveModelDatas;
	

		static ID3D12Device* device;

#pragma region �o�b�t�@_�q�[�v


		//���_
		std::vector<VertexBufferSet> vertexBufferSet;
		//�C���f�b�N�X
		std::vector <IndexBufferSet> indexBufferSet;
		ComPtr<ID3D12DescriptorHeap>textureDescHeap;
		std::vector<ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

		bool batchDeletionFlag = false;

#pragma region ���f�����
		ModelFormat modelFormat = ModelFormat::MODEL_FORMAT_NONE;

		//���f����
		std::string modelName;

		//�ǂݎ�����}�e���A��
		std::vector<MaterialData> materials;

		//PBR�p�}�e���A���f�[�^
		std::vector<PbrMaterial>pbrMaterials;

		//�e�N�X�`�����g��Ȃ��ꍇ�̐F
		Color color;

		//���f���t�@�C���ɉ����f�������邩
		UINT modelFileObjectNum = 1;
		std::vector<std::vector<FbxVertex>>vertices;
		//�X���[�Y�V�F�[�f�B���O�p�@��
		std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

		std::vector<std::vector<USHORT>> indices;
		std::vector<std::unique_ptr<Texture>>pTextures;


		UINT boneNum = 0;

		ObjBone objData;

		FbxData fbxData;

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
		void CteateTextureBufferSetColor();

#pragma endregion


		static void CreatePrimitiveModel();
		static void CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices,const std::vector<USHORT>& indices);
		void CreateModel();

		/// <summary>
		/// ���f���̒��_�f�[�^�A�}�e���A����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		bool LoadModel(const std::string& path, const std::string& name);


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
			const std::vector<std::vector<USHORT>>& indices
		);


	public:

		ModelData() {}
		~ModelData() {}


		/// <summary>
		/// ���f����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="path">�p�X</param>
		/// <param name="batchDeletionFlag">�ꊇ�폜�Ώۂ��ǂ���</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static bool Load(const std::string& path,const bool batchDeletionFlag, const std::string& name);

		static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }
		static ModelData* Get(const ShapeType3D type) { return pPrimitiveModelDatas[type].get(); }

		static void Delete(const std::string& name);
		
		/// <summary>
		/// �ꊇ�폜�̑ΏۂɂȂ��Ă��郂�f���f�[�^���폜���܂��B
		/// </summary>
		static void BatchDeletion();



#pragma region �J���җp�֐�
		static void Initialize(ID3D12Device* pDevice);

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


#pragma region fbx�֌W


		const std::vector<FbxBone>& GetFbxBone() const { return fbxData.bones; }

		/// <summary>
		/// ���f����FbxAnimationTimes��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		const FbxAnimationTimes& GetFbxAnimationTimes()const { return fbxData.animationTimes; }

#pragma endregion

#pragma region obj�֌W
		const std::vector<Vector3>& GetObjBonePosition()const { return objData.bonePosition; }
#pragma endregion


#pragma endregion

#pragma region �Z�b�g

#pragma endregion

#pragma region �Q�b�g

		ModelFormat GetModelFormat() const { return modelFormat; }

		UINT GetBoneNumber() const { return boneNum; }

		/// <summary>
		/// ���f���t�@�C���Ɋ܂܂�Ă���I�u�W�F�N�g(���f��)�̐����擾���܂��B
		/// </summary>
		/// <returns></returns>
		UINT GetModelFileObjectNumber()const { return modelFileObjectNum; }

		//����string�Ŏw�肷��悤�ɁBobj�̏ꍇ�A"1"�̂悤�Ɏw��
		/// <summary>
		/// �}�e���A�����擾���܂��B
		/// </summary>
		/// <param name="objectNum">���f���t�@�C�����̃��f�����w�肷��l</param>
		/// <returns></returns>
		MaterialData GetMaterial(const int objectNum)const { return materials[objectNum]; }

		PbrMaterial GetPbrMaterial(const int objectNum)const { return pbrMaterials[objectNum]; }
#pragma endregion

		//�t�����h�N���X�́Aprivate�̕ϐ��A�֐��ɃA�N�Z�X�ł���
		friend class FbxLoader;
	};

}