#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>

enum PipelineType
{
	PIPELINE_TYPE_MODEL,
	PIPELINE_TYPE_SPRITE,
	PIPELINE_TYPE_RENDER_TARGET,
};
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


	std::string modelClassName;

	static ID3D12RootSignature* modelRootSignature;
	static ID3D12RootSignature* spriteRootSignature;


public:
	PipelineState();
	~PipelineState();

	static bool Initialize
	(
		ID3D12Device* dev
	);


	/// <summary>
	/// パイプラインを生成します
	/// </summary>
	/// <param name="pipelineData">パイプライン情報</param>
	/// <param name="vShaderData">頂点シェーダー情報</param>
	/// <param name="gSyaderData">ジオメトリシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
	/// <param name="hShaderData">ハルシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
	/// <param name="dShaderData">ドメインシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
	/// <param name="pShaderData">ピクセルシェーダー情報</param>
	/// <param name="pipelineType">何のパイプラインを生成するか</param>
	/// <param name="inputLayoutData">インプットレイアウト情報(指定しない場合は、nullptrを渡す)</param>
	/// <param name="modelClassName">typeidのname関数で取得した文字列</param>
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
		const std::string& modelClassName
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
	static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }

};

