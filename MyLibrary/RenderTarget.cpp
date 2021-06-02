#include "RenderTarget.h"
#include"CreateBuffer.h"
#include<d3dx12.h>
#include"Library.h"

std::vector<RenderTarget*>RenderTarget::pRenderTarget;
float RenderTarget::clearColor[4] = { 0.5f,0.5f,0.5f,0.0f };
RenderTarget::RenderTarget():
	Sprite2D(nullptr)
{
	//���_�A�萔�o�b�t�@�쐬
	CreateBuffer();

	HRESULT result;

#pragma region �|���̃��\�[�X�쐬

	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_B8G8R8X8_UNORM,
		Library::GetWindowWidth(),
		Library::GetWindowHeight(),
		1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//D3D12_CLEAR_VALUE ���\�[�X�������_�[�^�[�Q�b�g�Ƃ��Ďg���ꍇ�ɂǂ����������邩���܂Ƃ߂�����
	D3D12_CLEAR_VALUE peClesrValue;
	peClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&peClesrValue,
		IID_PPV_ARGS(&textureBuffer)
	);

	//�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC peHeapDesc{};
	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	peHeapDesc.NumDescriptors = 2;
	peHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	peHeapDesc.NodeMask = 0;
	device->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&descHeap));

	//�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		textureBuffer.Get(),
		&srvDesc,
		descHeap.Get()->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion

#pragma region �����_�[�^�[�Q�b�g�q�[�v_�r���[�쐬
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap
	(
		&rtvHeapDesc,
		IID_PPV_ARGS(&rtvHeap)
	);
	assert(SUCCEEDED(result));

	device->CreateRenderTargetView
	(
		textureBuffer.Get(),
		nullptr,
		rtvHeap->GetCPUDescriptorHandleForHeapStart()
	);
#pragma endregion

#pragma region �[�x�o�b�t�@_�q�[�v_�r���[�쐬

#pragma endregion


	pipeline = defaultPipeline.GetPipelineState();
	pRenderTarget.push_back(this);
}

RenderTarget::~RenderTarget(){}
