#include "ObjModel.h"
#include"ModelLoader.h"

ObjModel::ObjModel()
{
	loadFileObjectNum = 0;
}

ObjModel::~ObjModel(){}


void ObjModel::loadModelVertices
(
	const std::string& path,
	const bool& loadUV,
	const bool& loadNormal
)
{
	

	//�}�e���A��
	std::vector<std::string>materialName;


	ModelLoader::getInstance()->loadOBJModel
	(
		path,
		loadUV,
		loadNormal,
		vertices,
		indices,
		materialFileName,
		materialName,
		smoothData,
		&loadFileObjectNum,
		&objBonePositions,
		&objBoneNums
	);
	auto vertexSize = vertices.size();
	for (int i = 0; i < vertexSize; i++)
		vertexBufferSet[i].materialName = materialName[i];


#pragma region �e�N�X�`�����]


	for (auto& v:vertices)
	{
		for (auto& v2 : v)
			v2.uv.y = (v2.uv.y - 1) * -1;
	}
#pragma endregion

#pragma region �@���v�Z

	auto vertSize = vertices.size();
	smoothNormal.resize(vertSize);
	for (int i = 0; i < vertSize; i++)
		smoothNormal[i].resize(vertices[i].size());

	for (int i = 0; i < vertSize; i++)
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

#pragma region �o�b�t�@
	std::vector<size_t>verticesNum(loadFileObjectNum);
	for (int i = 0; i < loadFileObjectNum; i++)
		verticesNum[i] = vertices[i].size();

	createVertexBuffer
	(
		sizeof(OBJAnimationVertex),
		verticesNum
	);

	createIndexBuffer(indices);

	for (int i = 0; i < loadFileObjectNum; i++)
	{

		OBJAnimationVertex* vertex;
		mapVertexBuffer
		(
			i,
			(void**)&vertex
		);
		
		for (int j = 0; j < vertexSize; i++)
			vertex[j] = vertices[i][j];

		unmapVertexBuffer(i);


		USHORT* index;
		mapIndexBuffer
		(
			i,
			(void**)&index
		);
		auto indexSize = indices.size();
		for (int j = 0; j < indexSize; i++)
			index[j] = indices[i][j];

		unmapIndexBuffer(i);
	}



#pragma endregion


}

void ObjModel::loadModelMaterial
(
	const int& createNum,
	void** constData,
	const size_t& constDataSize
)
{
#pragma region �p�X�ƃt�@�C������
	std::string directoryPath;
	std::string fileName;
	
	auto materialFileSize = materialFileName.size();
	for(int i = materialFileSize - 1;;i--)
	{
		if (materialFileName[i] == '/' ||
			materialFileName[i] == '\\')
		{
			std::copy
			(
				materialFileName.begin(),
				materialFileName.begin() + i,
				directoryPath.begin()
			);
			std::copy
			(
				materialFileName.begin() + i + 1, 
				materialFileName.end(), 
				fileName.begin()
			);
			break;
		}

	}


#pragma endregion



	//�}�e���A���ǂݍ���&�e�N�X�`�����擾
	int loadObjectNum = 0;
	ModelLoader::getInstance()->loadObjMaterial
	(
		directoryPath,
		materialFileName,
		materials,
		&loadObjectNum
	);
	//�e�N�X�`����
	std::vector<std::wstring>texturePathW(loadObjectNum);

	for (int i = 0; i < loadObjectNum; i++)
	{
		texturePathW[i].resize(materials[i].textureName.size());

		MultiByteToWideChar
		(
			CP_ACP,
			0,
			materials[i].textureName.c_str(),
			-1,
			texturePathW[i].data(),
			texturePathW[i].size()
		);
	}

	int heapHandleNum = 0;
	createTextureBuffer
	(
		texturePathW,
		heapHandleNum
	);

	std::vector<size_t>buffersSize;
	buffersSize =
	{
		sizeof(ModelConstData),
		sizeof(Material)
	};

	std::vector<HeapBufferTag>tags;
	tags =
	{
		HeapBufferTag::LIBRARY_CONST_BUFFER,
		HeapBufferTag::MATERIAL_CONST_BUFFER
	};

	if (constData)
	{
		buffersSize.push_back(constDataSize); 
		tags.push_back(HeapBufferTag::USER_CONST_BUFFER);
	}

	std::vector<size_t>bufferStructSize();

	heapHandleNum = texturePathW.size();
	createConstBuffer
	(
		createNum,
		loadFileObjectNum,
		heapHandleNum,
		buffersSize,
		tags
	);
}
