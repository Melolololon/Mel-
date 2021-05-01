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
	const size_t verticesSize,
	const std::vector<size_t>& verticesNum
)
{
	//�I�u�W�F�N�g�����T�C�Y
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
	
}

void Model::createIndexBuffer(const  std::vector<std::vector<USHORT>>& indicesNum)
{
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
	const std::vector<size_t>& constDataSize,
	const std::vector<HeapBufferTag>& tags
)
{
	auto oneModelBufferNum = constDataSize.size();
	//�����������T�C�Y
	constBuffer.resize(modelNum);
	//���f���t�@�C���ɂ���I�u�W�F�N�g���������T�C�Y
	for (auto c : constBuffer) 
	{
		c.resize(modelFileObjectNum);

		//�o�b�t�@���������T�C�Y
		for (auto& c2 : c)
			c2.resize(oneModelBufferNum);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle;
	int heapNum = heapTop;

	//��������CreateBuffer�֐����Ńq�[�v�̃n���h���擾����悤�ɂ���?
#pragma region �R�����o�b�t�@�쐬

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
	heapTags.push_back(COMMON_CONST_BUFFER);
	heapNum++;
#pragma endregion


	//���f�������[�v
	for (int i = 0; i < modelNum; i++) 
	{
		//���f�����̃I�u�W�F�N�g�����[�v
		for (int j = 0; j < modelFileObjectNum; j++)
		{
			//1�̃��f�����̃I�u�W�F�N�g�̃o�b�t�@�����[�v
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
		heapTags.push_back(HeapBufferTag::TEXTURE_BUFFER);
	}
}

#pragma region �}�b�v

void Model::mapVertexBuffer
(
	const int modelNum,
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
#pragma endregion
