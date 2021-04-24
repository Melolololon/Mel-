#include "Model.h"
#include"CreateBuffer.h"
#include"PipelineState.h"

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

	modelClassName = typeid(*this).name();
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


	ModelConstData* modelConstData;
	//モデル分ループ
	for (int i = 0; i < modelNum; i++)
	{
		//モデル内のオブジェクト分ループ
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			//1つのモデル内のオブジェクトのバッファ分ループ
			for (int k = 0; k < constBufferNum - commonBufferNum; k++)
			{
				constBuffer[i][j][k]->Map(0,nullptr,(void**)&modelConstData);
				
				constBuffer[i][j][k]->Unmap(0, nullptr);
			}
		}
	}
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
	int constBufferNum;
	int commonBufferNum = 1;

	if (userDataSize == 0)
		constBufferNum = 2;
	else
		constBufferNum = 3;

	//生成数分リサイズ
	constBuffer.resize(modelNum);
	//モデルファイルにあるオブジェクト分だけリサイズ
	for (auto c : constBuffer) 
	{
		c.resize(modelFileObjectNum);

		//バッファ分だけリサイズ
		for (auto& c2 : c)
			c2.resize(constBufferNum - commonBufferNum);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;
	int heapNum = heapTop;

#pragma region コモンバッファビュー作成

	hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetCPUDescriptorHandleForHeapStart(),
		heapNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::getInstance()->createConstBufferView
	(
		hHandle,
		commonBuffers.Get()
	);

	heapTags.push_back(HEAP_TAG_COMMON_CONST_BUFFER);
	heapNum++;
#pragma endregion

	HeapBufferTag tags[3] =
	{
		HeapBufferTag::HEAP_TAG_LIBRARY_CONST_BUFFER,
		HeapBufferTag::HEAP_TAG_MATERIAL_CONST_BUFFER,
		HeapBufferTag::HEAP_TAG_USER_CONST_BUFFER
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
			//モデル内のオブジェクトのバッファ分ループ
			for (int k = 0; k < constBufferNum - commonBufferNum; k++)
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
					constBuffer[i][j][k].Get()
				);
				CreateBuffer::getInstance()->createConstBufferView
				(
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
			textureBuffer[i].Get(),
			hHandle
		);
		heapTags.push_back(HeapBufferTag::HEAP_TAG_TEXTURE_BUFFER);
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

void Model::createCommonBuffer()
{
	CreateBuffer::getInstance()->createConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(CommonConstData),
		commonBuffers.Get()
	);
}

void Model::mapCommonConstData(const CommonConstData& data)
{
	CommonConstData* common;
	void* pCommon = static_cast<void*>(common);
	commonBuffers->Map(0, nullptr, static_cast<void**>(&pCommon));
	*common = data;
	commonBuffers->Unmap(0, nullptr);
}

void Model::draw(const int modelNum)
{
	//現在セットされてるパイプラインと別物だったら入れ替え
	//今は、スプライトなどとコマンドリストを分けていないので、コメントアウト
	//if(currentSetPipeline != pipeline.Get())
	//	cmdLists[0]->SetPipelineState(pipeline.Get());
	cmdLists[0]->SetPipelineState(pipeline.Get());
	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	


	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(desHeap.Get());
	cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetGPUDescriptorHandleForHeapStart(),
		0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	int handleNum = 0;//どのくらいハンドルをずらすか決めるための番号


	//共通定数バッファ
	handleNum = static_cast<int>(textureBuffer.size());
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetGPUDescriptorHandleForHeapStart(),
		handleNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdLists[0]->SetGraphicsRootDescriptorTable(4, gpuDescHandle);

	//頂点バッファ分ループ
	auto vertexBufferNum = vertexBufferSet.size();
	for (int i = 0; i < vertexBufferNum; i++)
	{

		cmdLists[0]->IASetIndexBuffer(&indexBufferSet[i].indexBufferView);
		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSet[i].vertexBufferView);

		handleNum = 0;
		for (int j = 0; j < vertexBufferNum; j++)
		{
			//マテリアル紐づけ
			if (vertexBufferSet[i].materialName == materials[j].materialName) 
			{
				handleNum = j;
				break;
			}
		}


		//テクスチャバッファ
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(0, gpuDescHandle);


		//定数バッファセット
		int commonBufferNum = 1;
		handleNum = 0;
		handleNum += static_cast<int>(textureBuffer.size()) + commonBufferNum;//テクスチャと共通分ずらす
		handleNum += static_cast<int>(constBuffer[modelNum][i].size()) * i * modelNum;
		//handleNum += static_cast<int>(constBuffer[modelNum].size()) * modelNum;//オブジェクトの場所までずらす

		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(1, gpuDescHandle);


		//マテリアルバッファセット
		handleNum++;
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(3, gpuDescHandle);



		//ユーザー定数バッファセット

		//要素数を超えてアクセスしないようにするためのif
		if (heapTags.size() > handleNum + 2)
		{
			//ユーザー定数があったら、セットする
			if (heapTags[handleNum + 1] == HEAP_TAG_USER_CONST_BUFFER)
			{
				handleNum++;
				gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
				(
					desHeap->GetGPUDescriptorHandleForHeapStart(),
					handleNum,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);
				cmdLists[0]->SetGraphicsRootDescriptorTable(2, gpuDescHandle);
			}
		}

		//描画
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

	}


}

void Model::initialize()
{

#pragma region ディスクリプタレンジ_ルートパラメーター

	D3D12_DESCRIPTOR_RANGE modelRangeCSV{};
	modelRangeCSV.NumDescriptors = 1;
	modelRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	modelRangeCSV.BaseShaderRegister = 0;
	modelRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE userDescRangeCSV{};
	userDescRangeCSV.NumDescriptors = 1;
	userDescRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	userDescRangeCSV.BaseShaderRegister = 1;
	userDescRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE materialDescRangeCSV{};
	materialDescRangeCSV.NumDescriptors = 1;
	materialDescRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	materialDescRangeCSV.BaseShaderRegister = 2;
	materialDescRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE commonRangeCSV{};
	commonRangeCSV.NumDescriptors = 1;
	commonRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	commonRangeCSV.BaseShaderRegister = 3;
	commonRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.NumDescriptors = 1;
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV.BaseShaderRegister = 0;
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	D3D12_ROOT_PARAMETER rootparam[5]{};

	//テクスチャ
	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数
	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].DescriptorTable.pDescriptorRanges = &modelRangeCSV;
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//ユーザー定数
	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].DescriptorTable.pDescriptorRanges = &userDescRangeCSV;
	rootparam[2].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//マテリアル
	rootparam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[3].DescriptorTable.pDescriptorRanges = &materialDescRangeCSV;
	rootparam[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//共通
	rootparam[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[4].DescriptorTable.pDescriptorRanges = &commonRangeCSV;
	rootparam[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion

#pragma region ルートシグネチャ


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

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	PipelineState::setModelRootSignature(rootSignature.Get());
#pragma endregion

	cmdLists.resize(1);
}

void Model::setPipeline(PipelineState* pipelineState)
{
	if(pipelineState->getModelClassName() != typeid(*this).name())
	{
		OutputDebugString(L"パイプラインに設定されたモデルクラスと違います。\0");
		return;
	}
	pipeline = pipelineState->getPipelineState();
}
