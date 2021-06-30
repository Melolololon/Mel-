#include"ModelObject.h"
#include"CreateBuffer.h"

ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature>ModelObject::rootSignature;


ModelObject::ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;
	modelFileObjectNum = pModelData->GetModelFileObjectNumber();
	this->pModelData = pModelData;
}

void ModelObject::CreateConstBuffer(ConstBufferData* modelBufferData)
{
	if (modelBufferData)modelConstBufferType = modelBufferData->bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region リサイズ

	constBuffer.resize(modelFileObjectNum);
	materialConstBuffer.resize(modelFileObjectNum);

	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		modelConstBuffer.resize(1);
	}
	else if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		modelConstBuffer.resize(modelFileObjectNum);
	}


	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		userConstBuffer.resize(1);
	}
	else if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		userConstBuffer.resize(modelFileObjectNum);
	}

#pragma endregion


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize
		)
	{
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
	};


	//モデル
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelBufferData->bufferDataSize
		);
	}

	//ユーザー
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//モデル内のオブジェクト分ループ
	for (int j = 0; j < modelFileObjectNum; j++)
	{

		//メイン
		CreateBuffer
		(
			&constBuffer[j],
			sizeof(ModelConstBufferData)
		);

		//マテリアル
		CreateBuffer
		(
			&materialConstBuffer[j],
			sizeof(ModelConstBufferData)
		);

		//モデル
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[j],
				modelBufferData->bufferDataSize
			);
		}

		//ユーザー
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[j],
				userConstBufferData.bufferDataSize
			);
		}
	}


}

void ModelObject::DrawCommonProcessing(const std::string& rtName)
{
	if (!pModelData)return;
	MapConstData(RenderTarget::Get(rtName)->GetCamera());
	SetCmdList();
}

void ModelObject::MapConstData(const Camera* camera)
{
	ModelConstBufferData* constBufferData;

	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 基本的な情報のマップ

		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[i].addColor;
		constBufferData->subColor = modelConstDatas[i].subColor;
		constBufferData->mulColor = modelConstDatas[i].mulColor;
		constBufferData->ex = modelConstDatas[i].pushPolygonNum;


#pragma region 行列計算
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstDatas[i].scale.x,
			modelConstDatas[i].scale.y,
			modelConstDatas[i].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas[i].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas[i].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas[i].angle.y));

		DirectX::XMMATRIX cameraMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 cameraAngle = camera->GetCameraAngle().ToXMFLOAT3();
		cameraMat = DirectX::XMMatrixRotationZ(-cameraAngle.z);
		cameraMat = DirectX::XMMatrixRotationX(-cameraAngle.x);
		cameraMat = DirectX::XMMatrixRotationY(-cameraAngle.y);
		constBufferData->normalMat = matWorld * cameraMat;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[i].position.x,
			modelConstDatas[i].position.y,
			modelConstDatas[i].position.z
		);


		constBufferData->mat = matWorld * camera->GetViewAndProjectionMat();
		constBufferData->worldMat = matWorld;

#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);

#pragma endregion

#pragma region マテリアルのマップ
		MaterialConstData* materialConstData;

		materialConstBuffer[i]->Map(0, nullptr, (void**)&materialConstData);
		materialConstData->ambient = materials[i].ambient;
		materialConstData->diffuse = materials[i].diffuse;
		materialConstData->specular = materials[i].specular;
		materialConstData->alpha = materials[i].alpha;
		materialConstBuffer[i]->Unmap(0, nullptr);

#pragma endregion
	}
}

void ModelObject::SetCmdList()
{

	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D12DescriptorHeap* textureDescHeap = pModelData->GetTextureDesctiptorHeap();
	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(textureDescHeap);
	cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);


	//モデル特有バッファセット
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//ユーザーモデルバッファセット
	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());



	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//モデルのオブジェクトごとをセット
	//頂点バッファ分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		cmdLists[0]->SetPipelineState(pPipeline[i]->GetPipelineState().Get());


		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);

#pragma region テクスチャ

		////紐づけ
		//for (int j = 0; j < vertexBufferNum; j++)
		//{
		//	//マテリアル紐づけ
		//	//紐づけ失敗(マテリアル名未設定などで)だと、一番最初のテクスチャが選ばれる
		//	if (vertexBufferSet[i].materialName == materials[j].materialName)
		//	{
		//		textureBufferHandleNum = j;
		//		break;
		//	}
		//}
		//読み取ったモデルとモデルのテクスチャの順番同じだろうから紐づけいらない?

		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			textureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(4, gpuDescHandle);



		////2枚目(テスト用)
		//textureBufferHandleNum = 1;
		//gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		//(
		//	desHeap->GetGPUDescriptorHandleForHeapStart(),
		//	textureBufferHandleNum,
		//	device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		//);
		//cmdLists[0]->SetGraphicsRootDescriptorTable(5, gpuDescHandle);

#pragma endregion

#pragma region 定数



		//定数バッファセット
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		//マテリアルバッファセット
		cmdLists[0]->SetGraphicsRootConstantBufferView(MATERIAL_BUFFER_REGISTER, materialConstBuffer[i]->GetGPUVirtualAddress());


		//モデルバッファセット
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//ユーザー定数バッファセット
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//描画
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}

}

void ModelObject::Draw(const std::string& rtName)
{
	DrawCommonProcessing(rtName);
}

void ModelObject::SetPosition(const Vector3& position)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].position = position.ToXMFLOAT3();
	}
}

void ModelObject::SetScale(const Vector3& scale)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].scale = scale.ToXMFLOAT3();
	}
}

void ModelObject::SetAngle(const Vector3& angle)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].angle = angle.ToXMFLOAT3();
	}
}


bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region ディスクリプタレンジ_ルートパラメーター
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);

	CD3DX12_ROOT_PARAMETER rootparam[5] = {};

	//行列や色など
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	//ユーザー定数
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	//マテリアル
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//モデルごと
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャ
	rootparam[4].InitAsDescriptorTable(0, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

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

	PipelineState::SetModelRootSignature(rootSignature.Get());
}

void ModelObject::SetPipeline(PipelineState* pipelineState)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		pPipeline[i] = pipelineState;
	}
}
