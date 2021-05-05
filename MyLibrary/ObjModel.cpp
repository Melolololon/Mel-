#include "ObjModel.h"
#include"ModelLoader.h"

PipelineState ObjModel::defaultPipeline;

ObjModel::ObjModel()
{
}

ObjModel::~ObjModel(){}


void ObjModel::LoadModelVertices
(
	const std::string& path,
	const bool loadUV,
	const bool loadNormal,
	const size_t vertexSize
)
{
	//オブジェクトのマテリアル名格納
	std::vector<std::string>materialName;

	ModelLoader::GetInstance()->LoadObjModel
	(
		path,
		loadUV,
		loadNormal,
		vertices,
		indices,
		materialFileName,
		materialName,
		smoothData,
		&modelObjectNum,
		&objBonePositions,
		&objBoneNums
	);
	boneNum = objBonePositions.size();

	if(boneNum == 0)
	{
		//ボーンがなかったら0にしとく
		for (int i = 0; i < modelObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
				vertices[i][j].boneNumber = 0;
		}
	}
	else
	{
		for (int i = 0; i < modelObjectNum; i++)
		{
			auto vertexNum = vertices[i].size();
			for (int j = 0; j < vertexNum; j++)
				vertices[i][j].boneNumber = objBoneNums[i][j];
		}
	}

#pragma region テクスチャ反転


	for (auto& v:vertices)
	{
		for (auto& v2 : v)
			v2.uv.y = (v2.uv.y - 1) * -1;
	}
#pragma endregion

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

#pragma region バッファ
	std::vector<size_t>verticesNum(modelObjectNum);
	for (int i = 0; i < modelObjectNum; i++)
		verticesNum[i] = vertices[i].size();

	size_t vertSize = 0;
	if (vertexSize == 0||
		typeid(this) == typeid(ObjModel))
		vertSize = sizeof(ObjAnimationVertex);
	else
		vertSize = vertexSize;

	CreateModelVertexResources
	(
		vertSize,
		verticesNum,
		indices
	);

	//マテリアル名割り当て
	for (int i = 0; i < modelObjectNum; i++)
		vertexBufferSet[i].materialName = materialName[i];


	//マップ
	for (int i = 0; i < modelObjectNum; i++)
	{

		ObjAnimationVertex* vertex;
		MapVertexBuffer
		(
			i,
			(void**)&vertex
		);

		auto vertexNum = vertices[i].size();
		for (int j = 0; j < vertexNum; j++) 
			vertex[j] = vertices[i][j];

		UnmapVertexBuffer(i);


		USHORT* index;
		MapIndexBuffer
		(
			i,
			(void**)&index
		);
		auto indexSize = indices.size();
		for (int j = 0; j < indexSize; j++)
			index[j] = indices[i][j];

		UnmapIndexBuffer(i);
	}



#pragma endregion


}

void ObjModel::LoadModelMaterial
(
	const std::string& directryPath,
	const int createNum,
	const size_t constDataSize 
)
{


	//マテリアル読み込み&テクスチャ名取得
	int loadObjectNum = 0;
	ModelLoader::GetInstance()->LoadObjMaterial
	(
		directryPath,
		materialFileName,
		materials,
		&loadObjectNum
	);
	
	//テクスチャ読み込み
	textures.resize(loadObjectNum);
	std::vector<Texture*> pTexture(loadObjectNum);
	for(int i = 0; i < loadObjectNum;i++)
	{
		textures[i] = std::make_unique<Texture>();
		textures[i]->LoadModelTexture(materials[i].textureName);
		pTexture[i] = textures[i].get();
	}
	
	CreateModelHeapResourcesSetTexture
	(
		pTexture,
		createNum,
		static_cast<int>(materials.size()),
		constDataSize
	);

	boneDatas.resize(createNum);
	for (auto& b : boneDatas)
		b.resize(boneNum);

	parentBoneDatas.resize(createNum);
	for (auto& b : parentBoneDatas)
		b.resize(boneNum);

	pipeline = defaultPipeline.GetPipelineState();

}

