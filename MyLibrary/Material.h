#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>
#include<tuple>

#include"Color.h"
#include"Values.h"
#include"Texture3D.h"

namespace MelLib
{


	//Material���p�����ăp�����[�^�𑝂₹��悤�ɂ���
	//�p�����Ă��ǂ�����ăV�F�[�_�[�ɓn��?���p�҂�Map���Ă��炤�����Ȃ�?
	//�萔�o�b�t�@�����R�ɍ���悤�ɂ����ق�������?

	//�}�e���A�����̒萔�o�b�t�@���}�e���A���N���X�Ɏ�������B
	//��������΁A���f���ɕ����̃}�e���A����񎝂�����K�v���Ȃ��B
	//�}�e���A���̊��N���X����Ă����ɒ萔�o�b�t�@�������Ȃ��Ƃ����Ȃ�
	
	//�q�[�v�͂ǂ��Ɏ�������?
	//�e�N�X�`���o�b�t�@�ȊO�Ɏ���������̂��肻���������烂�f����

	//ModelData����������ǂݍ��񂾂�A�����ŏ����}�e���A�����B

	//�}�e���A���̃f�[�^�̍\���̂����������A
	//�}�e���A���\���̂Ƀe���v���[�g�g�����}�e���A����񎝂�����?�ǂݎ���?�}�b�v�͓ǂݎ��Ȃ��Ă��ł���?
	//�z��ɂ܂Ƃ߂��Ȃ����番����
	//���C�u�����Œ�`����Ă���̂��������ŊǗ�����d�l�ɂ���?
	//���p�҂��p�ӂ����}�e���A���͎����ŊǗ����Ă��炤
	//�e���v���[�g��������������킯��
	
	//�}�e���A���̌^�ɍ��킹�ăp�C�v���C���������Ɏg�p����V�F�[�_�[�ς���?
	//�}�e���A���Ƀp�C�v���C����������?
	//�p�C�v���C���Ƀ����_�����O�̏��(�J�����O�Ƃ������_�[�^�[�Q�b�g��)�Ƃ������āA
	//�}�e���A���͂����܂Ō����ڂ̏�����������炢���B
	
	//�}�e���A���̃N���X�w�肷��ƁA���̃}�e���A���ɓK�����V�F�[�_�[�Ńp�C�v���C�������@�\�͂����Ă���������
	//�}�e���A�����Ƃɒ萔�o�b�t�@�ƃe�N�X�`���o�b�t�@�̐��ς�邩��A���[�g�V�O�l�`�����������Ⴄ? 
	//�e���v���[�g�̂��Create�ĂׂȂ��B
	//�p�����č���Ă��炤?

	//�Ƃ肠�������̐i�߂悤

	//�}�e���A�������܂Ƃ߂��N���X�B
	//template<class MTL>
	class Material
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//static std::unordered_map<std::string, Material<ADSAMaterialData>*>pAdsaMaterials;
		//static std::unordered_map<std::string, Material<PBRMaterialData>*>pPbrMaterials;
		

		//MTL materialData;

		//�}�e���A���̒萔�o�b�t�@
		ComPtr<ID3D12Resource>materialBuffer = nullptr;

		//(�}�e���A���̃J���[ + AddColor + SubColor)*MulColor = �ǉ��F
		Color color;
		//�e�N�X�`��
		Texture* pTexture = nullptr;
		Texture* pNormalMapTexture = nullptr;
		Texture3D* pTexture3D = nullptr;

		//�e�N�X�`���N���X�Ɏ������邽�߁A�R�����g�A�E�g
		//ComPtr<ID3D12Resource>textureBuffer;
		
		//�ǂ��Ɏ�������̂��x�X�g���킩��Ȃ����߁A�ۗ�
		//ComPtr<ID3D12DescriptorHeap>textureHeap;

	protected:
		/// <summary>
		/// �}�e���A���̃o�b�t�@�Ƃ������B
		/// </summary>
		/// <param name="mtlByte">�\���̂̃o�C�g��</param>
		void CreateBuffer(const size_t& mtlByte);

		void MapMaterialData(void** pData);
		void UnmapMaterialData();
	public:

		//materialData�̌^�`�F�b�N���āAumap�Ɋi�[
		/*static void Create();
		static Material<MTL>* Get(const std::string& name);
		static void Delete();*/

		virtual void Create() = 0;

		ID3D12Resource* GetPConstBuffer()const { return materialBuffer.Get(); }
		Texture* GetTexture() { return pTexture; }
		Texture* GetNormalTexture() { return pNormalMapTexture; }
		Texture3D* GetTexture3D() { return pTexture3D; }
		//MTL GetMaterialData(MTL data) const {  return materialData; }

		void SetColor(const Color& color) { this->color = color; }
		void SetTexture(Texture* pTex) { pTexture = pTex; }
		void SetNormalMapTexture(Texture* pNormalMapTex) { pNormalMapTex = pNormalMapTex; }
		void SetTexture3D(Texture3D* pTex) { pTexture3D = pTex; }

		//void SetMaterialData(MTL data) { materialData = data; }

	};

#pragma region Template

	//Create�֐��ĂׂȂ�

//	//���p�҃}�e���A���N���X
//	template<class MTL>
//	class TmpMaterial :public Material
//	{
//	private:
//		MTL materialData;
//
//	public:
//#pragma region �Z�b�g
//		void SetMaterialData(const MTL& data) { materialData = data; }
//#pragma endregion
//
//#pragma region �Q�b�g
//		MTL GetMaterialData() { return materialData; }
//#pragma endregion
//
//	};
//#pragma endregion


#pragma region ADSA



	struct ADSAMaterialData
	{
		//����
		Value4<float>ambient = 0.3f;
		//�g�U���ˌ�
		Value4<float>diffuse = 0.7f;
		//���ʔ��ˌ�
		Value4<float>specular = 0.0f;
		//�A���t�@�l
		float alpha = 1.0f;
	};


	//Ambient�ADiffuse�ASupecular�A���f������ǂݎ����Alpha�����������}�e���A��
	class ADSAMaterial :public Material
	{
	private:
		//static std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>>pMaterials;

		ADSAMaterialData materialData;

	public:

		void Create()override;
		void Map();

#pragma region �Z�b�g
		void SetMaterialData(const ADSAMaterialData& data) { materialData = data; }
#pragma endregion

#pragma region �Q�b�g
		ADSAMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};
#pragma endregion

#pragma region PBR

	struct PBRMaterialData
	{
		//�A���x�h�ƃA���t�@�l
		Color baseColor;

		//�����x(0�܂���1)
		float metalness = 0.0f;
		//���ʔ��ˌ�(float 0����1)
		float fSpecular = 0.5f;
		//�e��
		float roughness = 0.0f;
	};

	//PBR�}�e���A��
	class PBRMaterial:public Material
	{
	private:
		PBRMaterialData materialData;

	public:
		void Map();
#pragma region �Z�b�g
		void SetMaterialData(const PBRMaterialData& data) { materialData = data; }
#pragma endregion

#pragma region �Q�b�g
		PBRMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};

#pragma endregion


}
