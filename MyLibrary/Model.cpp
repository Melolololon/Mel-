#include "Model.h"
#include"CreateBuffer.h"
#include"PipelineState.h"

#include"RenderTarget.h"

ID3D12Device* Model::device;
std::vector<ID3D12GraphicsCommandList*>Model::cmdLists;
ComPtr<ID3D12RootSignature>Model::rootSignature;

DirectX::XMMATRIX Model::viewAndProjectionMat = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Model::cameraRotateMat = DirectX::XMMatrixIdentity();
std::vector<Model*> Model::pModels;
CommonConstData Model::commonConstData;
Model::Model()
{
	modelNum = 0;
	modelObjectNum = 0;
	textureBufferNum = 0;
	pModels.push_back(this);
}

Model::~Model()
{
	auto size = pModels.size();
	for(int i = 0;i < size;i++)
	{
		if(pModels[i] == this)
		{
			pModels.erase(pModels.begin() + i);
			break;
		}
	}
}

void Model::CreateModelVertexResources
(
	const size_t verticesSize,
	const std::vector<size_t>& verticesNum,
	const std::vector<std::vector<USHORT>>& indicesNum
)
{
#pragma region ���_�o�b�t�@

	//�I�u�W�F�N�g�����T�C�Y
	auto objectNum = verticesNum.size();
	vertexBufferSet.resize(objectNum);

	for (int i = 0; i < objectNum; i++)
	{
		CreateBuffer::GetInstance()->CreateVertexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			verticesSize,
			verticesNum[i],
			vertexBufferSet[i]
		);
	}
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@
	indexBufferSet.resize(objectNum);

	for (int i = 0; i < objectNum; i++)
	{
		CreateBuffer::GetInstance()->CreateIndexBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			indicesNum[i],
			indexBufferSet[i]
		);
	}
#pragma endregion

}

void Model::CreateModelHeapResourcesSetTexture
(
	const std::vector<Texture*>& pTextures,
	const int modelNum,
	const int modelFileObjectNum,
	BufferData* modelBufferData,
	BufferData* userBufferData
)
{

	BufferCreatePreparation
	(
		modelNum,
		modelFileObjectNum,
		pTextures.size(),
		modelBufferData,
		userBufferData
	);



#pragma region �f�B�X�N���v�^�q�[�v


	////�q�[�v�쐬
	//int constBufferNum = 3;
	//if (userDataSize != 0)constBufferNum++;
	//int commonBufferNum = 1;
	////�e�N�X�`���@+ ���ʃo�b�t�@ + (�萔�o�b�t�@ - ���ʃo�b�t�@) * ���f���� * ���f�����̃I�u�W�F�N�g��
	//int heapSize = 
	//	modelFileObjectNum
	//	+ commonBufferNum
	//	+ (constBufferNum - commonBufferNum)
	//	* modelNum
	//	* modelFileObjectNum;

	//heapSize = �e�N�X�`�� + ���� + 
	//				���f������ * ���f���� 
	//				���f���̃I�u�W�F�N�g���� * ���f���� * ���f���̃I�u�W�F�N�g��


	int heapSize =
		pTextures.size()
		+ commonConstBufferNum
		+ eachModelConstBufferNum * modelNum
		+ eachModelObjectConstBufferNum * modelNum * modelFileObjectNum;

	CreateDescriptorHeap(heapSize);

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@

	//�e�N�X�`���o�b�t�@�쐬
	CreateTextureBuffer
	(
		pTextures,
		0
	);

#pragma endregion

#pragma region �萔�o�b�t�@



	//�萔�o�b�t�@�쐬
	CreateConstBuffer
	(
		modelNum,
		modelFileObjectNum,
		pTextures.size(),
		modelBufferData,
		userBufferData
	);
#pragma endregion


	

}

