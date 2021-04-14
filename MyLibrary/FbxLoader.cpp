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
	//Importer‚ðŠJ•ú‚µ‚Ä‚©‚çManager‚ðŠJ•ú‚·‚é
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

void FbxLoader::parseNodeRecursive(FbxModel* model, FbxNode* fbxNode)
{
	std::string name = fbxNode->GetName();

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		parseNodeRecursive(model, fbxNode->GetChild(i));
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

}
