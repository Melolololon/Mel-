#pragma once
#include<vector>
#include<array>

#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

//�����_�[�^�[�Q�b�g�N���X
//�`�掞�ɓn�����ƂŁA�n���������_�[�^�[�Q�b�g�ɕ`��ł���悤�ɂ���
//�Z�b�g���Ȃ��ꍇ�́A�o�b�N�o�b�t�@�ɒ��ڂ܂���1���ڂ̃����_�[�^�[�Q�b�g�ɕ`��

class RenderTarget
{
private:
	static std::vector<RenderTarget*>pRenderTarget;
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;
	
	VertexBufferSet vertexBufferSet;

	std::array<Vertex, 4>vertices;

	ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[
	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	//�J�����̃|�C���^
	Camera* pCamera;
public:
	RenderTarget(){}
	~RenderTarget(){}

	static bool Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�𐶐����܂��B
	/// </summary>
	bool CreateRenderTarget();

	
	/// <summary>
	/// �����_�[�^�[�Q�b�g�ɃJ�������Z�b�g���܂��B�����_�[�^�[�Q�b�g�ɂ́A�Z�b�g���ꂽ�J�����ɉf���Ă�����̂��`�悳��܂��B
	/// </summary>
	/// <param name="camera">�J�����̃|�C���^</param>
	void SetCamera(Camera* pCamera){ this->pCamera = pCamera; }

};