void Model::CreateModelHeapResourcesSelectColor
(
	const Color& color,
	const int modelNum,
	const int modelFileObjectNum,
	BufferData* modelBufferData,
	BufferData* userBufferData
)
{
	BufferCreatePreparation
	(
		modelNum,
		1,
		1,
		modelBufferData,
		userBufferData
	);

#pragma region �f�B�X�N���v�^�q�[�v


	//�q�[�v�쐬
	int heapSize =
		1
		+ commonConstBufferNum
		+ eachModelConstBufferNum * modelNum
		+ eachModelObjectConstBufferNum * modelNum * modelFileObjectNum;

	CreateDescriptorHeap(heapSize);

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@

	//�e�N�X�`���o�b�t�@�쐬
	CreateOneColorTextureBuffer
	(
		color,
		0
	);

#pragma endregion

#pragma region �萔�o�b�t�@



	//�萔�o�b�t�@�쐬
	CreateConstBuffer
	(
		modelNum,
		modelFileObjectNum,
		modelFileObjectNum,
		modelBufferData,
		userBufferData
	);
#pragma endregion


}

void Model::BufferCreatePreparation
(
	const int modelNum,
	const int modelFileObjectNum,
	const size_t textureNum,
	BufferData* modelBufferData,
	BufferData* userBufferData
)
{
	this->modelNum = modelNum;
	this->modelObjectNum = modelFileObjectNum;

	pipeline.resize(modelNum);

	if (modelBufferData)
		modelConstBufferType = modelBufferData->bufferType;
	else
		modelConstBufferType = BufferData::BufferType::BUFFER_TYPE_NONE;

	if (userBufferData)
		userConstBufferType = userBufferData->bufferType;
	else
		userConstBufferType = BufferData::BufferType::BUFFER_TYPE_NONE;


	//�f�[�^���i�[���Ă���z���resize
	modelConstDatas.resize(modelNum);
	//���f�������[�v
	for (int i = 0; i < modelNum; i++) 
	{
		modelConstDatas[i].resize(modelFileObjectNum);
	}

	textureBuffer.resize(textureNum);

	//�o�b�t�@�����T�C�Y
	constBuffer.resize(modelNum);
	materialConstBuffer.resize(modelNum);

	for (int i = 0; i < modelNum; i++)
	{
		constBuffer[i].resize(modelFileObjectNum);
		materialConstBuffer[i].resize(modelFileObjectNum);
	}
	eachModelObjectConstBufferNum = 2;


	switch (modelConstBufferType)
	{

	case BufferData::BufferType::BUFFER_TYPE_COMMON:
		commonConstBufferNum++;
		modelConstBuffer.resize(1);
		break;

	case BufferData::BufferType::BUFFER_TYPE_EACH_MODEL:
		eachModelConstBufferNum++;
		modelConstBuffer.resize(modelNum);
		for (auto& buff : modelConstBuffer)
			buff.resize(1);
		break;

	case BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT:
		eachModelObjectConstBufferNum++;

		modelConstBuffer.resize(modelNum);
		for (auto& buff : modelConstBuffer)
			buff.resize(modelFileObjectNum);
		break;

	}

	switch (userConstBufferType)
	{

	case BufferData::BufferType::BUFFER_TYPE_COMMON:
		commonConstBufferNum++;
		userConstBuffer.resize(1);
		break;

	case BufferData::BufferType::BUFFER_TYPE_EACH_MODEL:
		eachModelConstBufferNum++;
		userConstBuffer.resize(modelNum);
		for (auto& buff : userConstBuffer)
			buff.resize(1);
		break;

	case BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT:
		eachModelObjectConstBufferNum++;

		userConstBuffer.resize(modelNum);
		for (auto& buff : userConstBuffer)
			buff.resize(modelFileObjectNum);
		break;

	}


}

void Model::CreateDescriptorHeap
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

