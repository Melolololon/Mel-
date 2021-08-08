#include "ModelData.h"

#include"CreateBuffer.h"
#include"ModelLoader.h"
#include"FbxLoader.h"


using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<ModelData>>ModelData::pModelDatas;
std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>ModelData::pPrimitiveModelDatas;


ID3D12Device* ModelData::device = nullptr;


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

	//オブジェクト分リサイズ
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
	textureBuffers.resize(textureNum);
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

void ModelData::CteateTextureBufferSetColor()
{
	//トイレから帰ってきたらやること
	//CteateTextureBuffer関数(一個上)と同じようにリサイズしたりする
	textureBuffers.resize(modelFileObjectNum);
	for (int i = 0; i < modelFileObjectNum; i++) 
	{
		CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
		(
			Color(255,255,255,255),
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureDescHeap->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			&textureBuffers[0]
		);
	}
}

void MelLib::ModelData::CreatePrimitiveModel()
{
	ModelData* pModelData = nullptr;

#pragma region BOARD
	pPrimitiveModelDatas.emplace(ShapeType3D::BOARD, std::make_unique<ModelData>());
	pModelData = pPrimitiveModelDatas[ShapeType3D::BOARD].get();

	std::vector<std::vector<FbxVertex>>& vertices = pModelData->vertices;
	vertices.resize(1);
	vertices[0].resize(4);

	vertices[0][0].pos = { -0.5f,-0.5f,-0.5f };
	vertices[0][0].uv = { 0.0f,1.0f };
	vertices[0][0].normal = {0.0f,0.0f,-1.0f };
	vertices[0][1].pos = { -0.5f,0.5f,-0.5f };
	vertices[0][1].uv = { 0.0f,0.0f };
	vertices[0][1].normal = { 0.0f,0.0f,-1.0f };
	vertices[0][2].pos = { 0.5f,-0.5f,-0.5f };
	vertices[0][2].uv = { 1.0f,1.0f };
	vertices[0][2].normal = { 0.0f,0.0f,-1.0f };
	vertices[0][3].pos = { 0.5f,0.5f,-0.5f };
	vertices[0][3].uv = { 1.0f,0.0f };
	vertices[0][3].normal = { 0.0f,0.0f,-1.0f };

	std::vector<std::vector<USHORT>>& indices = pModelData->indices;
	indices.resize(1);
	indices[0] =
	{
		0,1,2,2,1,3,//正面
	};

	pModelData->CreateModel();
#pragma endregion

#pragma region BOX

	pPrimitiveModelDatas.emplace(ShapeType3D::BOX, std::make_unique<ModelData>());

	pModelData = pPrimitiveModelDatas[ShapeType3D::BOX].get();

	std::vector<std::vector<FbxVertex>>& vertices = pModelData->vertices;
	vertices.resize(1);
	vertices[0].resize(24);
	
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	//正面
	vertices[0][0].pos = { -x,-y,-z };
	vertices[0][0].uv = { 0.0f,1.0f };
	vertices[0][1].pos = { -x,y,-z };
	vertices[0][1].uv = { 0.0f,0.0f };
	vertices[0][2].pos = { x,-y,-z };
	vertices[0][2].uv = { 1.0f,1.0f };
	vertices[0][3].pos = { x,y,-z };
	vertices[0][3].uv = { 1.0f,0.0f };


	//正面の上
	vertices[0][4].pos = { -x, y,-z };
	vertices[0][4].uv = { 0.0f,1.0f };
	vertices[0][5].pos = { -x, y,z };
	vertices[0][5].uv = { 0.0f,0.0f };
	vertices[0][6].pos = { x,y,-z };
	vertices[0][6].uv = { 1.0f,1.0f };
	vertices[0][7].pos = { x,y,z };
	vertices[0][7].uv = { 1.0f,0.0f };

	//正面の裏
	vertices[0][8].pos = { -x, y,z };
	vertices[0][8].uv = { 0.0f,1.0f };
	vertices[0][9].pos = { -x, -y,z };
	vertices[0][9].uv = { 0.0f,0.0f };
	vertices[0][10].pos = { x,y,z };
	vertices[0][10].uv = { 1.0f,1.0f };
	vertices[0][11].pos = { x,-y,z };
	vertices[0][11].uv = { 1.0f,0.0f };

	//正面の下
	vertices[0][12].pos = { -x, -y,z };
	vertices[0][12].uv = { 0.0f,1.0f };
	vertices[0][13].pos = { -x, -y,-z };
	vertices[0][13].uv = { 0.0f,0.0f };
	vertices[0][14].pos = { x,-y,z };
	vertices[0][14].uv = { 1.0f,1.0f };
	vertices[0][15].pos = { x,-y,-z };
	vertices[0][15].uv = { 1.0f,0.0f };

	//正面の右
	vertices[0][16].pos = { x,-y,-z };
	vertices[0][16].uv = { 0.0f,0.0f };
	vertices[0][17].pos = { x,y,-z };
	vertices[0][17].uv = { 1.0f,0.0f };
	vertices[0][18].pos = { x,-y,z };
	vertices[0][18].uv = { 0.0f,1.0f };
	vertices[0][19].pos = { x,y,z };
	vertices[0][19].uv = { 1.0f,1.0f };

	//正面の左
	vertices[0][20].pos = { -x,-y,z };
	vertices[0][20].uv = { 1.0f,1.0f };
	vertices[0][21].pos = { -x,y,z };
	vertices[0][21].uv = { 0.0f,1.0f };
	vertices[0][22].pos = { -x,-y,-z };
	vertices[0][22].uv = { 1.0f,0.0f };
	vertices[0][23].pos = { -x,y,-z };
	vertices[0][23].uv = { 0.0f,0.0f };


	//インデックスセット
	std::vector<std::vector<USHORT>>& indices = pModelData->indices;
	indices.resize(1);
	indices[0] = 
	{
		0,1,2,2,1,3,//正面
		4,5,6,6,5,7,//正面の上
		8,9,10,10,9,11,	//正面の裏
		12,13,14,14,13,15,//正面の下
		16,17,18,18,17,19,//正面の右
		20,21,22,22,21,23,//正面の左
	};


	//法線計算
	CalcPrimitiveModelNormal(vertices[0], indices[0]);

	pModelData->CreateModel();

#pragma endregion

}

