#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"
#include"CollisionType.h"
#include"Material.h"
//AddColor�Ƃ��܂Ƃ߂悤!

namespace MelLib
{
	
	//���f���̍��W�Ȃǂ��܂Ƃ߂�����
	class ModelObject
	{

	private:

		static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;
		static PipelineState defaultPipeline;


		std::vector<PipelineState*> pPipeline;

		//[���f�����̃I�u�W�F�N�g����]
		std::vector<Material>materials;

#pragma region �{�[���ƃA�j���[�V�����̏��
		static const UINT BONE_MAX = 64;
		struct SkinConstBufferData
		{
			DirectX::XMMATRIX bones[BONE_MAX];
		};

		//fbx���f���̃A�j���[�V�����p�̏����܂Ƃ߂�����
		struct FbxAnimationData
		{
			ModelData::FbxAnimationTimes animationTimes;
			FbxTime currentTime;
			int timeMag = 1;
		};


		//[�{�[������]
		std::vector<BoneData>boneDatas;
		std::vector<ParentBoneData> parentBoneDatas;//�e�{�[���Ɖe���x

		FbxAnimationData fbxAnimationData;
		bool isAnimation = false;
#pragma endregion

		//�萔�o�b�t�@
		static const int CONST_BUFFER_REGISTER = 0;
		std::vector<ComPtr<ID3D12Resource>> constBuffer;//���C��(��{�I�ȏ��)

		static const int MATERIAL_BUFFER_REGISTER = 2;

		static const int COLOR_MATERIAL_BUFFER_REGISTER = 4;

		static const int USER_BUFFER_REGISTER = 1;
		std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//���[�U�[
		ConstBufferData userConstBufferData;

		static const int MODEL_BUFFER_REGISTER = 3;
		std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//���f�����L(�A�j���[�V�����֌W�̏��)
		ConstBufferData modelConstBufferData;

		static const int TEXURE_ROOTPARAM_NUM = 5;

		//�萔�ɃZ�b�g������W�Ȃǂ̒l
		//[���f�����̃I�u�W�F�N�g��]
		std::vector<ModelConstData> modelConstDatas;

		//���f��(�N���G�C�g�����犄�蓖�Ă�)
		ModelData* pModelData = nullptr;
		UINT modelFileObjectNum = 0;


	private:

		void CreateConstBuffer();

		void DrawCommonProcessing(const std::string& rtName);
		void MapConstData(const Camera* camera);
		void SetCmdList();

		void FbxAnimation();
	public:

		//nullptr�n�����\�����l����ƁAbool��return�ł���悤�ɂ����ق�������?
		ModelObject() {}

		~ModelObject() {}

		static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);


		static ModelObject* Get(const std::string& name) { return pModelObjects[name].get(); }
		static void Delete(const std::string& name);

		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

#pragma region ����

#pragma region ���f���Ő���
		ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData);
		static bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name);
		bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData);
#pragma endregion ���f���Ő���


#pragma region �v���~�e�B�u���f������
	

#pragma endregion �v���~�e�B�u���f������


#pragma endregion ����

#pragma region �Z�b�g

#pragma region ���쌩���ڕύX

#pragma region ����


		void SetPosition(const Vector3& position);
		void SetScale(const Vector3& scale);
		void SetAngle(const Vector3& angle);


#pragma endregion

#pragma region �A�j���[�V����

		void SetAnimationFlag(const bool flag) { isAnimation = flag; }

		/// <summary>
	/// �A�j���[�V���������Z�b�g���܂��B
	/// </summary>
		void ResetAnimation();

		void SetCurrentFream(const UINT fream);

		void SetAnimationSpeedMagnification(const int magnification) { fbxAnimationData.timeMag = magnification; }
#pragma endregion


#pragma endregion

		void SetPipeline(PipelineState* pipelineState);

#pragma endregion

#pragma region �Q�b�g

		//�R���s���[�g�V�F�[�_�[�Ōv�Z�����ق��������B
		//�ł������Ȃ�`�掞�ɒ��_�V�F�[�_�[�Ōv�Z�������ʂ������Ă����ق�������?
		/// <summary>
		/// ���_���W���擾���܂��B
		/// </summary>
		/// <param name="scaleImpact"></param>
		/// <param name="angleImpact"></param>
		/// <param name="transformImpact"></param>
		/// <param name=""></param>
		/// <returns></returns>
		std::vector<std::vector<Vector3>>GetVerticesData
		(
			const bool scaleImpact,
			const bool angleImpact,
			const bool transformImpact
		);

#pragma endregion


	};
}
