#include "RenderTarget.h"
#include"CreateBuffer.h"
#include<d3dx12.h>
#include"Library.h"

std::vector<RenderTarget*>RenderTarget::pRenderTarget;
PipelineState RenderTarget::defaultPipeline;
float RenderTarget::clearColor[4] = { 0.5f,0.5f,0.5f,0.0f };

RenderTarget::RenderTarget(const Color& color):
	Sprite2D(color)
{
	//���_�A�萔�o�b�t�@�쐬
	CreateBuffer();
	InitializeVertices();

	HRESULT result;



	//�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC peHeapDesc{};
	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	peHeapDesc.NumDescriptors = _countof(textureBuffer);
	peHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	peHeapDesc.NodeMask = 0;
	result = device->CreateDescriptorHeap
	(
		&peHeapDesc, 
		IID_PPV_ARGS(&descHeap)
	);
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = _countof(textureBuffer);
	result = device->CreateDescriptorHeap
	(
		&rtvHeapDesc,
		IID_PPV_ARGS(&rtvHeap)
	);
	assert(SUCCEEDED(result));


	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		Library::GetWindowWidth(),
		Library::GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�F�Z�b�g
	clearColor[0] = color.r / 255;
	clearColor[1] = color.g / 255;
	clearColor[2] = color.b / 255;
	clearColor[3] = color.a / 255;

	//D3D12_CLEAR_VALUE ���\�[�X�������_�[�^�[�Q�b�g�Ƃ��Ďg���ꍇ�ɂǂ����������邩���܂Ƃ߂�����
	D3D12_CLEAR_VALUE peClesrValue;
	peClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);


	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
	
		result = device->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&peClesrValue,
			IID_PPV_ARGS(&textureBuffer[i])
		);

		//�r���[�쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			descHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		device->CreateShaderResourceView
		(
			textureBuffer[i].Get(),
			&srvDesc,
			handle
		);



		handle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);

		device->CreateRenderTargetView
		(
			textureBuffer[i].Get(),
			nullptr,
			handle
		);
	}

#pragma region �[�x�o�b�t�@_�q�[�v_�r���[�쐬
	//�[�x�p�q�[�v�쐬
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


	//�E�B���h�E�T�C�Y���X�P�[����
	constData.scale = DirectX::XMFLOAT2(Library::GetWindowWidth(), Library::GetWindowHeight());
	

	//�z��ɒǉ�
	pRenderTarget.push_back(this);
}

RenderTarget::~RenderTarget() {}


bool RenderTarget::Initialize()
{
	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_NONE;
	data.depthMode = DEPTH_NONE;
	data.drawMode = DRAW_SOLID;

	auto result = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_SPRITE,
		nullptr,
		typeid(RenderTarget).name(),
		1
	);
	if (!result)
	{
		OutputDebugString(L"RenderTarget�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł���\n");
		return false;
	}
	return true;
}


void RenderTarget::PreDrawProcess()
{
	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
		//�Z�b�g
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

	//�����_�[�^�[�Q�b�g�Z�b�g
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

	//��ʂ̃N���A
	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
		cmdList->ClearRenderTargetView(rtvHandle[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);



#pragma region �r���[�|�[�g_�V�U�[��`

	D3D12_VIEWPORT viewport[_countof(textureBuffer)]{};
	D3D12_RECT scissorRect[_countof(textureBuffer)]{};
	for (int i = 0; i < _countof(textureBuffer); i++) 
	{
		viewport[i].Width = Library::GetWindowWidth();
		viewport[i].Height = Library::GetWindowHeight();
		viewport[i].TopLeftX = 0;
		viewport[i].TopLeftY = 0;
		viewport[i].MinDepth = 0.0f;
		viewport[i].MaxDepth = 1.0f;
		cmdList->RSSetViewports(1, &viewport[i]);


		scissorRect[i].left = 0;
		scissorRect[i].right = scissorRect[i].left + Library::GetWindowWidth();
		scissorRect[i].top = 0;
		scissorRect[i].bottom = scissorRect[i].top + Library::GetWindowHeight();
		cmdList->RSSetScissorRects(1, &scissorRect[i]);
	}
#pragma endregion
}

void RenderTarget::Draw()
{

	ConstDataMat();
	MatrixMap(nullptr);

	//�߂�
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

	//�p�C�v���C���Z�b�g
	cmdList->SetPipelineState(pipeline.Get());


	//�R�}���h�Z�b�g
	std::vector<ID3D12DescriptorHeap*> ppHeaps;

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	//���_
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet.vertexBufferView);

	//�q�[�v
	ppHeaps.push_back(descHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	//�e�N�X�`��
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		descHeap->GetGPUDescriptorHandleForHeapStart(),
		0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	//�萔�Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());

	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
}

void RenderTarget::AllDraw()
{
	//���ʂ̂���Z�b�g
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	//�����Ŋe�����_�[�^�[�Q�b�g��Draw���Ăяo��
	for(auto& p : pRenderTarget)
	{
		p->Draw();
	}
}

