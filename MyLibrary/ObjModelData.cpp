#include "ObjModelData.h"
#include"ModelLoader.h"

bool ObjModelData::Load(const std::string& path)
{

	//�I�u�W�F�N�g�̃}�e���A�����i�[
	std::string materialFileName;
	std::vector<std::string>materialName;
	

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

	if (!result)return false;
	

	int modelFileObjectNum = vertices.size();
	int boneNum = objBonePositions.size();

	if (boneNum == 0)
	{
		//�{�[�����Ȃ�������0�ɂ��Ƃ�
		for (int i = 0; i < modelFileObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
				vertices[i][j].boneNumber = 0;
		}
	}
	else
	{
		for (int i = 0; i < modelFileObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
				vertices[i][j].boneNumber = objBoneNums[i][j];
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



#pragma region �f�B���N�g���p�X�擾
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

		loopCount++;
	}

#pragma endregion

	int materialNum = 0;
	std::vector<Material>materials;
	result = ModelLoader::GetInstance()->LoadObjMaterial
	(
		directoryPath,
		materialFileName,
		materials,
		&materialNum
	);
	
	//�e�N�X�`���ǂݍ���
	pTextures.resize(materialNum);
	for(int i = 0; i < materialNum;i++)
	{
		pTextures[i] = std::make_unique<Texture>();
		pTextures[i]->LoadModelTexture(materials[i].textureName);
	}


	std::vector<size_t>verticesNum(modelFileObjectNum);
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		verticesNum[i] = vertices[i].size();
	}

	BufferPreparationSetTexture
	(
		sizeof(ObjAnimationVertex),
		verticesNum,
		vertices,
		indices
	);

	return true;
}
