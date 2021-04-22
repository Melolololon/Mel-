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
		&loadFileObjectNum,
		&objBonePositions,
		&objBoneNums
	);
	auto vertexSize = vertices.size();
	for (int i = 0; i < vertexSize; i++)
		vertexBufferSet[i].materialName = materialName[i];


#pragma region テクスチャ反転


	for (auto& v:vertices)
	{
		for (auto& v2 : v)
			v2.uv.y = (v2.uv.y - 1) * -1;
	}
#pragma endregion

#pragma region 法線計算

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
	for (int i = 0; i < loadFileObjectNum; i++)
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
			(void**)&vertex
		);

		
		for (int j = 0; j < vertexSize; i++)
			vertex[j] = vertices[i][j];

		unmapVertexBuffer(i);


		createIndexBuffer(indices[i]);
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
	void** constData
)
{
#pragma region パスとファイル分離
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


	//テクスチャ名
	wchar_t texturePathW[20][256];

	//マテリアル読み込み&テクスチャ名取得
	int loadNum = 0;
	ModelLoader::getInstance()->loadObjMaterial
	(
		directoryPath,
		materialFileName,
		materials,
		&loadNum
	);

	D3D12_CPU_DESCRIPTOR_HANDLE hHandle = desHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < loadNum; i++)
	{
	
		MultiByteToWideChar
		(
			CP_ACP,
			0,
			materials[i].textureName.c_str(),
			-1,
			texturePathW[i],
			_countof(texturePathW[i])
		);

#pragma region テクスチャバッファ作成
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchimage{};

	const DirectX::Image* imgs;
	imgs = DirectXTexLoader::loadTexture
	(
		texturePathW[i],
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
#pragma endregion

#pragma region 定数バッファ作成
	

#pragma endregion


	}

}
