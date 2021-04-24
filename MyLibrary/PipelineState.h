#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>

class PipelineState
{

private:
	ComPtr<ID3D12PipelineState>pipeline;
	static ID3D12Device* device;
	static ID3D12RootSignature* modelRootSignature;

	void setPipelineDesc
	(
		const PipelineData& pipelineData,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc
	);

	std::string modelClassName;
public:
	PipelineState();

	~PipelineState();

	/// <summary>
	/// パイプラインを生成します
	/// </summary>
	/// <param name="pipelineData"></param>
	/// <param name="vShaderData"></param>
	/// <param name="gSyaderData"></param>
	/// <param name="hShaderData"></param>
	/// <param name="dShaderData"></param>
	/// <param name="pShaderData"></param>
	/// <param name="inputLayoutData"></param>
	/// <param name="modelClassName">typeidのname関数で取得した文字列</param>
	/// <returns></returns>
	bool createModelPipeline
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gSyaderData,
		const ShaderData& hShaderData,
		const ShaderData& dShaderData,
		const ShaderData& pShaderData,
		const std::vector<InputLayoutData>* inputLayoutData,
		const std::string& modelClassName
	);

	bool createSpritePipeline
	(
		const PipelineData& pipelineData,
		const ShaderData& vShaderData,
		const ShaderData& gSyaderData,
		const ShaderData& pShaderData,
		const std::vector<InputLayoutData>* inputLayoutData
	);

	std::string getModelClassName()
	{
		return modelClassName;
	}

	ComPtr<ID3D12PipelineState> getPipelineState()
	{
		return pipeline;
	}

	static void setDevice(ID3D12Device* dev)
	{
		device = dev;
	}

	static void setModelRootSignature(ID3D12RootSignature* sig)
	{
		modelRootSignature = sig;
	}
};

