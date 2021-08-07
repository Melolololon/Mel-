#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>
#include<unordered_map>
#include<memory>

//パイプラインは別にモデルみたいにstaticのアクセス関数とかいらない?
//それとも、シーンをまたいで使う可能性あるからいる?

namespace MelLib
{
	enum class PipelineStateType
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
			const PipelineStateType type,
			const int renderTargetNum
		);


		std::string modelClassName;

		static ID3D12RootSignature* modelRootSignature;
		static ID3D12RootSignature* spriteRootSignature;
		static ID3D12RootSignature* renderTargetRootSignature;


	public:
		PipelineState() {}
		~PipelineState() {}

		static bool Initialize
		(
			ID3D12Device* dev
		);

		/// <summary>
		/// パイプラインを生成します
		/// </summary>
		/// <param name="pipelineData">パイプライン情報</param>
		/// <param name="vShaderData">頂点シェーダー情報</param>
		/// <param name="hShaderData">ハルシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="dShaderData">ドメインシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="gShaderData">ジオメトリシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="pShaderData">ピクセルシェーダー情報</param>
		/// <param name="pipelineType">何のパイプラインを生成するか</param>
		/// <param name="inputLayoutData">インプットレイアウト情報(指定しない場合は、nullptrを渡す)</param>
		/// <param name="modelClassName">typeidにModelObjectなどを渡し、name関数で取得した文字列(誤セット防止に使用)</param>
		/// <param name="renderTargetNum">同時にどのくらい出力するか</param>
		/// <param name="name">登録名</param>
		/// <returns></returns>
		static void Create
		(
			const PipelineData& pipelineData,
			const ShaderData& vShaderData,
			const ShaderData& hShaderData,
			const ShaderData& dShaderData,
			const ShaderData& gShaderData,
			const ShaderData& pShaderData,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const std::string& modelClassName,
			const int renderTargetNum,
			const std::string& name
		);

		static PipelineState* Get(const std::string& name) { return pPipelineState[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// パイプラインを生成します
		/// </summary>
		/// <param name="pipelineData">パイプライン情報</param>
		/// <param name="vShaderData">頂点シェーダー情報</param>
		/// <param name="hShaderData">ハルシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="dShaderData">ドメインシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="gShaderData">ジオメトリシェーダー情報(指定しない場合は、パスに"NULL"を記述)</param>
		/// <param name="pShaderData">ピクセルシェーダー情報</param>
		/// <param name="pipelineType">何のパイプラインを生成するか</param>
		/// <param name="inputLayoutData">インプットレイアウト情報(指定しない場合は、nullptrを渡す)</param>
		/// <param name="modelClassName">typeidにModelObjectなどを渡し、name関数で取得した文字列(誤セット防止に使用)</param>
		/// <param name="renderTargetNum">同時にどのくらい出力するか</param>
		/// <returns></returns>
		bool CreatePipeline
		(
			const PipelineData& pipelineData,
			const ShaderData& vShaderData,
			const ShaderData& hShaderData,
			const ShaderData& dShaderData,
			const ShaderData& gShaderData,
			const ShaderData& pShaderData,
			const PipelineStateType pipelineType,
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
		/// ライブラリで使用しているパイプラインの情報を引数dataにセットします。
		/// </summary>
		/// <param name="data"></param>
		/// <param name="type"></param>
		static PipelineData GetDefaultPipelineData( const PipelineStateType type);

		static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }
		static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }
		static void SetRenderTargetRootSignature(ID3D12RootSignature* sig) { renderTargetRootSignature = sig; }

	};
}