void Model::CreateConstBuffer
(
	const int modelNum,
	const int modelFileObjectNum,
	const int heapTop, 
	BufferData * modelBufferData,
	BufferData* userBufferData 
)
{

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;
	int heapNum = heapTop;


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize,
		const HeapBufferTag tag
	) 
	{

		hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetCPUDescriptorHandleForHeapStart(),
			heapNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
		CreateBuffer::GetInstance()->CreateConstBufferView
		(
			hHandle,
			*pBuffer
		);

		heapTags.push_back(tag);
		heapNum++;

	};

	//���ʂ������炱���Ő���
	if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_COMMON) 
	{
		CreateBuffer
		(
			&modelConstBuffer[0][0],
			modelBufferData->bufferDataSize,
			HeapBufferTag::HEAP_TAG_MODEL_CONST_BUFFER
		);
	}

	//���ʂ������炱���Ő���
	if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_COMMON) 
	{
		CreateBuffer
		(
			&userConstBuffer[0][0],
			modelBufferData->bufferDataSize,
			HeapBufferTag::HEAP_TAG_USER_CONST_BUFFER
		);
	}
		


	//���f�������[�v
	for (int i = 0; i < modelNum; i++) 
	{
		//���f��
		if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL) 
		{
			CreateBuffer
			(
				&modelConstBuffer[i][0],
				modelBufferData->bufferDataSize,
				HeapBufferTag::HEAP_TAG_MODEL_CONST_BUFFER
			);
		}

		//���[�U�[
		if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL) 
		{
			CreateBuffer
			(
				&userConstBuffer[i][0],
				modelBufferData->bufferDataSize,
				HeapBufferTag::HEAP_TAG_USER_CONST_BUFFER
			);
		}

		//���f�����̃I�u�W�F�N�g�����[�v
		for (int j = 0; j < modelFileObjectNum; j++)
		{

			//���C��
			CreateBuffer
			(
				&constBuffer[i][j],
				sizeof(ModelConstBufferData),
				HeapBufferTag::HEAP_TAG_LIBRARY_CONST_BUFFER
			);

			//�}�e���A��
			CreateBuffer
			(
				&materialConstBuffer[i][j],
				sizeof(ModelConstBufferData),
				HeapBufferTag::HEAP_TAG_MATERIAL_CONST_BUFFER
			);

			//���f��
			if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT) 
			{
				CreateBuffer
				(
					&modelConstBuffer[i][j],
					modelBufferData->bufferDataSize,
					HeapBufferTag::HEAP_TAG_MODEL_CONST_BUFFER
				);
			}

			

			//���[�U�[
			if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT) 
			{
				CreateBuffer
				(
					&userConstBuffer[i][j],
					modelBufferData->bufferDataSize,
					HeapBufferTag::HEAP_TAG_USER_CONST_BUFFER
				);
			}
		}
	}

	
}

void Model::CreateTextureBuffer
(
	const std::vector<Texture*>& textures,
	const int heapTop
)
{

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;

	auto textureNum = textures.size();
	for (int i = 0; i < textureNum;i++)
	{
		hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetCPUDescriptorHandleForHeapStart(),
			heapTop + i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		CreateBuffer::GetInstance()->CreateTextureBuffer
		(
			textures[i]->GetMetadata(),
			textures[i]->GetImage(),
			hHandle,
			&textureBuffer[i]
		);

		heapTags.push_back(HeapBufferTag::HEAP_TAG_TEXTURE_BUFFER);
	}


	textureBufferNum = static_cast<int>(textureNum);
}

void Model::CreateOneColorTextureBuffer
(
	const Color& color,
	const int heapTop
)
{
	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;

	hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetCPUDescriptorHandleForHeapStart(),
		heapTop,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
	(
		color,
		hHandle,
		&textureBuffer[0]
	);

	heapTags.push_back(HeapBufferTag::HEAP_TAG_TEXTURE_BUFFER);
	textureBufferNum = 1;
}

#pragma region �}�b�v