void MelLib::ModelData::CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices, const std::vector<USHORT>& indices)
{
	for (int i = 0, size = indices.size() / 3; i < size; i++)
	{
		USHORT i0 = indices[i * 3 + 0];
		USHORT i1 = indices[i * 3 + 1];
		USHORT i2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[i0].pos;
		Vector3 p1 = vertices[i1].pos;;
		Vector3 p2 = vertices[i2].pos;;

		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;

		Vector3 normal = Vector3::Cross(v1, v2).Normalize();

		vertices[i0].normal = normal.ToXMFLOAT3();
		vertices[i1].normal = normal.ToXMFLOAT3();
		vertices[i2].normal = normal.ToXMFLOAT3();

	}

}

void MelLib::ModelData::CreateModel()
{
	//頂点、インデックス、テクスチャバッファ作成
	modelFileObjectNum = vertices.size();
	std::vector<size_t>verticesNum(modelFileObjectNum);
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		verticesNum[i] = vertices[i].size();
	}


	BufferPreparationSetColor
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

		for (int j = 0; j < verticesNum[i]; j++)
		{
			pFbxVertex[j] = vertices[i][j];
		}
		UnmapVertices(i);
	}

	materials.resize(modelFileObjectNum);
	pbrMaterials.resize(modelFileObjectNum);
	modelFormat = ModelFormat::MODEL_FORMAT_PRIMITIVE;
}

bool ModelData::Load(const std::string& path, const bool batchDeletionFlag, const std::string& name)
{
	pModelDatas.emplace(name, std::make_unique<ModelData>());
	bool result = pModelDatas[name]->LoadModel(path, name);

	if(!result)
	{
		pModelDatas.erase(name);
		return false;
	}
	
	pModelDatas[name]->batchDeletionFlag = batchDeletionFlag;
	

	return true;
}

void ModelData::Delete(const std::string& name)
{
	pModelDatas.erase(name);
}

void MelLib::ModelData::BatchDeletion()
{
	std::vector<std::string>deleteName;
	deleteName.reserve(pModelDatas.size());
	for(const auto& p : pModelDatas)
	{
		if(p.second->batchDeletionFlag)
		{
			deleteName.push_back(p.first);
		}
	}

	for(const auto& name : deleteName)
	{
		pModelDatas.erase(name);
	}
}

