#include "RenderTarget.h"
#include"CreateBuffer.h"
#include<d3dx12.h>
#include"Library.h"

std::unordered_map<std::string, std::unique_ptr<RenderTarget>> RenderTarget::pRenderTargets;
UINT RenderTarget::createCount = 0;
std::string RenderTarget::mainRenderTargetNama = "";

PipelineState RenderTarget::defaultPipeline;
ComPtr<ID3D12RootSignature>RenderTarget::rootSignature;



RenderTarget::RenderTarget(const Color& color) :
	Sprite2D(color)
{
	//���_�A�萔�o�b�t�@�쐬
	CreateBuffer();
	InitializeVertices();



	HRESULT result;

#pragma region �e�N�X�`��_RT_�q�[�v�쐬


	D3D12_DESCRIPTOR_HEAP_DESC texHeapDesc{};
	texHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	texHeapDesc.NumDescriptors = RT_NUM;
	texHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	texHeapDesc.NodeMask = 0;
	result = device->CreateDescriptorHeap
	(
		&texHeapDesc,
		IID_PPV_ARGS(&textureDescHeap)
	);
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = RT_NUM;
	result = device->CreateDescriptorHeap
	(
		&rtvHeapDesc,
		IID_PPV_ARGS(&rtvDescHeap)
	);
	assert(SUCCEEDED(result));


	CD3DX12_RESOURCE_DESC texRTDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		Library::GetWindowWidth(),
		Library::GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);


#pragma endregion


	//�F�Z�b�g
	clearColor[0] = (float)color.r / 255;
	clearColor[1] = (float)color.g / 255;
	clearColor[2] = (float)color.b / 255;
	clearColor[3] = (float)color.a / 255;

	//D3D12_CLEAR_VALUE ���\�[�X�������_�[�^�[�Q�b�g�Ƃ��Ďg���ꍇ�ɂǂ����������邩���܂Ƃ߂�����
	D3D12_CLEAR_VALUE renderTargetClearValue;
	renderTargetClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	for (int i = 0; i < RT_NUM; i++)
	{
		result = device->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&texRTDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&renderTargetClearValue,
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
			textureDescHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
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
			rtvDescHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
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


}

RenderTarget::~RenderTarget()
{
}


void RenderTarget::Create(const Color& initColor, const std::string& name)
{
	//�z��ɒǉ�
	std::string keyName = name;
	if (keyName.size() == 0)keyName = "RenderTarget_" + std::to_string(createCount);
	pRenderTargets.emplace(keyName, std::make_unique<RenderTarget>(initColor));
	createCount++;
	if (mainRenderTargetNama.size() == 0)mainRenderTargetNama = name;
}

void RenderTarget::Delete(const std::string& name)
{
	pRenderTargets.erase(name);
}

bool RenderTarget::Initialize()
{
	//
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1{};
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV2{};
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);


	CD3DX12_ROOT_PARAMETER rootparam[3] = {};
	
	rootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparam[1].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_PIXEL);
	rootparam[2].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_PIXEL);
	


#pragma region ���[�g�V�O�l�`��


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



	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetRenderTargetRootSignature(rootSignature.Get());



	PipelineData data;
	PipelineState::GetDefaultPipelineData(data, PipelineType::PIPELINE_TYPE_SPRITE);

	bool bResult = defaultPipeline.CreatePipeline
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
	if (!bResult)
	{
		OutputDebugString(L"RenderTarget�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł���\n");
		return false;
	}
	return true;
}


void RenderTarget::PreDrawProcess()
{
	//�؂�ւ�
	for (int i = 0; i < RT_NUM; i++)
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
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle[RT_NUM];
	for (int i = 0; i < RT_NUM; i++)
	{
		rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			rtvDescHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(RT_NUM, rtvHandle, false, &dsvHandle);

	//�N���A
	for (int i = 0; i < RT_NUM; i++)
	{
		cmdList->ClearRenderTargetView(rtvHandle[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//�r���[�|�[�g�V�U�[��`�Z�b�g
	D3D12_VIEWPORT viewport[RT_NUM]{};
	D3D12_RECT scissorRect[RT_NUM]{};

	for (int i = 0; i < RT_NUM; i++)
	{
		viewport[i].Width = Library::GetWindowWidth();
		viewport[i].Height = Library::GetWindowHeight();
		viewport[i].TopLeftX = 0;
		viewport[i].TopLeftY = 0;
		viewport[i].MinDepth = 0.0f;
		viewport[i].MaxDepth = 1.0f;
		


		scissorRect[i].left = 0;
		scissorRect[i].right = scissorRect[i].left + Library::GetWindowWidth();
		scissorRect[i].top = 0;
		scissorRect[i].bottom = scissorRect[i].top + Library::GetWindowHeight();
		
	}

	cmdList->RSSetViewports(RT_NUM, viewport);
	cmdList->RSSetScissorRects(RT_NUM, scissorRect);
}


void RenderTarget::SetCmdList()
{
#pragma region ���ʂ�ʂ̃e�N�X�`���o�b�t�@�ɕ`��


	//�߂�
	for (int i = 0; i < RT_NUM; i++)
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

	cmdList->SetPipelineState(pipeline.Get());

	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	//���_
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet.vertexBufferView);

	ppHeaps.push_back(textureDescHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);


	//�萔
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());

#pragma region ���݂̉��

	//�e�N�X�`��1
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		textureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		renderingRTNum * RT_NUM,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	//�e�N�X�`��2
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		textureDescHeap->GetGPUDescriptorHandleForHeapStart(),
		renderingRTNum * RT_NUM + 1,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandle);

	

	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);

#pragma endregion

#pragma endregion


	
	
}

void RenderTarget::Draw()
{


	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	ConstDataMat();
	MatrixMap(nullptr);
	SetCmdList();
}

void RenderTarget::AllDraw()
{
	//���ʂ̂���Z�b�g
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	//�����Ŋe�����_�[�^�[�Q�b�g��Draw���Ăяo��
	for (auto& p : pRenderTargets)
	{
		p.second->ConstDataMat();
		p.second->MatrixMap(nullptr);
		p.second->SetCmdList();
	}
}

