#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>

class PipelineState
{

private:
	ComPtr<ID3D12PipelineState>pipeline;
	static ID3D12Device* device;

	static void SetPipelineDesc
	(
		const PipelineData& pipelineData,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc
	);

	static bool CreatePipelineState
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gShaderData,
		const ShaderData& hShaderData,
		const ShaderData& dShaderData,
		const ShaderData& pShaderData,
		const std::vector<InputLayoutData>* inputLayoutData,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState** pipelineState
	);

	std::string modelClassName;

	static ID3D12RootSignature* modelRootSignature;


public:
	PipelineState();
	~PipelineState();

	static bool Initialize
	(
		ID3D12Device* dev
	);


	/// <summary>
	/// �p�C�v���C���𐶐����܂�
	/// </summary>
	/// <param name="pipelineData">�p�C�v���C�����</param>
	/// <param name="vShaderData">���_�V�F�[�_�[���</param>
	/// <param name="gSyaderData">�W�I���g���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="hShaderData">�n���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="dShaderData">�h���C���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="pShaderData">�s�N�Z���V�F�[�_�[���</param>
	/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
	/// <param name="modelClassName">typeid��name�֐��Ŏ擾����������</param>
	/// <returns></returns>
	bool CreateModelPipeline
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gShaderData,
		const ShaderData& hShaderData,
		const ShaderData& dShaderData,
		const ShaderData& pShaderData,
		const std::vector<InputLayoutData>* inputLayoutData,
		const std::string& modelClassName
	);

	bool CreateSpritePipeline
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gShaderData,
		const ShaderData& pShaderData,
		const std::vector<InputLayoutData>* inputLayoutData
	);

	std::string GetModelClassName()
	{
		return modelClassName;
	}

	ComPtr<ID3D12PipelineState> GetPipelineState()
	{
		return pipeline;
	}

	static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }

};

