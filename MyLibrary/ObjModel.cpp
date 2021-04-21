#include "ObjModel.h"
#include"ModelLoader.h"

ObjModel::ObjModel(){}

ObjModel::~ObjModel(){}


void ObjModel::loadModelVertices
(
	const std::string& path,
	const bool& loadUV,
	const bool& loadNormal
)
{
	//obj�ɂ��郂�f����
	int loadNum = 0;

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
		&loadNum,
		&objBonePositions,
		&objBoneNums
	);
	auto size = vertices.size();
	for (int i = 0; i < size; i++)
		vertexBufferSet[i].materialName = materialName[i];


#pragma region �e�N�X�`�����]


	for (auto& v:vertices)
	{
		for (auto& v2 : v)
			v2.uv.y = (v2.uv.y - 1) * -1;
	}
#pragma endregion

#pragma region �@���v�Z

	auto verticesSize = vertices.size();
	smoothNormal.resize(verticesSize);
	for (int i = 0; i < verticesSize; i++)
		smoothNormal[i].resize(vertices[i].size());

	for (int i = 0; i < verticesSize; i++)
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
	for (int i = 0; i < loadNum; i++)
	{
		createVertexBuffer
		(
			sizeof(OBJAnimationVertex),
			vertices[i].size()

		);

		OBJAnimationVertex* vertex;
		mapVertexBuffer
		(
			i,
			(void**)vertex
		);

		
		for (int j = 0; j < size; i++)
			vertex[j] = vertices[i][j];

		unmapVertexBuffer(i);

		createIndexBuffer(indices[i]);

	}



#pragma endregion


}

void loadModelMaterial
(
	const int& createNum
)
{
	//�e�N�X�`����
	wchar_t texturePathW[20][256];

	//�}�e���A���ǂݍ���&�e�N�X�`�����擾
	int loadNum = 0;
	ModelLoader::getInstance()->loadObjMaterial
	(
		materialDirectoryPath,
		materialFileName,
		materials[key],
		&loadNum
	);

	for (int i = 0; i < loadNum; i++)
	{
		//mbsrtowcs_s(&size, texturePathW, 256, &texturePath, 256, nullptr);
		MultiByteToWideChar
		(
			CP_ACP,
			0,
			materials[key][i].textureName.c_str(),
			-1,
			texturePathW[i],
			_countof(texturePathW[i])
		);
	}
}