void Model::MapVertexBuffer
(
	const int modelNum,
	void** vertexData
)
{
	vertexBufferSet[modelNum].vertexBuffer->Map(0, nullptr, vertexData);
}

void Model::UnmapVertexBuffer(const int& modelNum)
{
	vertexBufferSet[modelNum].vertexBuffer->Unmap(0, nullptr);
}

void Model::MapIndexBuffer
(
	const int modelNum,
	void** index
)
{
	indexBufferSet[modelNum].indexBuffer->Map(0, nullptr, index);
}

void Model::UnmapIndexBuffer(const int& modelNum)
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
//	//���ъm�F
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
//	//�Y�����v�Z
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

#pragma region �J���җp�֐�



void Model::SetCommonConstData(const CommonConstData& data)
{
	commonConstData = data;
	for (auto& m : pModels)
		m->MapCommonConstData();
}

void Model::MapCommonConstData()
{
	ModelConstBufferData* constBufferData;

	for (auto& buff : constBuffer)
	{
		for (auto& buff2 : buff) 
		{
			buff2->Map(0, nullptr, (void**)&constBufferData);

			constBufferData->light = commonConstData.light;
			constBufferData->lightColor = commonConstData.lightColor;
			constBufferData->lightMat = commonConstData.lightMat;
			constBufferData->cameraPos = commonConstData.cameraPos;

			buff2->Unmap(0, nullptr);
		}
	}

}

