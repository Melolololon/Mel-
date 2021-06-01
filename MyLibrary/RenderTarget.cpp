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
	pipelineData.cullMode = CULL_NONE;
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
	


#pragma region 描画先板ポリのリソース作成
//	postEfectConstBuffers.resize(1);
//	postEfectConstBuffers.reserve(20);
//
//	//D3D12_CLEAR_VALUE リソースをレンダーターゲットとして使う場合にどう初期化するかをまとめたもの
//	D3D12_CLEAR_VALUE peClesrValue;
//
//
//
//	peClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
//	//リソース作成
//	postEffectResources.resize(1);
//
//	dev->CreateCommittedResource
//	(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
//		D3D12_HEAP_FLAG_NONE,
//		&backBuffer[0].Get()->GetDesc(),
//		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
//		&peClesrValue,
//		IID_PPV_ARGS(&postEffectResources[0])
//	);
//
//#pragma region ヒープとビュー作成
//	//テクスチャ
//
//	//ヒープ作成
//	D3D12_DESCRIPTOR_HEAP_DESC peHeapDesc{};
//	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	peHeapDesc.NumDescriptors = 2;
//	peHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	peHeapDesc.NodeMask = 0;
//	dev->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&postEffectHeap));
//
//	//ビュー作成
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//
//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = 1;
//
//	dev->CreateShaderResourceView
//	(
//		postEffectResources[0].Get(),
//		&srvDesc,
//		postEffectHeap.Get()->GetCPUDescriptorHandleForHeapStart()
//	);
//
//	//定数バッファ作成
//	postEffectWorldMatData.resize(1);
//	postEffectWorldMatData[0].scale = { 1,1,1 };
//
//	postEfectConstBuffers[0].constBuffer.resize(1);
//	createBuffer->CreateConstBufferSet(
//		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		CD3DX12_RESOURCE_DESC::Buffer((sizeof(PostEffectConstData) + 0xff) & ~0xff),
//		CD3DX12_CPU_DESCRIPTOR_HANDLE
//		(
//			postEffectHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
//			1,
//			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//		),
//		(void**)&postEffectConstDataP,
//		postEfectConstBuffers[0],
//		0);
//	postEffectConstDataP->worldMat = DirectX::XMMatrixIdentity();
//	//postEfectConstBuffers[0].constBuffer[0].Get()->Unmap(0, nullptr);
//
//	//レンダーターゲット
//
//	//ヒープ作成
//	peHeapDesc = {};
//	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	peHeapDesc.NumDescriptors = 10;
//	dev->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&postEffectRTVHeap));
//
//	//ビュー作成
//	D3D12_RENDER_TARGET_VIEW_DESC peRTVDesc = {};
//	peRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//	peRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//
//	dev.Get()->CreateRenderTargetView
//	(
//		postEffectResources[0].Get(),
//		&peRTVDesc,
//		postEffectRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart()
//	);
#pragma endregion

#pragma endregion




	pRenderTarget.push_back(this);
	return true;
}
