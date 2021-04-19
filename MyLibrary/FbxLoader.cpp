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

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		parseNodeRecursive(model, fbxNode->GetChild(i), &node);
}

void FbxLoader::parseMesh(FbxModel* model, FbxNode* node)
{

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

