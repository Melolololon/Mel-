#pragma once
#include<vector>
#include<array>

#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"

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

	ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`�� + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[
	ComPtr<ID3D12DescriptorHeap>rtvHeap;
public:
	RenderTarget(){}
	~RenderTarget(){}

	static bool Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* cmdList);

	bool CreateRenderTarget();


};

