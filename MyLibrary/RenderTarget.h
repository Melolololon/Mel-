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


	static std::unordered_map<std::string,std::unique_ptr<RenderTarget>>pRenderTarget;
	static UINT createCount;

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


	RenderTarget(const Color& color);
	~RenderTarget();
	
	/// <summary>
	/// �����_�[�^�[�Q�b�g�𐶐����܂��B
	/// </summary>
	/// <param name="initColor">�������F</param>
	/// <param name="name">���O</param>
	static void Create(const Color& initColor, std::string name = "");
	/// <summary>
	/// �����_�[�^�[�Q�b�g���폜���܂��B
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns></returns>
	static void Delete(std::string name);
	
	/// <summary>
	/// �����_�[�^�[�Q�b�g���Q�Ƃ��܂��B
	/// </summary>
	/// <param name="name">���O</param>
	/// <returns>name�Ŏw�肵�������_�[�^�[�Q�b�g�̎Q��</returns>
	static RenderTarget& GetRenderTarget(std::string name)
	{
		return *pRenderTarget[name];
	}

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

