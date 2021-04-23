#include "Model.h"
#include"CreateBuffer.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::createModelVertexResources
(
	const size_t verticesSize,
	const std::vector<size_t>& verticesNum,
	const std::vector<std::vector<USHORT>>& indicesNum
)
{
#pragma region 頂点バッファ

	//オブジェクト分リサイズ
	auto objectNum = verticesNum.size();
	vertexBufferSet.resize(objectNum);

	for (int i = 0; i < objectNum; i++)
	{
		CreateBuffer::getInstance()->createVertexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			verticesSize,
			verticesNum[i],
			vertexBufferSet[i]
		);
	}
#pragma endregion

#pragma region インデックスバッファ
	auto objectNum = indicesNum.size();
	indexBufferSet.resize(objectNum);

	for (int i = 0; i < objectNum; i++)
	{
		CreateBuffer::getInstance()->createIndexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			indicesNum[i],
			indexBufferSet[i]
		);
	}
#pragma endregion


}

void Model::createModelHeapResources
(
	const std::vector<std::wstring>& texturePath,
	const int modelNum,
	const int modelFileObjectNum,
	const size_t userDataSize = 0
)
{

	//ヒープ作成
	int constBufferNum = 3;
	if (userDataSize != 0)constBufferNum++;
	int commonBufferNum = 1;
	//テクスチャ　+ 共通バッファ + (定数バッファ - 共通バッファ) * モデル数 * モデル内のオブジェクト数
	int heapSize = 
		modelFileObjectNum
		+ commonBufferNum
		+ (constBufferNum - commonBufferNum)
		* modelNum
		* modelFileObjectNum;

	createDescriptorHeap(heapSize);

	//テクスチャバッファ作成
	createTextureBuffer
	(
		texturePath,
		0
	);

	//定数バッファ作成
	createConstBuffer
	(
		modelNum,
		modelFileObjectNum,
		modelFileObjectNum,
		userDataSize
	);

}



void Model::createDescriptorHeap
(
	const int arrayNum
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
	const int modelNum,
	const int modelFileObjectNum,
	const int heapTop,
	const size_t userDataSize = 0
)
{
	int oneModelBufferNum;

	if (userDataSize == 0)
		oneModelBufferNum = 2;
	else
		oneModelBufferNum = 3;

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

	//生成時にCreateBuffer関数内でヒープのハンドル取得するようにする?
#pragma region コモンバッファ作成

	hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetCPUDescriptorHandleForHeapStart(),
		heapNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::getInstance()->createConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(CommonConstData),
		hHandle,
		commonBuffers.Get()
	);
	heapTags.push_back(TAG_COMMON_CONST_BUFFER);
	heapNum++;
#pragma endregion

	HeapBufferTag tags[3] =
	{
		HeapBufferTag::TAG_LIBRARY_CONST_BUFFER,
		HeapBufferTag::TAG_MATERIAL_CONST_BUFFER,
		HeapBufferTag::TAG_USER_CONST_BUFFER
	};

	size_t constDataSize[3] =
	{
		sizeof(ModelConstData),
		sizeof(Material),
		userDataSize
	};

	//モデル分ループ
	for (int i = 0; i < modelNum; i++) 
	{
		//モデル内のオブジェクト分ループ
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			//1つのモデル内のオブジェクトのバッファ分ループ
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
					constDataSize[k],
					hHandle,
					constBuffer[i][j][k].Get()
				);
				heapTags.push_back(tags[k]);
				heapNum++;
			}
		}
	}
}

void Model::createTextureBuffer
(
	const std::vector<std::wstring>& texturePath,
	const int heapTop
)
{
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchimage{};
	const DirectX::Image* imgs;

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;

	auto textureNum = texturePath.size();
	for (int i = 0; i < textureNum;i++)
	{
		hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetCPUDescriptorHandleForHeapStart(),
			heapTop + i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		imgs = DirectXTexLoader::loadTexture
		(
			texturePath[i].c_str(),
			&metadata,
			&scratchimage
		);

		CreateBuffer::getInstance()->createTextureBuffer
		(
			metadata,
			imgs,
			hHandle,
			textureBuffer[i].Get()
		);
		heapTags.push_back(HeapBufferTag::TAG_TEXTURE_BUFFER);
	}
}

#pragma region マップ

void Model::mapVertexBuffer
(
	const int modelNum,
	void** vertexData
)
{
	vertexBufferSet[modelNum].vertexBuffer->Map(0, nullptr, vertexData);
}

void Model::unmapVertexBuffer(const int& modelNum)
{
	vertexBufferSet[modelNum].vertexBuffer->Unmap(0, nullptr);
}

void Model::mapIndexBuffer
(
	const int modelNum,
	void** index
)
{
	indexBufferSet[modelNum].indexBuffer->Map(0, nullptr, index);
}

void Model::unmapIndexBuffer(const int& modelNum)
{
	indexBufferSet[modelNum].indexBuffer->Unmap(0, nullptr);
}
//
//
//void Model::mapConstBuffer
//(
//	const int modelNum,
//	const int objectNum,
//	const Model::ConstBufferType type,
//	void** constData
//)
//{
//	//並び確認
//	int otherBufferNum = 0;
//	int libraryNum = -1;
//	int materialNum = -1;
//	int userNum = -1;
//	for(const auto& tag: heapTags)
//	{
//		switch (tag)
//		{
//		case HeapBufferTag::TAG_LIBRARY_CONST_BUFFER:
//			libraryNum = otherBufferNum;
//			break;
//		case HeapBufferTag::TAG_MATERIAL_CONST_BUFFER:
//			materialNum = otherBufferNum;
//			break;
//		case HeapBufferTag::TAG_USER_CONST_BUFFER:
//			userNum = otherBufferNum;
//			break;
//
//		default:
//			otherBufferNum++;
//			break;
//
//
//		}
//		if (libraryNum != -1 &&
//			materialNum != -1 &&
//			userNum != -1)
//			break;
//		
//	}
//
//	//添え字計算
//	int bufferNum = 0;
//	switch (type)
//	{
//	case ConstBufferType::CONSTBUFFER_LIBRARY:
//		bufferNum = libraryNum - otherBufferNum;
//		break;
//	case ConstBufferType::CONSTBUFFER_MATERIAL:
//		bufferNum = materialNum - otherBufferNum;
//		break;
//	case ConstBufferType::CONSTBUFFER_USER:
//		bufferNum = userNum - otherBufferNum;
//		break;
//
//	default:
//		otherBufferNum++;
//		break;
//
//
//	}
//
//	constBuffer[modelNum][objectNum][bufferNum]->Map
//	(0, nullptr, constData);
//}
#pragma endregion
