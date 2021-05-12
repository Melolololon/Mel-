#include "Sprite.h"
#include"CreateBuffer.h"
#include"PipelineState.h"

const UINT Sprite::MAX_TEXTURE_LOAD_NUM = 256 * 15;

ID3D12Device* Sprite::device;
ID3D12GraphicsCommandList* Sprite::cmdList;
ComPtr<ID3D12RootSignature>Sprite::rootSignature;
ComPtr<ID3D12PipelineState>Sprite::defaultPipeline;

std::vector<ComPtr<ID3D12Resource>> Sprite::textureBuffer;
ComPtr<ID3D12DescriptorHeap> Sprite::textureHeap;

Sprite::Sprite(){}

Sprite::~Sprite(){}


void Sprite::CreateBuffer()
{

	//頂点バッファ作成
	CreateBuffer::GetInstance()->CreateVertexBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(SpriteVertex),
		vertices.size(),
		vertexBufferSet
	);



	//定数バッファ作成
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(SpriteConstBufferData),
		&constBuffer
	);
	
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);
	vertices[0].uv = { 0,1 };
	vertices[1].uv = { 0,0 };
	vertices[2].uv = { 1,1 };
	vertices[3].uv = { 1,0 };
	vertex[0].uv = { 0,1 };
	vertex[1].uv = { 0,0 };
	vertex[2].uv = { 1,1 };
	vertex[3].uv = { 1,0 };
	UnmapVertexBuffer();
}


void Sprite::CommonDataMat()
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	constBufferData->addColor = constData.addColor;
	constBufferData->subColor = constData.subColor;
	constBufferData->mulColor = constData.mulColor;

	constBuffer->Unmap(0, nullptr);
}

void Sprite::SetCmdList(Texture* texture)
{

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	cmdList->SetPipelineState(pipeline.Get());


	//頂点
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet.vertexBufferView);


	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(textureHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	//テクスチャ
	UINT heapNum = texture->GetTextureNumber();
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		textureHeap->GetGPUDescriptorHandleForHeapStart(),
		heapNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);
	//定数セット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());
	

	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
	
}


void Sprite::MapVertexBuffer(void** data)
{
	vertexBufferSet.vertexBuffer->Map(0, nullptr, data);
}

void Sprite::UnmapVertexBuffer()
{
	vertexBufferSet.vertexBuffer->Unmap(0, nullptr);
}



void Sprite::Draw(Texture* texture)
{
}



void Sprite::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd)
{
	device = dev;
	cmdList = cmd;


#pragma region ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = MAX_TEXTURE_LOAD_NUM;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));
#pragma endregion


	CD3DX12_DESCRIPTOR_RANGE spriteDescRangeSRV;
	spriteDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER spriteRootparam[2] = {};
	spriteRootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	spriteRootparam[1].InitAsDescriptorTable(1, &spriteDescRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma region ルートシグネチャ


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = spriteRootparam;
	rootSignatureDesc.NumParameters = _countof(spriteRootparam);


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

	 result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetSpriteRootSignature(rootSignature.Get());
}

void Sprite::CreateTextureBuffer(Texture* texture)
{
	UINT textureNum = texture->GetTextureNumber();
	if(MAX_TEXTURE_LOAD_NUM <= textureNum)
	{
		OutputDebugString(L"テクスチャの最大読み込み数を超えました。これ以上テクスチャを読み込めません。\n");
		return;
	}

	textureBuffer.resize(textureBuffer.size() + 1);

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		textureHeap->GetCPUDescriptorHandleForHeapStart(),
		textureNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::GetInstance()->CreateTextureBuffer
	(
		texture->GetMetadata(),
		texture->GetImage(),
		hHandle,
		&textureBuffer[textureNum]
	);

	

}


