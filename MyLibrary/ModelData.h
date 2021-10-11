#pragma once

#include<memory>
#include<vector>
#include<array>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"
#include"CollisionType.h"

#include"Material.h"

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

namespace MelLib
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
			//1�t���[���̎���
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

		//�v���~�e�B�u���f���A�}�e���A����nullptr�̃��f���f�[�^�Ɋ��蓖�Ă���}�e���A��
		static std::unique_ptr<ADSAMaterial>defaultMaterial;


		static ID3D12Device* device;

#pragma region �o�b�t�@_�q�[�v

		//���_
		std::vector<VertexBufferSet> vertexBufferSet;
		//�C���f�b�N�X
		std::vector <IndexBufferSet> indexBufferSet;
		ComPtr<ID3D12DescriptorHeap>textureDescHeap;
		std::vector<ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

		//��č폜�Ώۃt���O
		bool batchDeletionFlag = false;

#pragma region ���f�����
		ModelFormat modelFormat = ModelFormat::MODEL_FORMAT_NONE;

		//���f����
		std::string modelName;

		//�}�e���A���ɃZ�b�g����e�N�X�`��
		std::vector<std::unique_ptr<Texture>>pTexture;
		//�}�e���A��
		std::vector<std::unique_ptr<ADSAMaterial>> material;

		//�e�N�X�`�����g��Ȃ��ꍇ�̐F
		Color color;

		//���f���t�@�C���ɉ����f�������邩
		UINT modelFileObjectNum = 1;
		std::vector<std::vector<FbxVertex>>vertices;
		//�X���[�Y�V�F�[�f�B���O�p�@��
		std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;
		//�㉺���E�O��̍ō�(�Œ�)���W�̒l(�㉺���E�O��̏��Ɋi�[)
		std::vector<std::array<float, 6>>directionMaxPos;

		std::vector<std::vector<USHORT>> indices;

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


		static std::vector<std::array<float, 6>>CalcDirectionMaxPosition
		(
			std::vector<std::vector<FbxVertex>>vertices
		);
	public:

		ModelData() {}
		~ModelData() {}

		/// <summary>
		/// ���_�������Ƀ��f���f�[�^�𐶐����܂��B
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		/// <param name=""></param>
		/// <param name="batchDeletionFlag"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		static bool Create(std::vector<std::vector<FbxVertex>>vertices, std::vector<std::vector<USHORT>>indices, const bool batchDeletionFlag, const std::string& name);

		void Create(std::vector<std::vector<FbxVertex>>vertices, std::vector<std::vector<USHORT>>indices);

		/// <summary>
		/// ���f����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="path">�p�X</param>
		/// <param name="batchDeletionFlag">�ꊇ�폜�Ώۂ��ǂ���</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static bool Load(const std::string& path,const bool batchDeletionFlag, const std::string& name);

		/// <summary>
		///	���f���f�[�^���擾���܂��B
		/// </summary>
		/// <param name="name">���f���̓o�^��</param>
		/// <returns></returns>
		static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

		/// <summary>
		/// ���f���f�[�^���擾���܂��B
		/// </summary>
		/// <param name="type">�擾���������f���̌`��</param>
		/// <returns></returns>
		static ModelData* Get(const ShapeType3D type) { return pPrimitiveModelDatas[type].get(); }

		/// <summary>
		/// ���f���f�[�^���폜���܂��B
		/// </summary>
		/// <param name="name"></param>
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
		/// <summary>
		/// ���f���̃t�H�[�}�b�g���擾���܂��B
		/// </summary>
		/// <returns></returns>
		ModelFormat GetModelFormat() const { return modelFormat; }

		/// <summary>
		/// �{�[�������擾���܂��B
		/// </summary>
		/// <returns></returns>
		UINT GetBoneNumber() const { return boneNum; }

		/// <summary>
		/// ���f���t�@�C���Ɋ܂܂�Ă���I�u�W�F�N�g(���f��)�̐����擾���܂��B
		/// </summary>
		/// <returns></returns>
		UINT GetModelFileObjectNumber()const { return modelFileObjectNum; }

		//�|�C���^�Ԃ��Ȃ��ƁA2��ڌĂяo��������metadata�������Ƒ���ł��ĂȂ��̂����������ĂȂ��Ƃ��Ɠ����ɂȂ邩��|�C���^�Ԃ��Ă�
		/// <summary>
		/// �ǂݍ��񂾃��f���̃}�e���A���̃|�C���^���擾���܂��B
		/// </summary> 
		/// <returns></returns>
		std::vector<ADSAMaterial*> GetPMaterial();

		static ADSAMaterial* GetDefaultMaterial() { return defaultMaterial.get(); }

		/// <summary>
		/// �㉺���E�O��̍ō��l���擾���܂��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::array<float, 6>>GetDirectionMaxPosition() const{ return directionMaxPos; }

		/// <summary>
		/// ���_���擾���܂��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<FbxVertex>>GetVertices()const { return vertices; }

		/// <summary>
		/// ���_���W���擾���܂��B
		/// </summary>
		/// <returns>���_���W</returns>
		std::vector<std::vector<Vector3>>GetVerticesPosition()const;

#pragma endregion

		//�t�����h�N���X�́Aprivate�̕ϐ��A�֐��ɃA�N�Z�X�ł���
		friend class FbxLoader;
	};

}