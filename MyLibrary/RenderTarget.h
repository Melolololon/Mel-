#pragma once
#include<vector>
#include<array>

#include"Sprite2D.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


class RenderTarget :public Sprite2D
{
private:



	static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
	static UINT createCount;
	static std::string mainRenderTargetNama;

	static float clearColor[4];
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static const UINT RT_NUM = 2;
	static const UINT MOTION_BLAR_RT_NUM = 2;
	//[���[�V�����u���[�p�����_�[�^�[�Q�b�g(�O�̃t���[���̌��ʕ`��Ɏg�p)][�����`�搔]
	std::array<std::array<ComPtr<ID3D12Resource>, RT_NUM>, MOTION_BLAR_RT_NUM>textureBuffer;
	ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[
	ComPtr<ID3D12Resource>motionBlurConstBuffer;
	static std::vector<Color>rtColor;

	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//�J�����̃|�C���^
	Camera* pCamera = nullptr;

	//���[�V�����u���[���s�����ǂ���
	bool isMotionBlur = false;
	/// <summary>
	/// ���[�V�����u���[���s�����߂ɑO�t���[���̃����_�����O���ʂ��R�s�[����֐�
	/// </summary>
	void CopyPreviousRenderingTexture();

	void MapMotionBlurConstData();

public:

	RenderTarget(const Color& color);
	~RenderTarget();


	static void Create(const Color& initColor, const std::string& name = "");
	static void Delete(const std::string& name);
	static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

	static bool Initialize();

	void PreDrawProcess();
	void SetCmdList();
	static void AllDraw();



#pragma region �Z�b�g


	void SetCamera(const std::string& name = Camera::GetMainCameraName()) { this->pCamera = Camera::Get(name); }

	void SetMotionBlurFlag(const bool flag) { isMotionBlur = flag; }

#pragma endregion

#pragma region �Q�b�g

	static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

	Camera* GetCamera() { return pCamera; }
#pragma endregion


};

