#include "RenderTarget.h"
#include"CreateBuffer.h"
#include<d3dx12.h>
#include"Library.h"

std::vector<RenderTarget*>RenderTarget::pRenderTarget;
float RenderTarget::clearColor[4] = { 0.5f,0.5f,0.5f,0.0f };

RenderTarget::RenderTarget(const Color& color):
	Sprite2D(color)
{
	//頂点、定数バッファ作成
	CreateBuffer();
	InitializeVertices();

	HRESULT result;

	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
#pragma region 板ポリのリソース作成

		CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D
		(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			Library::GetWindowWidth(),
			Library::GetWindowHeight(),
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		//色セット
		clearColor[0] = color.r / 255;
		clearColor[1] = color.g / 255;
		clearColor[2] = color.b / 255;
		clearColor[3] = color.a / 255;

		//D3D12_CLEAR_VALUE リソースをレンダーターゲットとして使う場合にどう初期化するかをまとめたもの
		D3D12_CLEAR_VALUE peClesrValue;
		peClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

		result = device->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&peClesrValue,
			IID_PPV_ARGS(&textureBuffer[i])
		);

		//ヒープ作成
		D3D12_DESCRIPTOR_HEAP_DESC peHeapDesc{};
		peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		peHeapDesc.NumDescriptors = _countof(textureBuffer);
		peHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		peHeapDesc.NodeMask = 0;
		result = device->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&descHeap));

		//ビュー作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView
		(
			textureBuffer[i].Get(),
			&srvDesc,
			descHeap.Get()->GetCPUDescriptorHandleForHeapStart()
		);

#pragma endregion

#pragma region レンダーターゲットヒープ_ビュー作成
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.NumDescriptors = _countof(textureBuffer);

		result = device->CreateDescriptorHeap
		(
			&rtvHeapDesc,
			IID_PPV_ARGS(&rtvHeap)
		);
		assert(SUCCEEDED(result));

		device->CreateRenderTargetView
		(
			textureBuffer[i].Get(),
			nullptr,
			rtvHeap->GetCPUDescriptorHandleForHeapStart()
		);
#pragma endregion
	}

#pragma region 深度バッファ_ヒープ_ビュー作成
	//深度用ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap
	(
		&dsvHeapDesc,
		IID_PPV_ARGS(&depthHeap)
	);
	assert(SUCCEEDED(result));

	D3D12_CPU_DESCRIPTOR_HANDLE depthHeapHandle = depthHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		Library::GetWindowWidth(),
		Library::GetWindowHeight(),
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CreateBuffer::GetInstance()->CreateDepthBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), 
		depthResDesc, 
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), 
		depthHeapHandle, 
		&depthBuffer
	);


#pragma endregion


	pipeline = defaultPipeline.GetPipelineState();


	//ウィンドウサイズをスケールに
	constData.scale = DirectX::XMFLOAT2(Library::GetWindowWidth(), Library::GetWindowHeight());
	

	//配列に追加
	pRenderTarget.push_back(this);
}

RenderTarget::~RenderTarget() {}


void RenderTarget::Initialize()
{
}


void RenderTarget::PreDrawProcess()
{
	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
		//セット
		cmdList->ResourceBarrier
		(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition
			(
				textureBuffer[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET
			)
		);
	}

	//レンダーターゲットセット
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[_countof(textureBuffer)];
	for(int i = 0; i < _countof(textureBuffer);i++)
	{
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(2, rtvHandle, false, &dsvHandle);

	//画面のクリア
	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
		cmdList->ClearRenderTargetView(rtvHandle[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);



#pragma region ビューポート_シザー矩形

	D3D12_VIEWPORT viewport{};
	viewport.Width = Library::GetWindowWidth();
	viewport.Height = Library::GetWindowHeight();
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	cmdList->RSSetViewports(1, &viewport);


	D3D12_RECT scissorrect{};
	scissorrect.left = 0;
	scissorrect.right = scissorrect.left + Library::GetWindowWidth();
	scissorrect.top = 0;
	scissorrect.bottom = scissorrect.top + Library::GetWindowHeight();
	cmdList->RSSetScissorRects(1, &scissorrect);

#pragma endregion
}

void RenderTarget::Draw()
{

	ConstDataMat();
	MatrixMap(nullptr);

	//戻す
	for (int i = 0; i < _countof(textureBuffer); i++)
	{
		cmdList->ResourceBarrier
		(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition
			(
				textureBuffer[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			)
		);
	}

	//パイプラインセット
	cmdList->SetPipelineState(pipeline.Get());


	//コマンドセット
	std::vector<ID3D12DescriptorHeap*> ppHeaps;

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	//頂点
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet.vertexBufferView);

	//ヒープ
	ppHeaps.push_back(descHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	//テクスチャ
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		descHeap->GetGPUDescriptorHandleForHeapStart(),
		0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	//定数セット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());

	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
}

void RenderTarget::AllDraw()
{
	//共通のやつをセット
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	//ここで各レンダーターゲットのDrawを呼び出す
	for(auto& p : pRenderTarget)
	{
		p->Draw();
	}
}

