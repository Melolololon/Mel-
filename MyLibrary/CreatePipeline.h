#pragma once
#include<vector>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dcompiler.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#include"DirectXStruct.h"
#include"ShaderData.h"

enum FORMAT_TYPE
{
	FORMAT_TYPE_UNSIGNED_INT,
	FORMAT_TYPE_SIGNED_INT,
	FORMAT_TYPE_FLOAT,
};

//�p�C�v���C���쐬����
struct InputLayoutData
{
	//�Z�}���e�B�N�X��
	const char* semantics;

	//����l�̐�
	UINT number;

	//���鐔�l�̌^
	FORMAT_TYPE formatType;
};


class CreatePipeline
{
private:
	ID3D12Device* dev;
	
	D3D12_INPUT_ELEMENT_DESC inputLayout[3];
	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[2];

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutVector;
	D3D12_INPUT_ELEMENT_DESC* userInputLayout;

public:
	CreatePipeline(ID3D12Device* dev);
	~CreatePipeline();

	void createRootSigneture(D3D12_ROOT_SIGNATURE_DESC rootSigDesc, ID3D12RootSignature** rootSigneture);
	void createPipeline(int dimention, D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc, ID3D12PipelineState** pipeline);
	
	bool createUserPipeline
	(
		int dimention,
		ShaderData vShaderData,
		ShaderData gSyaderData,
		ShaderData pShaderData,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc, 
		ID3D12PipelineState** pipeline,
		bool useUserInputLayout);

	//�C���v�b�g���C�A�E�g�̃Z�b�g�ƁA�p�C�v���C���̍쐬�܂Ƃ߂Ă���������
	//bool�̕�����std::vector<InputLayoutData>*�ɂ���?

	/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�����܂�
/// </summary>
/// <param name="semantics">�Z�}���e�B�N�X��</param>
/// <param name="num">�f�[�^��(Vector3�̏ꍇ�A�O�����Ȃ̂�3�����)</param>
	void setInputLayout(const std::vector<InputLayoutData>& inputLayoutData);

	/// <summary>
/// ���݂̃C���v�b�g���C�A�E�g���폜���܂�
/// </summary>
	void deleteInputLayout();
};
