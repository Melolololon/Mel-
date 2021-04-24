#include "FbxLoader.h"

#include<cassert>




FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
}

FbxLoader* FbxLoader::getInstance()
{
	static FbxLoader inst;
	return &inst;
}

void FbxLoader::initialize(ID3D12Device* device)
{
	assert(!fbxManager);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");
	
}

void FbxLoader::end()
{
	//Importerを開放してからManagerを開放する
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

void FbxLoader::parseNodeRecursive
(
	FbxModel* model,
	FbxNode* fbxNode, 
	Node* parentNode
)
{
	using namespace DirectX;

	model->nodes.emplace_back();
	Node& node = model->nodes.back();

	std::string name = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.rotation =
	{
		(float)rotation[0],
		(float)rotation[1],
		(float)rotation[2],
		0.0f
	};

	node.scaling =
	{
		(float)scaling[0],
		(float)scaling[1],
		(float)scaling[2],
		0.0f
	};

	node.translation =
	{
		(float)translation[0],
		(float)translation[1],
		(float)translation[2],
		0.0f
	};

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixScalingFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;
	if(parentNode)
	{
		//親ノード代入
		node.parentNode = parentNode;

		//親の行列を乗算
		node.globalTransform = parentNode->globalTransform;
	}

	//FbxNodeAttribute ノードの追加情報
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	if(fbxNodeAttribute)
	{
		if(fbxNodeAttribute->GetAttributeType() == 
			FbxNodeAttribute::eMesh)
		{
			model->meshNode = &node;
			parseMesh(model, fbxNode);
		}
	}


	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		parseNodeRecursive(model, fbxNode->GetChild(i), &node);
}


void FbxLoader::loadFbxModel(const std::string& modelPath)
{
	if (!fbxImporter->Initialize
	(
		modelPath.c_str(), 
		-1,
		fbxManager->GetIOSettings()
	))
		assert(0);

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);

	FbxModel* model = new FbxModel();
	model->modelName = "";
	
	//ノード数取得
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()で配列を参照してるので、メモリの確保しなおさないようにするためにreserveしてる
	model->nodes.reserve(nodeCount);

	parseNodeRecursive(model, fbxScene->GetRootNode());

	fbxScene->Destroy();
}



void FbxLoader::parseMesh(FbxModel* model, FbxNode* node)
{
	FbxMesh* fbxMesh = node->GetMesh();

	parseMeshVertices(model, fbxMesh);
}

void FbxLoader::parseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
	//ControlPointとは、頂点データのこと(このライブラリでいうVertex構造体)

	//頂点数取得
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = model->vertices;
	vertices.resize(vertexNum);

	//座標取得
	FbxVector4* pCount = fbxMesh->GetControlPoints();

	//コピー
	for(int i = 0; i < vertexNum;i++)
	{
		vertices[i].pos.x = (float)pCount[i][0];
		vertices[i].pos.y = (float)pCount[i][1];
		vertices[i].pos.z = (float)pCount[i][2];
	}
}

void FbxLoader::parseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->getIndices();

	assert(indices.size() == 0);

	//面数
	const int polygonCount = fbxMesh->GetPolygonCount();
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV名のリスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//面ごとの情報読み込み
	for(int i = 0; i < polygonCount;i++)
	{
		//面のインデックス数
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for(int j = 0; j < polygonSize;j++)
		{
			//verticesの添え字
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);


			//法線読み込み
			FbxVector4 normal;

			//GetPolygonVertexNormal(面番号,面の何個目の頂点か)
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertices[index].normal.x = (float)normal[0];
				vertices[index].normal.y = (float)normal[1];
				vertices[index].normal.z = (float)normal[2];
			}


			//UV読み込み
			if(textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if(fbxMesh->GetPolygonVertexUV(i,j,uvNames[0],uvs,lUnmappedUV))
				{
					vertices[index].uv.x = (float)uvs[0];
					vertices[index].uv.y = (float)uvs[1];
				}
			}

			if (j < 3)
				indices.push_back(index);
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}

	}
}

void FbxLoader::parseMaterial(FbxModel* model, FbxMesh* fbxMesh)
{

}

void FbxLoader::parseTexture(FbxModel* model, const std::string& path)
{

}
