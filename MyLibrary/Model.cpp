#include "Model.h"
#include"CreateBuffer.h"
#include"PipelineState.h"

ID3D12Device* Model::device;
std::vector<ID3D12GraphicsCommandList*>Model::cmdLists;
ComPtr<ID3D12Resource>Model::commonBuffers;
ComPtr<ID3D12RootSignature>Model::rootSignature;

DirectX::XMMATRIX Model::viewAndProjectionMat = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Model::cameraRotateMat = DirectX::XMMatrixIdentity();

Model::Model()
{
	modelNum = 0;
	modelObjectNum = 0;
	textureBufferNum = 0;
}

Model::~Model()
{
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

	modelClassName = typeid(*this).name();
}

void Model::CreateModelHeapResourcesSetTexture
(
	const std::vector<Texture*>& pTextures,
	const int modelNum,
	const int modelFileObjectNum,
	const size_t userDataSize
)
{
	ResizeConstData
	(
		modelNum,
		1
	);

#pragma region �f�B�X�N���v�^�q�[�v


	//�q�[�v�쐬
	int constBufferNum = 3;
	if (userDataSize != 0)constBufferNum++;
	int commonBufferNum = 1;
	//�e�N�X�`���@+ ���ʃo�b�t�@ + (�萔�o�b�t�@ - ���ʃo�b�t�@) * ���f���� * ���f�����̃I�u�W�F�N�g��
	int heapSize = 
		modelFileObjectNum
		+ commonBufferNum
		+ (constBufferNum - commonBufferNum)
		* modelNum
		* modelFileObjectNum;

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
		modelFileObjectNum,
		userDataSize
	);
#pragma endregion


	this->modelNum = modelNum;
	this->modelObjectNum = modelFileObjectNum;
}

void Model::CreateModelHeapResourcesSelectColor
(
	const Color& color,
	const int modelNum,
	const int modelFileObjectNum,
	const size_t userDataSize
)
{
	ResizeConstData
	(
		modelNum,
		1
	);
#pragma region �f�B�X�N���v�^�q�[�v


	//�q�[�v�쐬
	int constBufferNum = 3;
	if (userDataSize != 0)constBufferNum++;
	int commonBufferNum = 1;
	//�e�N�X�`���@+ ���ʃo�b�t�@ + (�萔�o�b�t�@ - ���ʃo�b�t�@) * ���f���� * ���f�����̃I�u�W�F�N�g��
	int heapSize =
		modelFileObjectNum
		+ commonBufferNum
		+ (constBufferNum - commonBufferNum)
		* modelNum
		* modelFileObjectNum;

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
		userDataSize
	);
#pragma endregion


	this->modelNum = modelNum;
	this->modelObjectNum = modelFileObjectNum;
}

void Model::ResizeConstData
(
	const int modelNum,
	const int modelFileObjectNum
)
{
	modelConstDatas.resize(modelNum);
	//���f�������[�v
	for (int i = 0; i < modelNum; i++)
		modelConstDatas[i].resize(modelFileObjectNum);

	textureBuffer.resize(modelFileObjectNum);
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
	const size_t userDataSize
)
{
	int constBufferNum;
	int commonBufferNum = 1;

	if (userDataSize == 0)
		constBufferNum = 3;
	else
		constBufferNum = 4;

	//�����������T�C�Y
	constBuffer.resize(modelNum);
	//���f���t�@�C���ɂ���I�u�W�F�N�g���������T�C�Y
	for (auto& c : constBuffer) 
	{
		c.resize(modelFileObjectNum);

		//�o�b�t�@���������T�C�Y
		for (auto& c2 : c)
			c2.resize(constBufferNum - commonBufferNum);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;
	int heapNum = heapTop;

#pragma region �R�����o�b�t�@�r���[�쐬

	hHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetCPUDescriptorHandleForHeapStart(),
		heapNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);

	CreateBuffer::GetInstance()->CreateConstBufferView
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
		sizeof(ModelConstBufferData),
		sizeof(Material),
		userDataSize
	};

	//���f�������[�v
	for (int i = 0; i < modelNum; i++) 
	{
		//���f�����̃I�u�W�F�N�g�����[�v
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			//���f�����̃I�u�W�F�N�g�̃o�b�t�@�����[�v
			for (int k = 0; k < constBufferNum - commonBufferNum; k++)
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
					constDataSize[k],
					&constBuffer[i][j][k]
				);
				CreateBuffer::GetInstance()->CreateConstBufferView
				(
					hHandle,
					constBuffer[i][j][k].Get()
				);

				heapTags.push_back(tags[k]);
				heapNum++;
			}
		}
	}


	//�{�[���s��̏�����(���������ĂȂ��z������ƁAGPU�͂���ȍ~���Ă���Ȃ�����)
	ModelConstBufferData* constBufferData;
	for (int i = 0; i < modelNum; i++)
	{
		//���f�����̃I�u�W�F�N�g�����[�v
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			constBuffer[i][j][0]->Map(0, nullptr, (void**)&constBufferData);


			for (int k = 0; k < _countof(constBufferData->boneMat); k++)
				constBufferData->boneMat[k] = DirectX::XMMatrixIdentity();

			constBuffer[i][j][0]->Unmap(0, nullptr);
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


void Model::CreateCommonBuffer()
{
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(CommonConstData),
		&commonBuffers
	);
}