void Model::Initialize
(
	ID3D12Device* dev,
	std::vector<ID3D12GraphicsCommandList*> cmdList
)
{
	device = dev;
	cmdLists = cmdList;



#pragma region �f�B�X�N���v�^�����W_���[�g�p�����[�^�[

	D3D12_DESCRIPTOR_RANGE mainRangeCSV{};
	mainRangeCSV.NumDescriptors = 1;
	mainRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	mainRangeCSV.BaseShaderRegister = 0;
	mainRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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

	
	D3D12_DESCRIPTOR_RANGE modelRangeCSV{};
	modelRangeCSV.NumDescriptors = 1;
	modelRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	modelRangeCSV.BaseShaderRegister = 3;
	modelRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	D3D12_DESCRIPTOR_RANGE descRangeSRV0{};
	descRangeSRV0.NumDescriptors = 1;
	descRangeSRV0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV0.BaseShaderRegister = 0;
	descRangeSRV0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	D3D12_DESCRIPTOR_RANGE descRangeSRV1{};
	descRangeSRV1.NumDescriptors = 1;
	descRangeSRV1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV1.BaseShaderRegister = 1;
	descRangeSRV1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	


	D3D12_ROOT_PARAMETER rootparam[6]{};

	//�萔
	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].DescriptorTable.pDescriptorRanges = &mainRangeCSV;
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//���[�U�[�萔
	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].DescriptorTable.pDescriptorRanges = &userDescRangeCSV;
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�}�e���A��
	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].DescriptorTable.pDescriptorRanges = &materialDescRangeCSV;
	rootparam[2].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	//���f��
	rootparam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[3].DescriptorTable.pDescriptorRanges = &modelRangeCSV;
	rootparam[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	

	//�e�N�X�`��
	rootparam[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[4].DescriptorTable.pDescriptorRanges = &descRangeSRV0;
	rootparam[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootparam[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[5].DescriptorTable.pDescriptorRanges = &descRangeSRV1;
	rootparam[5].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	//����
	//rootparam[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootparam[4].DescriptorTable.pDescriptorRanges = &commonRangeCSV;
	//rootparam[4].DescriptorTable.NumDescriptorRanges = 1;
	//rootparam[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion

#pragma region ���[�g�V�O�l�`��


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

	//cmdLists.resize(1);
}


#pragma endregion


void Model::MapConstData(const int modelNum, const Camera* camera)
{
	ModelConstBufferData* constBufferData;
	for (int i = 0; i < modelObjectNum; i++) 
	{
		
#pragma region ��{�I�ȏ��̃}�b�v

		constBuffer[modelNum][i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[modelNum][i].addColor;
		constBufferData->subColor = modelConstDatas[modelNum][i].subColor;
		constBufferData->mulColor = modelConstDatas[modelNum][i].mulColor;
		constBufferData->ex = modelConstDatas[modelNum][i].pushPolygonNum;
		

#pragma region �s��v�Z
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstDatas[modelNum][i].scale.x,
			modelConstDatas[modelNum][i].scale.y,
			modelConstDatas[modelNum][i].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas[modelNum][i].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas[modelNum][i].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas[modelNum][i].angle.y));

		DirectX::XMMATRIX cameraMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 cameraAngle = camera->GetCameraAngle().ToXMFLOAT3();
		cameraMat = DirectX::XMMatrixRotationZ(-cameraAngle.z);
		cameraMat = DirectX::XMMatrixRotationX(-cameraAngle.x);
		cameraMat = DirectX::XMMatrixRotationY(-cameraAngle.y);
		constBufferData->normalMat = matWorld * cameraMat;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[modelNum][i].position.x,
			modelConstDatas[modelNum][i].position.y,
			modelConstDatas[modelNum][i].position.z
		);

	
		constBufferData->mat = matWorld * camera->GetViewAndProjectionMat();
		constBufferData->worldMat = matWorld;
	
#pragma endregion


		constBuffer[modelNum][i]->Unmap(0, nullptr);

#pragma endregion

#pragma region �}�e���A���̃}�b�v
		MaterialConstData* materialConstData;

		materialConstBuffer[modelNum][i]->Map(0, nullptr, (void**)&materialConstData);
		materialConstData->ambient = materials[i].ambient;
		materialConstData->diffuse = materials[i].diffuse;
		materialConstData->specular = materials[i].specular;
		materialConstData->alpha = materials[i].alpha;
		materialConstBuffer[modelNum][i]->Unmap(0, nullptr);

#pragma endregion

	}
}

void Model::DrawCommonProcessing(const int modelNum, const std::string& rtName)
{
	if (rtName != "")
	{
		
		MapConstData(modelNum, RenderTarget::Get(rtName)->GetCamera());
		SetCmdList(modelNum);
	}
	else
	{
		MapConstData(modelNum,RenderTarget::Get()->GetCamera());
		SetCmdList(modelNum);
	}
}

void Model::SetCmdList(const int modelNum)
{
	cmdLists[0]->SetPipelineState(pipeline[modelNum].Get());
	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(desHeap.Get());
	cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	//�ǂ̂��炢�n���h�������炷�����߂邽�߂̔ԍ�
	int constBufferHandleNum = 0;
	auto SetConstBufferDesTable = [&](const int rootParamIndex)
	{
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			constBufferHandleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(rootParamIndex, gpuDescHandle);
		constBufferHandleNum++;
	};


	//���ʂ��Z�b�g
	constBufferHandleNum = modelObjectNum;
	//���f�����L�o�b�t�@�Z�b�g
	if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_COMMON)
		SetConstBufferDesTable(3);

	//���[�U�[���f���o�b�t�@�Z�b�g
	if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_COMMON)
		SetConstBufferDesTable(1);


	//���f�����Ƃ��Z�b�g
	constBufferHandleNum =
		textureBufferNum
		+ commonConstBufferNum
		+ eachModelConstBufferNum * modelNum
		+ eachModelObjectConstBufferNum * modelObjectNum * modelNum;

	//���f�����L�o�b�t�@�Z�b�g
	if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		SetConstBufferDesTable(3);

	//���[�U�[���f���o�b�t�@�Z�b�g
	if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		SetConstBufferDesTable(1);

	
	//���f���̃I�u�W�F�N�g���Ƃ��Z�b�g
	//���_�o�b�t�@�����[�v
	auto vertexBufferNum = vertexBufferSet.size();
	for (int i = 0; i < vertexBufferNum; i++)
	{

		cmdLists[0]->IASetIndexBuffer(&indexBufferSet[i].indexBufferView);
		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSet[i].vertexBufferView);


