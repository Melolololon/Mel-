#include "Model.h"
#include"CreateBuffer.h"

Model::Model()
{
}

Model::~Model()
{
}


void Model::createVertexBuffer
(
	const size_t& verticesSize,
	const size_t& verticesNum
)
{
	vertexBufferSet.resize(vertexBufferSet.size() + 1);
	CreateBuffer::getInstance()->createVertexBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		verticesSize,
		verticesNum,
		vertexBufferSet[vertexBufferSet.size() - 1]
	);

	
}

void Model::createIndexBuffer(const std::vector<USHORT>& indicesNum)
{
	indexBufferSet.resize(indexBufferSet.size() + 1);
	CreateBuffer::getInstance()->createIndexBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		indicesNum,
		indexBufferSet[indexBufferSet.size() - 1]
	);
}

void Model::mapVertexBuffer
(
	const int& modelNum,
	void** vertexStruct
)
{
	vertexBufferSet[modelNum].vertexBuffer->Map(0, nullptr, vertexStruct);
}

void Model::unmapVertexBuffer(const int& modelNum)
{
	vertexBufferSet[modelNum].vertexBuffer->Unmap(0, nullptr);
}
