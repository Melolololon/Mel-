#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>
#include<unordered_map>
#include<memory>
enum class PipelineType
{
	MODEL,
	SPRITE,
	RENDER_TARGET,
};
class PipelineState
{

private:
	static std::unordered_map<std::string, std::unique_ptr<PipelineState>>pPipelineState;

	ComPtr<ID3D12PipelineState>pipeline;
	static ID3D12Device* device;

	static void SetPipelineDesc
	(
		const PipelineData& pipelineData,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc,
		const int renderTargetNum
	);


	std::string modelClassName;

	static ID3D12RootSignature* modelRootSignature;
	static ID3D12RootSignature* spriteRootSignature;
	static ID3D12RootSignature* renderTargetRootSignature;


public:
	PipelineState();
	~PipelineState();

	static bool Initialize
	(
		ID3D12Device* dev
	);

	static void Create
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gShaderData,
		const ShaderData& hShaderData,
		const ShaderData& dShaderData,
		const ShaderData& pShaderData,
		const PipelineType pipelineType,
		const std::vector<InputLayoutData>* inputLayoutData,
		const std::string& modelClassName,
		const int renderTargetNum,
		const std::string& name
	);
	static PipelineState* Get(const std::string& name) { return pPipelineState[name].get(); }
	static void Delete(const std::string& name);

	/// <summary>
	/// �p�C�v���C���𐶐����܂�
	/// </summary>
	/// <param name="pipelineData">�p�C�v���C�����</param>
	/// <param name="vShaderData">���_�V�F�[�_�[���</param>
	/// <param name="gSyaderData">�W�I���g���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="hShaderData">�n���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="dShaderData">�h���C���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
	/// <param name="pShaderData">�s�N�Z���V�F�[�_�[���</param>
	/// <param name="pipelineType">���̃p�C�v���C���𐶐����邩</param>
	/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
	/// <param name="modelClassName">typeid��name�֐��Ŏ擾����������</param>
	/// <param name="renderTargetNum">�����ɂǂ̂��炢�o�͂��邩</param>
	/// <returns></returns>
	bool CreatePipeline
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gShaderData,
		const ShaderData& hShaderData,
		const ShaderData& dShaderData,
		const ShaderData& pShaderData,
		const PipelineType pipelineType,
		const std::vector<InputLayoutData>* inputLayoutData,
		const std::string& modelClassName,
		const int renderTargetNum
	);



	std::string GetModelClassName()
	{
		return modelClassName;
	}

	ComPtr<ID3D12PipelineState> GetPipelineState()
	{
		return pipeline;
	}

	/// <summary>
	/// ���C�u�����Ŏg�p���Ă���p�C�v���C���̏�������data�ɃZ�b�g���܂��B
	/// </summary>
	/// <param name="data"></param>
	/// <param name="type"></param>
	static void GetDefaultPipelineData(PipelineData& data, const PipelineType type);

	static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }
	static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }
	static void SetRenderTargetRootSignature(ID3D12RootSignature* sig) { renderTargetRootSignature = sig; }

};

