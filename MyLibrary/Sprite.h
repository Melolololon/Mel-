#pragma once
#include"DirectXStruct.h"

#include<d3d12.h>
#include<dxgi.h>
#include<vector>

//������p������2D�A3D�X�v���C�g�N���X�����
//Draw�͉��z�֐��B���ꂼ���map����������
//

class Sprite
{
private:

	static DirectX::XMMATRIX sprite2DCameraMatrix;
	static DirectX::XMMATRIX sprite3DCameraMatrix;

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;

	

	//�C���f�b�N�X�́A�X�v���C�g���o���o���ɂ����肷�鏈���Ɏg������������������Ƃ�
	VertexBufferSet vertexBufferSet;
	IndexBufferSet indexBufferSet;

	//�S�X�v���C�g�Ńe�N�X�`���q�[�v���L����(��������V�F�[�_�[���\�[�X�r���[�������肾��)
	static ComPtr<ID3D12DescriptorHeap>desHeap;
	ComPtr<ID3D12Resource> constBuffer;
	//�X�v���C�g�S�̂Ńo�b�t�@���L����
	static std::vector<ComPtr<ID3D12Resource>>textureBuffer;

	std::string modelClassName;
protected:
	ComPtr<ID3D12PipelineState> pipeline;
	ID3D12PipelineState* currentSetPipeline;

public:
	Sprite();
	~Sprite();

#pragma region �J���җp�֐�

	
#pragma endregion


#pragma region �����ڑ���
	//AddColor�Ƃ�
#pragma endregion

	/// <summary>
	/// �X�v���C�g��`�悵�܂��B
	/// </summary>
	virtual void Draw();
};

