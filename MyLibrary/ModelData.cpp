#include "ModelData.h"

#include"CreateBuffer.h"
#include"ModelLoader.h"
#include"FbxLoader.h"

std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;

void ModelData::CreateDescriptorHeap(const UINT textureNum)
{
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = textureNum;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureDescHeap));

} 


void ModelData::CreateVertexBufferSet
(
	const size_t vertexSize,
	const std::vector<size_t>& vertexNum
)
{

	//�I�u�W�F�N�g�����T�C�Y
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

void ModelData::MapVertices(void** data, const int bufferNum)
{
	vertexBufferSet[bufferNum].vertexBuffer->Map(0, nullptr, data);
}

void ModelData::UnmapVertices(const int bufferNum)
{
	vertexBufferSet[bufferNum].vertexBuffer->Unmap(0, nullptr);
}


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

void ModelData::CteateTextureBuffer()
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

void ModelData::CteateTextureBufferAndViewSetColor()
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

bool ModelData::Load(const std::string& path, const std::string& name)
{
	pModelDatas.emplace(name, std::make_unique<ModelData>());
	bool result = pModelDatas[name]->LoadModel(path, name);

	if(!result)
	{
		pModelDatas.erase(name);
		return false;
	}
	
	return true;
}

void ModelData::Delete(const std::string& name)
{
	pModelDatas.erase(name);
}

bool ModelData::LoadModel(const std::string& path, const std::string& name)
{
	

	if (path.find(".obj") != std::string::npos)
	{
		//�I�u�W�F�N�g�̃}�e���A�����i�[
		std::string materialFileName;
		std::vector<std::string>materialName;

		std::vector<Vector3> objBonePositions;
		std::vector<std::vector<int>> objBoneNums;
		
		//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
		std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;
		bool result = ModelLoader::GetInstance()->LoadObjModel
		(
			path,
			true,
			true,
			vertices,
			indices,
			materialFileName,
			materialName,
			smoothData,
			nullptr,
			&objBonePositions,
			&objBoneNums
		);
		boneNum = objBonePositions.size();
		modelFileObjectNum = vertices.size();

		if (!result)return false;


		if (boneNum == 0)
		{
			//�{�[�����Ȃ�������0�ɂ��Ƃ�
			for (int i = 0; i < modelFileObjectNum; i++)
			{
				auto vertexNum = vertices[i].size();
				for (int j = 0; j < vertexNum; j++) 
				{
					vertices[i][j].boneIndex[0] = 0;
					vertices[i][j].boneWeight[0] = 1;

				}
			}
		}
		else
		{
			for (int i = 0; i < modelFileObjectNum; i++)
			{
				auto vertexNum = vertices[i].size();

				for (int j = 0; j < vertexNum; j++)
				{
					vertices[i][j].boneIndex[0] = objBoneNums[i][j];
					vertices[i][j].boneWeight[0] = 1;

					//obj�ł�0�����g��Ȃ��̂ŁA0
					for (int k = 1; k < _countof(vertices[i][j].boneIndex); k++)
					{
						vertices[i][j].boneIndex[k] = 0;
						vertices[i][j].boneWeight[k] = 1;

					}
				}
			}
		}

		//�e�N�X�`�����]
		for (auto& v : vertices)
		{
			for (auto& v2 : v)
			{
				v2.uv.y = (v2.uv.y - 1) * -1;
			}
		}


#pragma region �@���v�Z

		auto vertNum = vertices.size();
		smoothNormal.resize(vertNum);
		for (int i = 0; i < vertNum; i++)
			smoothNormal[i].resize(vertices[i].size());

		for (int i = 0; i < vertNum; i++)
		{
			for (int j = 0; j < vertices[i].size(); j++)
			{
				smoothNormal[i][j] = vertices[i][j].normal;
			}
		}

		//�@��(���ϋ��߂�p�z��B�����ɓ���āA���ς����߂�)
		std::vector<DirectX::XMFLOAT3>sNor;

		//�I�u�W�F�N�g�����[�v
		for (int i = 0; i < smoothData.size(); i++)
		{
			auto itr = smoothData[i].begin();
			std::vector<USHORT>ver;
			for (; itr != smoothData[i].end(); ++itr)
			{
				ver = itr->second;
				for (auto& v : ver)
				{
					//��C��24�����Ă邵�Aclear���ĂȂ����炨����������
					sNor.push_back(vertices[i][v].normal);
				}

				//�@�����ω�
				DirectX::XMVECTOR aveNormal = { 0,0,0 };
				for (auto& n : sNor)
				{
					aveNormal.m128_f32[0] += n.x;
					aveNormal.m128_f32[1] += n.y;
					aveNormal.m128_f32[2] += n.z;
				}
				aveNormal.m128_f32[0] /= sNor.size();
				aveNormal.m128_f32[1] /= sNor.size();
				aveNormal.m128_f32[2] /= sNor.size();
				aveNormal = DirectX::XMVector3Normalize(aveNormal);
				for (auto& v : ver)
				{
					smoothNormal[i][v] = { aveNormal.m128_f32[0],aveNormal.m128_f32[1], aveNormal.m128_f32[2] };
				}
				sNor.clear();
			}
		}


#pragma endregion



#pragma region �f�B���N�g���p�X�ƃ��f�����擾
		std::string directoryPath;
		std::string fullPath = path;

		auto fullPathSize = fullPath.size();
		int loopCount = 0;
		for (int i = fullPathSize - 1;; i--)
		{
			if (fullPath[i] == '/' ||
				fullPath[i] == '\\')
			{
				directoryPath.resize(fullPathSize - loopCount);
				std::copy
				(
					fullPath.begin(),
					fullPath.begin() + i + 1,
					directoryPath.begin()
				);
				break;
			}
			else
			{
				modelName.push_back(fullPath[i]);
			}

			loopCount++;
		}

		std::reverse(modelName.begin(), modelName.end());
#pragma endregion

		int materialNum = 0;
		result = ModelLoader::GetInstance()->LoadObjMaterial
		(
			directoryPath,
			materialFileName,
			materials,
			&materialNum
		);

		//�e�N�X�`���ǂݍ���
		pTextures.resize(materialNum);
		for (int i = 0; i < materialNum; i++)
		{
			pTextures[i] = std::make_unique<Texture>();
			result = pTextures[i]->LoadModelTexture(materials[i].textureName);
			if(!result)
			{
				OutputDebugStringA(path.data());
				OutputDebugStringW(L"�̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n");

				return false;
			}
		}

		modelName = path;

		modelFormat = ModelFormat::MODEL_FORMAT_OBJ;

		return true;

	}
	else 
	if (path.find(".fbx") != std::string::npos)
	{	
		FbxLoader::GetInstance()->LoadFbxModel(path, this);

		//�ꎞ�I�ɏ����Ă�
		materials[0].ambient = { 0.1f,0.1f,0.1f };
		materials[0].diffuse = { 1.0f,1.0f,1.0f };

		boneNum = fbxData.bones.size();
		modelFormat = ModelFormat::MODEL_FORMAT_FBX;

		return true;
	}
	else
	{
		OutputDebugStringA(path.data());
		OutputDebugStringW(L"��ǂݍ��߂܂���ł����B�Ή����Ă��Ȃ����f���`���A�܂��́Apath�̓��̓~�X�̉\�����l�����܂��B\n");

		return false;
	}

	//���_�A�C���f�b�N�X�A�e�N�X�`���o�b�t�@�쐬
	modelFileObjectNum = vertices.size();
	std::vector<size_t>verticesNum(modelFileObjectNum);
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		verticesNum[i] = vertices[i].size();
	}


	BufferPreparationSetTexture
	(
		sizeof(FbxVertex),
		verticesNum,
		indices
	);

	//Map
	for (int i = 0; i < modelFileObjectNum; i++) 
	{
		FbxVertex* pFbxVertex = nullptr;
		MapVertices((void**)&pFbxVertex, i);

		for(int j = 0; j < verticesNum[i];j++)
		{
			pFbxVertex[j] = vertices[i][j];
		}
	}
}



void ModelData::BufferPreparationSetTexture
(
	const size_t vertexSize,
	const std::vector<size_t>& vertexNum,
	const std::vector<std::vector<USHORT>>& indices
)
{
	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum
	);


	CreateIndexBufferSet(indices);
	MapIndices(indices);

	CreateDescriptorHeap(pTextures.size());
	CteateTextureBuffer();


}


void ModelData::BufferPreparationSetColor
(
	const size_t vertexSize,
	const  std::vector<size_t>& vertexNum,
	const std::vector<std::vector<FbxVertex>>& vertices,
	const std::vector<std::vector<USHORT>>& indices
)
{

	CreateVertexBufferSet
	(
		vertexSize,
		vertexNum
	);

	CreateIndexBufferSet(indices);
	MapIndices(indices);

	CreateDescriptorHeap(1);
	CteateTextureBufferAndViewSetColor();


}