#pragma once
#include<vector>
#include<array>

#include"Sprite2DBase.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

//�����_�[�^�[�Q�b�g���ǂ��ɕ`�悷�邩���w��ł���悤�ɂ���

namespace MelLib
{
	class RenderTarget :public Sprite2DBase
	{
	private:

		static const int RT_NUM = 1;

		static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
		static UINT createCount;
		static std::string mainRenderTargetNama;

		static float sClearColor[4];
		static PipelineState defaultPipeline;
		static ComPtr<ID3D12RootSignature>rootSignature;

		//���ݕ`��ΏۂɂȂ��Ă��郌���_�[�^�[�Q�b�g(�}���`�Ή��̂��߁A�z��ɂ��Ă���)
		static std::vector<RenderTarget*> pCurrentSelectRTs;

		ComPtr<ID3D12Resource>textureBuffer[RT_NUM];
		ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[

		ComPtr<ID3D12DescriptorHeap>rtvHeap;

		ComPtr<ID3D12Resource>depthBuffer;
		ComPtr<ID3D12DescriptorHeap>depthHeap;

		//�J�����̃|�C���^
		Camera* pCamera = nullptr;
		RenderTarget* pRenderingRenderTarget = nullptr;
		float clearColor[4];
	public:

		RenderTarget(const Color& color);
		~RenderTarget();


		static void Create(const Color& initColor, const std::string& name = "");
		static void Delete(const std::string& name);
		static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

		static bool Initialize();

		//����private��
		void PreDrawProcess();
		void SetCmdList();
		static void AllDraw();
		//PreDrawProcess���Ăяo���֐�
		static void AllDrawBegin();

		static void ChangeCurrentRenderTarget(std::vector<RenderTarget*> pRTs);

		/// <summary>
		/// �J�������Z�b�g���܂��B
		/// </summary>
		/// <param name="name"></param>
		void SetCamera(Camera* pCamera) { this->pCamera = pCamera; }

		/// <summary>
		/// �����_�[�^�[�Q�b�g��`�悷�郌���_�[�^�[�Q�b�g���w�肵�܂��B
		/// </summary>
		/// <param name="pRT">�����_�[�^�[�Q�b�g�̃|�C���^�B�w�肵�Ȃ��ꍇ�A���C�������_�[�^�[�Q�b�g�ɕ`�悵�܂��B</param>
		void SetRenderingRenderTarget(RenderTarget* pRT = RenderTarget::Get(mainRenderTargetNama)) { pRenderingRenderTarget = pRT; }

		static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

		Camera* GetCamera() { return pCamera; }

	};

}