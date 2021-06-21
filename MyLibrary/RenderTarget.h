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

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static const UINT RT_NUM = 2;
	//[���[�V�����u���[�p�����_�[�^�[�Q�b�g(�O�̃t���[���̌��ʕ`��Ɏg�p)][�����`�搔]
	std::array<ComPtr<ID3D12Resource>, RT_NUM>textureBuffer;
	ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[



	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//�J�����̃|�C���^
	Camera* pCamera = nullptr;



public:

	RenderTarget(const Color& color);
	~RenderTarget();


	static void Create(const Color& initColor, const std::string& name = "");
	static void Delete(const std::string& name);
	static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

	static bool Initialize();

	void PreDrawProcess();
	void SetCmdList();
	void Draw();
	static void AllDraw();



#pragma region �Z�b�g

	void SetCamera(const std::string& name = Camera::GetMainCameraName()) { this->pCamera = Camera::Get(name); }



#pragma endregion

#pragma region �Q�b�g
	static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

	Camera* GetPCamera(){ return pCamera; }
#pragma endregion


};

