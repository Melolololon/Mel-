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


void Model::createDescriptorHeap
(
	const int& arrayNum
)
{
	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = arrayNum;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&desHeap));



}

void Model::createConstBuffer
(
	const size_t& constStructDataSize,
	void** constData,
	const size_t& constDataSize,
	const int& modelNum
)
{
	//これだと共通とかマテリアル作るごとにリサイズされてしまう
	constBuffer.resize(constBuffer.size() + 1);
	//生成数分リサイズ
	constBuffer[constBuffer.size() - 1].resize(modelNum);

	for (int i = 0; i < modelNum; i++) 
	{
		CreateBuffer::getInstance()->createConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			constData,
			constDataSize,
			constBuffer[constBuffer.size() - 1][i].Get(),

		);
	}
}

void Model::createTextureBuffer
(
	std::wstring path
)
{
	//DirectX::TexMetadata metadata{};
	//DirectX::ScratchImage scratchimage{};

	//const DirectX::Image* imgs;
	//imgs = DirectXTexLoader::loadTexture
	//(
	//	path.c_str(),
	//	&metadata,
	//	&scratchimage
	//);

	//CreateBuffer::getInstance()->createTextureBuffer
	//(
	//	metadata,
	//	imgs,
	//	,
	//	textureBuffer
	//);
}
