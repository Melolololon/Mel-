#include "FbxModel.h"
#include"FbxLoader.h"
FbxModel::FbxModel()
{
}

FbxModel::~FbxModel()
{
}

bool FbxModel::loadModel
(
	const std::string& path,
	const int createNum,
	const size_t constDataSize
)
{
	FbxLoader::getInstance()->loadFbxModel("Resources/cube/cube.fbx");

	std::vector<size_t> verticesNum(1);
	verticesNum[0] = vertices.size();
	
	//バッファ作成
	createModelVertexResources
	(
		sizeof(Vertex),
		verticesNum,
		indices
	);

	createModelHeapResources
	(
		textures,
		createNum,
		1,
		constDataSize
	);

	return true;
}

	
