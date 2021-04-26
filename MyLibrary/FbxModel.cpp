#include "FbxModel.h"
#include"FbxLoader.h"


PipelineState FbxModel::defaultFbxPipeline;

FbxModel::FbxModel()
{
	pipeline = defaultFbxPipeline.getPipelineState();
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


	FbxLoader::getInstance()->loadFbxModel("Resources/cube/cube.fbx",this);

	std::vector<size_t> verticesNum(1);
	verticesNum[0] = vertices.size();
	
	//バッファ作成
	createModelVertexResources
	(
		sizeof(Vertex),
		verticesNum,
		indices
	);

	auto texNum = textures.size();
	std::vector<Texture*>pTextures(texNum);
	for (int i = 0; i < texNum; i++)
		pTextures[i] = textures[i].get();

	createModelHeapResources
	(
		pTextures,
		createNum,
		1,
		constDataSize
	);

	return true;
}

bool FbxModel::initialize()
{
	/*PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_BACK;
	data.depthMode = DEPTH_TRUE;
	data.drawMode = DRAW_SOLID;
	auto result = defaultFbxPipeline.createModelPipeline
	(
		data,
		{ L"../MyLibrary/ObjAnimationVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		nullptr,
		typeid(FbxModel).name()
	);
	if (!result)
	{
		OutputDebugString(L"FbxModelの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}*/
	return true;
}