void ObjModel::LoadModel
(
	const std::string& path,
	const bool loadUV,
	const int modelNum,
	//const size_t vertexDataSize,
	const size_t constDataSize
)
{
	//LoadModelVertices(path, loadUV, true, vertexDataSize);
	LoadModelVertices(path, loadUV, true, sizeof(ObjAnimationVertex));


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


	LoadModelMaterial(directoryPath,modelNum, constDataSize);
}

bool ObjModel::Initialize() 
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

	auto result = defaultPipeline.CreatePipeline
	(
		pipelineData,
		{ L"../MyLibrary/ObjAnimationVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" }, 
		PipelineType::PIPELINE_TYPE_MODEL,
		&layoutData,
		typeid(ObjModel).name()
	);

	if(!result)
	{
		OutputDebugString(L"ObjModelの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void ObjModel::Draw(const int modelNum)
{
	MapConstData(modelNum);
	MapBoneMatrix(modelNum);
	SetCmdList(modelNum);
}

#pragma region 情報取得
std::vector<Vector3>ObjModel::GetBonePosition()
{
	std::vector<Vector3>v3BonePos(boneNum);
	for (int i = 0; i < boneNum; i++)
		v3BonePos[i] = objBonePositions[i];
	return v3BonePos;
}

#pragma endregion


void ObjModel::MapBoneMatrix(const int modelNum)
{
	if (boneNum == 0)return;

	ModelConstBufferData* modelConstData;

	//ボーンの行列を全オブジェクトに持たせるのはもったいないから、やっぱバッファ分けれるようにしたほうがいい
	//(ボーンの行列は、オブジェクトで共通だから)
	for (int i = 0; i < modelObjectNum; i++)
	{

#pragma region ボーンの値マップ
		constBuffer[modelNum][i][0]->Map(0, nullptr, (void**)&modelConstData);



		DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 boneScale;
		DirectX::XMFLOAT3 boneAngle;
		DirectX::XMFLOAT3 boneMoveVector;

		//親ボーンの行列乗算
		int parentBoneNum = 0;
		int bone = 0;

		DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
		DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
		DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
		DirectX::XMFLOAT3 bonePos = { 0,0,0 };


		for (UINT j = 0; j < boneNum; j++)
		{
			boneMat = DirectX::XMMatrixIdentity();

			//ボーンから頂点の距離分移動
			bonePos = objBonePositions[j];
			boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

			boneScale = boneDatas[modelNum][j].scale;
			boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

			boneAngle = boneDatas[modelNum][j].angle;
			boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
			boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
			boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

			boneMoveVector = boneDatas[modelNum][j].moveVector;

			//モデル自体のスケールの乗算により、ボーンの平行移動の値にスケールが乗算されるため、
			//割って増減を抑えている
			boneMat *= DirectX::XMMatrixTranslation
			(
				boneMoveVector.x / modelConstDatas[i][0].scale.x,
				boneMoveVector.y / modelConstDatas[i][0].scale.y,
				boneMoveVector.z / modelConstDatas[i][0].scale.z
			);

			//回転させたら戻す
			boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

			boneAngleImpact = parentBoneDatas[modelNum][j].angleImpact;
			boneScaleImpact = parentBoneDatas[modelNum][j].scaleImpact;
			boneMoveVectorImpact = parentBoneDatas[modelNum][j].moveVectorImpact;

			//親のボーン番号代入
			parentBoneNum = parentBoneDatas[modelNum][j].parentBoneNum;

			//これを使って影響度とか加算してく
			//子　→　親の順で入れる
			std::vector<int>parentNums;
			parentNums.reserve(50);

			//こちらは親　→　子
			std::vector<DirectX::XMMATRIX>parentMat;
			parentMat.reserve(50);


			//先にすべての親を取得&影響度取得
			while (1)
			{
				//-1だったら(親がセットされてなかったら)抜ける
				if (parentBoneNum == -1)break;
				parentNums.push_back(parentBoneNum);

				//親のボーン番号代入
				parentBoneNum = parentBoneDatas[modelNum][parentBoneNum].parentBoneNum;

			}
			if (parentNums.size() != 0)
			{


				DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

				//親数
				const int maxParentSize = static_cast<int>(parentNums.size());

				DirectX::XMFLOAT3 pAngle = { 0,0,0 };
				DirectX::XMFLOAT3 pScale = { 1,1,1 };
				DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
				DirectX::XMFLOAT3 pPos = { 0,0,0 };

				DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
				DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
				DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

				mulMat = DirectX::XMMatrixIdentity();

				//最後にある親のボーンを基準に回すので、入れる
				pPos.x = objBonePositions[parentNums[maxParentSize - 1]].x;
				pPos.y = objBonePositions[parentNums[maxParentSize - 1]].y;
				pPos.z = objBonePositions[parentNums[maxParentSize - 1]].z;

				int impactIndex = j;
				for (auto& pNum : parentNums)
				{
					pAngle.x += boneDatas[modelNum][pNum].angle.x;
					pAngle.y += boneDatas[modelNum][pNum].angle.y;
					pAngle.z += boneDatas[modelNum][pNum].angle.z;

					pScale.x *= boneDatas[modelNum][pNum].scale.x;
					pScale.y *= boneDatas[modelNum][pNum].scale.y;
					pScale.z *= boneDatas[modelNum][pNum].scale.z;

					pMoveVector.x += boneDatas[modelNum][pNum].moveVector.x;
					pMoveVector.y += boneDatas[modelNum][pNum].moveVector.y;
					pMoveVector.z += boneDatas[modelNum][pNum].moveVector.z;



					pAngleImpact.x *= parentBoneDatas[modelNum][impactIndex].angleImpact.x;
					pAngleImpact.y *= parentBoneDatas[modelNum][impactIndex].angleImpact.y;
					pAngleImpact.z *= parentBoneDatas[modelNum][impactIndex].angleImpact.z;
					pScaleImpact.x *= parentBoneDatas[modelNum][impactIndex].scaleImpact.x;
					pScaleImpact.y *= parentBoneDatas[modelNum][impactIndex].scaleImpact.y;
					pScaleImpact.z *= parentBoneDatas[modelNum][impactIndex].scaleImpact.z;
					pMoveVectorImpact.x *= parentBoneDatas[modelNum][impactIndex].moveVectorImpact.x;
					pMoveVectorImpact.y *= parentBoneDatas[modelNum][impactIndex].moveVectorImpact.y;
					pMoveVectorImpact.z *= parentBoneDatas[modelNum][impactIndex].moveVectorImpact.z;

					impactIndex = pNum;
				}

				pAngle.x *= pAngleImpact.x;
				pAngle.y *= pAngleImpact.y;
				pAngle.z *= pAngleImpact.z;

				pScale.x *= pScaleImpact.x;
				pScale.y *= pScaleImpact.y;
				pScale.z *= pScaleImpact.z;

				pMoveVector.x *= pMoveVectorImpact.x;
				pMoveVector.y *= pMoveVectorImpact.y;
				pMoveVector.z *= pMoveVectorImpact.z;


				//ボーンから頂点の距離分移動
				mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

				mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

				mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
				mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
				mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));


				//親はまとめて掛けているので、モデルと自分の拡縮だけ掛ければいい
				//modelConstDatas[i][0].scale に boneScale掛ける必要あるか要確認
				mulMat *= DirectX::XMMatrixTranslation
				(
					pMoveVector.x / (modelConstDatas[i][0].scale.x * boneScale.x),
					pMoveVector.y / (modelConstDatas[i][0].scale.y * boneScale.y),
					pMoveVector.z / (modelConstDatas[i][0].scale.z * boneScale.z)
				);

				//回転させたら戻す
				mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


				boneMat *= mulMat;
			}
			//配列の0は設定しないようにする(0はボーン未割当ての頂点の行列なので、いじらないようにする)
			modelConstData->boneMat[j + 1] = boneMat;

		}


		
		constBuffer[modelNum][i][0]->Unmap(0, nullptr);
#pragma endregion
	}

}