void ModelData::Initialize(ID3D12Device* pDevice)
{
	device = pDevice;
	CreatePrimitiveModel();
}



bool ModelData::LoadModel(const std::string& path, const std::string& name)
{
	bool result = false;
	
	//読み込み結果がfalseだったときの終了処理
	auto LoadFalseEndProcess = [&path]()
	{
		OutputDebugStringA(path.data());
		OutputDebugStringW(L"を読み込めませんでした。対応していないモデル形式、または、pathの入力ミスの可能性が考えられます。\n");

		return false;
	};

	if (path.find(".obj") != std::string::npos)
	{
		//オブジェクトのマテリアル名格納
		std::string materialFileName;
		std::vector<std::string>materialName;

		std::vector<Vector3> objBonePositions;
		std::vector<std::vector<int>> objBoneNums;
		
		//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
		std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;
		result = ModelLoader::GetInstance()->LoadObjModel
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

		if (!result)return LoadFalseEndProcess();


		if (boneNum == 0)
		{
			//ボーンがなかったら0にしとく
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

					//objでは0しか使わないので、0
					for (int k = 1; k < _countof(vertices[i][j].boneIndex); k++)
					{
						vertices[i][j].boneIndex[k] = 0;
						vertices[i][j].boneWeight[k] = 0;

					}
				}
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


#pragma region ディレクトリパスとモデル名取得
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

#pragma region マテリアル


		int materialNum = 0;
		result = ModelLoader::GetInstance()->LoadObjMaterial
		(
			directoryPath,
			materialFileName,
			materials,
			&materialNum
		);
		if (!result)LoadFalseEndProcess();
		pbrMaterials.resize(modelFileObjectNum);

		//テクスチャ読み込み
		pTextures.resize(materialNum);
		for (int i = 0; i < materialNum; i++)
		{
			pTextures[i] = std::make_unique<Texture>();
			result = pTextures[i]->LoadModelTexture(materials[i].textureName);
			if(!result)
			{
				OutputDebugStringA(path.data());
				OutputDebugStringW(L"のテクスチャの読み込みに失敗しました。\n");

				return false;
			}
		}

#pragma endregion

		modelName = path;

		modelFormat = ModelFormat::MODEL_FORMAT_OBJ;


	}
	else 
	if (path.find(".fbx") != std::string::npos)
	{	
		result = FbxLoader::GetInstance()->LoadFbxModel(path, this);
		if (!result)return LoadFalseEndProcess();


		boneNum = fbxData.bones.size();
		modelFormat = ModelFormat::MODEL_FORMAT_FBX;

#pragma region アニメーション関係準備
		if (fbxData.bones.size() != 0)
		{
			
			FbxAnimStack* animstack = fbxData.fbxScene->GetSrcObject<FbxAnimStack>(0);
			const char* animstackname = animstack->GetName();
			FbxTakeInfo* takeinfo = fbxData.fbxScene->GetTakeInfo(animstackname);

			fbxData.animationTimes.startTime = takeinfo->mLocalTimeSpan.GetStart();
			fbxData.animationTimes.endTime = takeinfo->mLocalTimeSpan.GetStop();

			//1秒60フレームで設定されてるアニメーションの場合、eFream60って設定する?
			fbxData.animationTimes.freamTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
		}


#pragma endregion

	}
	else
	{
		LoadFalseEndProcess();
	}

	//頂点、インデックス、テクスチャバッファ作成
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
		UnmapVertices(i);
	}


	return true;
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

	CreateDescriptorHeap(modelFileObjectNum);
	CteateTextureBufferSetColor();


}


std::vector<std::vector<Vector3>> MelLib::ModelData::GetVerticesPosition()
{
	int verticesSize = vertices.size();
	std::vector<std::vector<Vector3>>verticesPos(verticesSize);
	for (int i = 0; i < verticesSize; i++)
	{
		verticesPos[i].resize(vertices[i].size());


		for (int j = 0,size = verticesPos[i].size(); j < verticesSize; j++)
		{
			verticesPos[i][j] = vertices[i][j].pos;
		}
	}

	return verticesPos;
	
}

