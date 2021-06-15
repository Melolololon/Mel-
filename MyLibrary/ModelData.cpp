#include "ModelData.h"

#include"CreateBuffer.h"

void ModelData::CreateDescriptorHeap(const UINT textureNum)
{
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = textureNum;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureDescHeap));

}

void ModelData::CreateVertexBuffer
(
	const size_t verticesSize,
	const std::vector<size_t>& verticesNum
)
{

	modelFileObjectNum = verticesNum.size();

	//オブジェクト分リサイズ
	vertexBufferSet.resize(modelFileObjectNum);

	for (int i = 0; i < modelFileObjectNum; i++)
	{
		CreateBuffer::GetInstance()->CreateVertexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			verticesSize,
			verticesNum[i],
			vertexBufferSet[i]
		);
	}

}

void ModelData::CreateIndexBuffer(const std::vector<std::vector<USHORT>>& indices)
{

	indexBufferSet.resize(modelFileObjectNum);

	for (int i = 0; i < modelFileObjectNum; i++)
	{
		CreateBuffer::GetInstance()->CreateIndexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			indices[i],
			indexBufferSet[i]
		);
	}
}

void ModelData::CteateTextureBuffer(const std::vector<Texture*>& pTextures)
{
	CreateDescriptorHeap(pTextures.size());

	auto textureNum = pTextures.size();
	for (int i = 0; i < textureNum; i++)
	{
		CreateBuffer::GetInstance()->CreateTextureBuffer
		(
			pTextures[i]->GetMetadata(),
			pTextures[i]->GetImage(),
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureDescHeap->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			&textureBuffers[i]
		);
	}

}

void ModelData::CteateTextureBufferSetColor(const Color& color)
{

	CreateDescriptorHeap(pTextures.size());

	CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
	(
		color,
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		&textureBuffers[0]
	);

}
