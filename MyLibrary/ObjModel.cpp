#include "ObjModel.h"
#include"ModelLoader.h"

PipelineState ObjModel::defaultObjPipeline;

ObjModel::ObjModel()
{
	loadFileObjectNum = 0;
	pipeline = defaultObjPipeline.getPipelineState();
}

ObjModel::~ObjModel(){}


void ObjModel::loadModelVertices
(
	const std::string& path,
	const bool loadUV,
	const bool loadNormal
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
	for (int i = 0; i < loadFileObjectNum; i++)
		vertexBufferSet[i].materialName = materialName[i];



	if(objBoneNums.size() == 0)
	{
		for (int i = 0; i < loadFileObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
			{
				vertices[i][j].boneNumber = 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < loadFileObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
			{
				vertices[i][j].boneNumber = objBoneNums[i][j];
			}
		}
	}

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

	createModelVertexResources
	(
		sizeof(ObjAnimationVertex),
		verticesNum,
		indices
	);

	//�}�b�v
	for (int i = 0; i < loadFileObjectNum; i++)
	{

		ObjAnimationVertex* vertex;
		mapVertexBuffer
		(
			i,
			(void**)&vertex
		);

		auto vertexNum = vertices[i].size();
		for (int j = 0; j < vertexNum; i++) 
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
	const int createNum,
	const size_t constDataSize 
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
	

	//�e�N�X�`���ǂݍ���
	textures.resize(loadObjectNum);
	std::vector<Texture*> pTexture(loadObjectNum);
	for(int i = 0; i < loadObjectNum;i++)
	{
		textures[i] = std::make_unique<Texture>();
		textures[i]->loadTexture(materials[i].textureName);
		pTexture[i] = textures[i].get();
	}
	
	createModelHeapResources
	(
		pTexture,
		createNum,
		static_cast<int>(materials.size()),
		constDataSize
	);
}

bool ObjModel::initialize() 
{
	PipelineData pipelineData;
	pipelineData.alphaWriteMode = ALPHA_WRITE_TRUE;
	pipelineData.blendMode = BLEND_ADD;
	pipelineData.cullMode = CULL_BACK;
	pipelineData.depthMode = DEPTH_TRUE;
	pipelineData.drawMode = DRAW_SOLID;

	std::vector<InputLayoutData> layoutData(4);
	layoutData[0].semantics = "POSITION";
	layoutData[0].formatType = FORMAT_TYPE_FLOAT;
	layoutData[0].number = 3;
	layoutData[1].semantics = "TEXCOORD";
	layoutData[1].formatType = FORMAT_TYPE_FLOAT;
	layoutData[1].number = 2;
	layoutData[2].semantics = "NORMAL";
	layoutData[2].formatType = FORMAT_TYPE_FLOAT;
	layoutData[2].number = 3;
	layoutData[3].semantics = "BONENUM";
	layoutData[3].formatType = FORMAT_TYPE_UNSIGNED_INT;
	layoutData[3].number = 1;

	auto result = defaultObjPipeline.createModelPipeline
	(
		pipelineData,
		{ L"../MyLibrary/ObjAnimationVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		&layoutData,
		typeid(ObjModel).name()
	);

	if(!result)
	{
		OutputDebugString(L"ObjModel�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł���\n");
		return false;
	}
	return true;
}
