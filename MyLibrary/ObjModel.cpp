#include "ObjModel.h"
#include"ModelLoader.h"

ObjModel::ObjModel(){}

ObjModel::~ObjModel(){}


void ObjModel::loadModelVertices
(
	const std::wstring& path,
	const bool& loadUV,
	const bool& loadNormal
)
{
	//objにあるモデル数
	int loadNum = 0;

	//マテリアル
	std::vector<std::string>materialName;

	std::vector<DirectX::XMFLOAT3>bonePos;
	std::vector<std::vector<int>>boneNum;

	//仮配列
	std::vector<std::vector<Vertex>>temporaryVertex;
	std::vector<std::vector<USHORT>>temporaryIndex;

	ModelLoader::getInstance()->loadOBJModel
	(
		path,
		loadUV,
		loadNormal,
		temporaryVertex,
		temporaryIndex,
		materialFileName,
		materialName,
		smoothData,
		&loadNum,
		&bonePos,
		&boneNum
	);
}
