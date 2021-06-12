#pragma once
#include<vector>
#include<array>

#include"Sprite2D.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


//�����_�[�^�[�Q�b�g�N���X
//�`�掞�ɓn�����ƂŁA�n���������_�[�^�[�Q�b�g�ɕ`��ł���悤�ɂ���
//�Z�b�g���Ȃ��ꍇ�́A�o�b�N�o�b�t�@�ɒ��ڂ܂���1���ڂ̃����_�[�^�[�Q�b�g�ɕ`��

class RenderTarget :public Sprite2D
{
private:

	static std::vector<RenderTarget*>pRenderTarget;
	static float clearColor[4];
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	ComPtr<ID3D12Resource>textureBuffer[2];
	ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[

	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//�J�����̃|�C���^
	Camera* pCamera = nullptr;

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="color">�������F</param>
	RenderTarget(const Color& color);
	~RenderTarget();

	//void Create(const Color& color);

	static bool Initialize();

	/// <summary>
	/// �`��O�̏���
	/// </summary>
	void PreDrawProcess();

	void Draw()override;
	static void AllDraw();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�ɃJ�������Z�b�g���܂��B�����_�[�^�[�Q�b�g�ɂ́A�Z�b�g���ꂽ�J�����ɉf���Ă�����̂��`�悳��܂��B
	/// </summary>
	/// <param name="camera">�J�����̃|�C���^</param>
	void SetCamera(Camera* pCamera){ this->pCamera = pCamera; }

};

