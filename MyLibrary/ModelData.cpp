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

template<class VERTEX>
void ModelData::CreateVertexBufferSet
(
	const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<VERTEX>>& vertices
)
{
	
	modelFileObjectNum = vertices.size();

	//オブジェクト分リサイズ
	vertexBufferSet.resize(modelFileObjectNum);

	for (int i = 0; i < modelFileObjectNum; i++)
	{
		CreateBuffer::GetInstance()->CreateVertexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			vertexSize,
			vertexNum[i],
			vertexBufferSet[i]
		);
	}


}
template void ModelData::CreateVertexBufferSet<Vertex>
(const size_t vertexSize, const std::vector<size_t>& vertexNum, const std::vector<std::vector<Vertex>>& vertices);
template void ModelData::CreateVertexBufferSet<ObjAnimationVertex>
(const size_t vertexSize, const std::vector<size_t>& vertexNum, const std::vector<std::vector<ObjAnimationVertex>>& vertices);
template void ModelData::CreateVertexBufferSet<FbxVertex>
(const size_t vertexSize, const std::vector<size_t>& vertexNum, const std::vector<std::vector<FbxVertex>>& vertices);



template<class VERTEX>
void ModelData::MapVertices(const std::vector<std::vector<VERTEX>>& vertices)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		//Map
		VERTEX* pVertices;
		vertexBufferSet[i].vertexBuffer->Map(0, nullptr, (void**)&pVertices);
		std::copy(vertices[i].begin(), vertices[i].end(), pVertices);
		vertexBufferSet[i].vertexBuffer->Unmap(0, nullptr);
	}
}
template void ModelData::MapVertices<Vertex>
( const std::vector<std::vector<Vertex>>& vertices);
template void ModelData::MapVertices<ObjAnimationVertex>
(const std::vector<std::vector<ObjAnimationVertex>>& vertices);
template void ModelData::MapVertices<FbxVertex>
( const std::vector<std::vector<FbxVertex>>& vertices);



void ModelData::CreateIndexBufferSet(const std::vector<std::vector<USHORT>>& indices)
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

void ModelData::MapIndices(const std::vector<std::vector<USHORT>>& indices)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		//Map
		USHORT* pIndices;
		indexBufferSet[i].indexBuffer->Map(0, nullptr, (void**)&pIndices);
		std::copy(indices[i].begin(), indices[i].end(), pIndices);
		indexBufferSet[i].indexBuffer->Unmap(0, nullptr);
	}
}

void ModelData::CteateTextureBufferAndView()
{
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

void ModelData::CteateTextureBufferAndView()
{

	CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
	(
		Color(0, 0, 0, 0), 
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureDescHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		&textureBuffers[0]
	);

}



template<class VERTEX>
void ModelData::BufferPreparationSetTexture
(
	const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<VERTEX>>& vertices,
	const std::vector<std::vector<USHORT>>& indices
)
{
	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum,
		vertices
	);
	MapVertices(vertices);

	CreateIndexBufferSet(indices);
	MapIndices(index);

	CreateDescriptorHeap(pTextures.size());
	CteateTextureBufferAndView(pTextures);


}
template void ModelData::BufferPreparationSetTexture<Vertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<Vertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);
template void ModelData::BufferPreparationSetTexture<ObjAnimationVertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<ObjAnimationVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);
template void ModelData::BufferPreparationSetTexture<FbxVertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<FbxVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);



template<class VERTEX>
void ModelData::BufferPreparationSetColor
(
	const size_t vertexSize,
	const  std::vector<size_t>& vertexNum,
	const std::vector<std::vector<VERTEX>>& vertices,
	const std::vector<std::vector<USHORT>>& indices
)
{

	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum,
		vertices
	);
	MapVertices(vertices);

	CreateIndexBufferSet(indices);
	MapIndices(index);

	CreateDescriptorHeap(1);
	CteateTextureBufferAndView(Color(0, 0, 0, 0));


}
template void ModelData::BufferPreparationSetColor<Vertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<Vertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);
template void ModelData::BufferPreparationSetColor<ObjAnimationVertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<ObjAnimationVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);
template void ModelData::BufferPreparationSetColor<FbxVertex>
(const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<FbxVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices);
