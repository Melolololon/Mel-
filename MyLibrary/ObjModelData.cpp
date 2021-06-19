#include "ObjModelData.h"
#include"ModelLoader.h"

bool ObjModelData::Load(const std::string& path)
{

	//オブジェクトのマテリアル名格納
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
		//ボーンがなかったら0にしとく
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

	//テクスチャ反転
	for (auto& v : vertices)
	{
		for (auto& v2 : v) 
		{
			v2.uv.y = (v2.uv.y - 1) * -1;
		}
	}


#pragma region 法線計算

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



#pragma region ディレクトリパス取得
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
	
	//テクスチャ読み込み
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