#pragma region �e�N�X�`��


		int textureBufferHandleNum = 0;
		////�R�Â�
		//for (int j = 0; j < vertexBufferNum; j++)
		//{
		//	//�}�e���A���R�Â�
		//	//�R�Â����s(�}�e���A�������ݒ�Ȃǂ�)���ƁA��ԍŏ��̃e�N�X�`�����I�΂��
		//	if (vertexBufferSet[i].materialName == materials[j].materialName)
		//	{
		//		textureBufferHandleNum = j;
		//		break;
		//	}
		//}

		//�e�N�X�`���o�b�t�@
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			textureBufferHandleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(4, gpuDescHandle);



		////2����(�e�X�g�p)
		//textureBufferHandleNum = 1;
		//gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		//(
		//	desHeap->GetGPUDescriptorHandleForHeapStart(),
		//	textureBufferHandleNum,
		//	device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		//);
		//cmdLists[0]->SetGraphicsRootDescriptorTable(5, gpuDescHandle);

#pragma endregion

#pragma region �萔

		

		//�萔�o�b�t�@�Z�b�g
		SetConstBufferDesTable(0);

		//�}�e���A���o�b�t�@�Z�b�g
		SetConstBufferDesTable(2);

		//���f���o�b�t�@�Z�b�g
		if (modelConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			SetConstBufferDesTable(3);

		//���[�U�[�萔�o�b�t�@�Z�b�g
		if (userConstBufferType == BufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			SetConstBufferDesTable(1);
		
#pragma endregion


		//�`��
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(indices[i].size()), 1, 0, 0, 0);

	}

}

void Model::SetSubColor(const Color& color, const int modelNum)
{
	for (int i = 0; i < modelObjectNum; i++)
		modelConstDatas[modelNum][i].subColor =
	{
		(float)color.r / 255,
		(float)color.g / 255,
		(float)color.b / 255,
		(float)color.a / 255
	};
}

void Model::Draw(const int modelNum , const std::string& rtName)
{
	DrawCommonProcessing(modelNum, rtName);
}

void Model::AllDraw(){}

#pragma region ����
void Model::SetPosition(const Vector3& position, const int modelNum)
{
	for (int i = 0; i < modelObjectNum; i++)
		modelConstDatas[modelNum][i].position = position.ToXMFLOAT3();
}

void Model::SetAngle(const Vector3& angle, const int modelNum)
{
	for (int i = 0; i < modelObjectNum; i++)
		modelConstDatas[modelNum][i].angle = angle.ToXMFLOAT3();
}

void Model::SetScale(const Vector3& scale, const int modelNum)
{
	for (int i = 0; i < modelObjectNum; i++)
		modelConstDatas[modelNum][i].scale = scale.ToXMFLOAT3();
}

#pragma endregion


void Model::SetPipeline(PipelineState* pipelineState, const int modelNum)
{
	if(pipelineState->GetModelClassName() != typeid(*this).name())
	{
		OutputDebugString(L"�p�C�v���C���ɐݒ肳�ꂽ���f���N���X�ƈႢ�܂��B\0");
		return;
	}
	pipeline[modelNum] = pipelineState->GetPipelineState();
}

void Model::SetPipelineAllSet(PipelineState* pipelineState)
{
	if (pipelineState->GetModelClassName() != typeid(*this).name())
	{
		OutputDebugString(L"�p�C�v���C���ɐݒ肳�ꂽ���f���N���X�ƈႢ�܂��B\0");
		return;
	}

	for (auto& p : pipeline) 
	{
		p = pipelineState->GetPipelineState();
	}
}
