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

void Model::mapIndexBuffer
(
	const int& modelNum,
	void** index
)
{
	indexBufferSet[modelNum].indexBuffer->Map(0, nullptr, index);
}

void Model::unmapIndexBuffer(const int& modelNum)
{
	indexBufferSet[modelNum].indexBuffer->Unmap(0, nullptr);
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
	const int& modelNum, 
	const int& modelFileObjectNum,
	const int& heapTop,
	const std::vector<size_t>& constDataSize,
	std::vector<void**> constData
)
{
	auto oneModelBufferNum = constDataSize.size();
	//生成数分リサイズ
	constBuffer.resize(modelNum);
	//モデルファイルにあるオブジェクト分だけリサイズ
	for (auto c : constBuffer) 
	{
		c.resize(modelFileObjectNum);

		//バッファ分だけリサイズ
		for (auto& c2 : c)
			c2.resize(oneModelBufferNum);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;
	int heapNum = heapTop;

	//モデル分ループ
	for (int i = 0; i < modelNum; i++) 
	{
		//モデル内のオブジェクト分ループ
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			//1つのモデルのバッファ分ループ
			for (int k = 0; k < oneModelBufferNum; k++) 
			{
				hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
					(
						desHeap->GetCPUDescriptorHandleForHeapStart(),
						heapNum,
						device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
					);

				CreateBuffer::getInstance()->createConstBuffer
				(
					CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					constData[j],
					constDataSize[j],
					hHandle,
					constBuffer[i][j][k].Get()
				);
				heapNum++;
			}
		}
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
