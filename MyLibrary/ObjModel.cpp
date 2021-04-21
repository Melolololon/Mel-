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
	//objにあるモデル数
	int loadNum = 0;

	//マテリアル
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


#pragma region テクスチャ反転


	for (auto& v:vertices)
	{
		for (auto& v2 : v)
			v2.uv.y = (v2.uv.y - 1) * -1;
	}
#pragma endregion

#pragma region 法線計算

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

	//法線(平均求める用配列。ここに入れて、平均を求める)
	std::vector<DirectX::XMFLOAT3>sNor;

	//オブジェクト分ループ
	for (int i = 0; i < smoothData.size(); i++)
	{
		auto itr = smoothData[i].begin();
		std::vector<USHORT>ver;
		for (; itr != smoothData[i].end(); ++itr)
		{
			ver = itr->second;
			for (auto& v : ver)
			{
				//一気に24個入ってるし、clearしてないからおかしかった
				sNor.push_back(vertices[i][v].normal);
			}

			//法線平均化
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

#pragma region バッファ
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
	//テクスチャ名
	wchar_t texturePathW[20][256];

	//マテリアル読み込み&テクスチャ名取得
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
