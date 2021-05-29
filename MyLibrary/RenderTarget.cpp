#include "RenderTarget.h"
#include"CreateBuffer.h"
#include<d3dx12.h>

PipelineState RenderTarget::defaultPipeline;
std::vector<RenderTarget*>RenderTarget::pRenderTarget;
ComPtr<ID3D12RootSignature>RenderTarget::rootSignature;

bool RenderTarget::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList)
{

	//レンジ
	D3D12_DESCRIPTOR_RANGE peRangeSRV{};
	peRangeSRV.NumDescriptors = 1;
	peRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	peRangeSRV.BaseShaderRegister = 0;
	peRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE peRangeCBV{};
	peRangeCBV.NumDescriptors = 1;
	peRangeCBV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	peRangeCBV.BaseShaderRegister = 0;
	peRangeCBV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータ
	D3D12_ROOT_PARAMETER rootparam[2]{};

	//テクスチャ
	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].DescriptorTable.pDescriptorRanges = &peRangeSRV;
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].DescriptorTable.pDescriptorRanges = &peRangeCBV;
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	//ルートシグネチャ

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	result = dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	if (!result)
	{
#ifdef _DEBUG
		OutputDebugString(L"RenderTargetのルートシグネチャの生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	PipelineState::SetRenderTargetRootSignature(rootSignature.Get());

	//パイプライン
	PipelineData pipelineData;
	pipelineData.alphaWriteMode = ALPHA_WRITE_TRUE;
	pipelineData.blendMode = BLEND_ADD;
	pipelineData.cullMode = CULL_BACK;
	pipelineData.depthMode = DEPTH_TRUE;
	pipelineData.drawMode = DRAW_SOLID;

	result = defaultPipeline.CreatePipeline
	(
		pipelineData,
		{ L"../MyLibrary/PostEffectVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/PostEffectPixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_RENDER_TARGET,
		nullptr,
		typeid(RenderTarget).name()
	);

	if(!result)
	{
#ifdef _DEBUG
		OutputDebugString(L"RenderTargetのパイプラインの生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}
	return true;
}

bool RenderTarget::CreateRenderTarget()
{
#pragma region 頂点関係

	vertices[0] = { DirectX::XMFLOAT3(-1,-1,0),DirectX::XMFLOAT2(0,1),DirectX::XMFLOAT3(0,0,0) };
	vertices[1] = { DirectX::XMFLOAT3(-1, 1, 0), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0, 0, 0) };
	vertices[2] = { DirectX::XMFLOAT3(1,-1,0),DirectX::XMFLOAT2(1,1),DirectX::XMFLOAT3(0,0,0) };
	vertices[3] = { DirectX::XMFLOAT3(1,1,0),DirectX::XMFLOAT2(1,0),DirectX::XMFLOAT3(0,0,0) };

	CreateBuffer::GetInstance()->CreateVertexBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(Vertex),
		4,
		vertexBufferSet
	);

#pragma endregion
	
	


	pRenderTarget.push_back(this);
	return true;
}