void Model::MapCommonConstData(const CommonConstData& data)
{
	CommonConstData* common;
	commonBuffers->Map(0, nullptr, (void**)&common);
	common->cameraPos = data.cameraPos;
	common->light = data.light;
	common->lightColor = data.lightColor;
	common->lightMat = data.lightMat;
	commonBuffers->Unmap(0, nullptr);
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

	//�e�N�X�`��
	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�萔
	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].DescriptorTable.pDescriptorRanges = &modelRangeCSV;
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//���[�U�[�萔
	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].DescriptorTable.pDescriptorRanges = &userDescRangeCSV;
	rootparam[2].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�}�e���A��
	rootparam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[3].DescriptorTable.pDescriptorRanges = &materialDescRangeCSV;
	rootparam[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//����
	rootparam[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[4].DescriptorTable.pDescriptorRanges = &commonRangeCSV;
	rootparam[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
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

void Model::MapConstData(const int modelNum)
{
	ModelConstBufferData* constBufferData;

	for (int i = 0; i < modelObjectNum; i++) 
	{
		
#pragma region ��{�I�ȏ��̃}�b�v

		constBuffer[modelNum][i][0]->Map(0, nullptr, (void**)&constBufferData);
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

		constBufferData->normalMat = matWorld * cameraRotateMat;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[modelNum][i].position.x,
			modelConstDatas[modelNum][i].position.y,
			modelConstDatas[modelNum][i].position.z
		);

		constBufferData->mat = matWorld * viewAndProjectionMat;
		constBufferData->worldMat = matWorld;
	
#pragma endregion


		constBuffer[modelNum][i][0]->Unmap(0, nullptr);

#pragma endregion

#pragma region �}�e���A���̃}�b�v
		MaterialConstData* materialConstData;

		constBuffer[modelNum][i][1]->Map(0, nullptr, (void**)&materialConstData);
		materialConstData->ambient = materials[i].ambient;
		materialConstData->diffuse = materials[i].diffuse;
		materialConstData->specular = materials[i].specular;
		materialConstData->alpha = materials[i].alpha;
		constBuffer[modelNum][i][1]->Unmap(0, nullptr);

#pragma endregion

	}
}

void Model::SetCmdList(const int modelNum)
{
	cmdLists[0]->SetPipelineState(pipeline.Get());
	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(desHeap.Get());
	cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	int handleNum = 0;//�ǂ̂��炢�n���h�������炷�����߂邽�߂̔ԍ�

	//���ʒ萔�o�b�t�@
	handleNum = textureBufferNum;
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		desHeap->GetGPUDescriptorHandleForHeapStart(),
		handleNum,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdLists[0]->SetGraphicsRootDescriptorTable(4, gpuDescHandle);

	//���_�o�b�t�@�����[�v
	auto vertexBufferNum = vertexBufferSet.size();
	for (int i = 0; i < vertexBufferNum; i++)
	{

		cmdLists[0]->IASetIndexBuffer(&indexBufferSet[i].indexBufferView);
		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSet[i].vertexBufferView);

		handleNum = 0;
		for (int j = 0; j < vertexBufferNum; j++)
		{
			//�}�e���A���R�Â�
			//�R�Â����s(�}�e���A�������ݒ�Ȃǂ�)���ƁA��ԍŏ��̃e�N�X�`�����I�΂��
			if (vertexBufferSet[i].materialName == materials[j].materialName)
			{
				handleNum = j;
				break;
			}
		}


		//�e�N�X�`���o�b�t�@
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(0, gpuDescHandle);


		//�萔�o�b�t�@�Z�b�g
		int commonBufferNum = 1;
		handleNum = 0;
		handleNum += textureBufferNum + commonBufferNum;//�e�N�X�`���Ƌ��ʕ����炷
		int cBuffSize = static_cast<int>(constBuffer[modelNum][i].size());

		handleNum += i * cBuffSize;
		handleNum += modelNum * (cBuffSize * vertexBufferNum);


		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(1, gpuDescHandle);


		//�}�e���A���o�b�t�@�Z�b�g
		handleNum++;
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			desHeap->GetGPUDescriptorHandleForHeapStart(),
			handleNum,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(3, gpuDescHandle);



		//���[�U�[�萔�o�b�t�@�Z�b�g

		//�v�f���𒴂��ăA�N�Z�X���Ȃ��悤�ɂ��邽�߂�if
		if (heapTags.size() > handleNum + 2)
		{
			//���[�U�[�萔����������A�Z�b�g����
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

		//�`��
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(indices[i].size()), 1, 0, 0, 0);

	}

}

void Model::Draw(const int modelNum)
{
	MapConstData(modelNum);
	SetCmdList(modelNum);

}

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


void Model::SetPipeline(PipelineState* pipelineState)
{
	if(pipelineState->GetModelClassName() != typeid(*this).name())
	{
		OutputDebugString(L"�p�C�v���C���ɐݒ肳�ꂽ���f���N���X�ƈႢ�܂��B\0");
		return;
	}
	pipeline = pipelineState->GetPipelineState();
}
