#include "FbxModel.h"
#include"FbxLoader.h"


PipelineState FbxModel::defaultPipeline;

FbxModel::FbxModel()
{
	

}

FbxModel::~FbxModel()
{
}

bool FbxModel::modelNum
(
	const std::string& path,
	const int createNum,
	//const size_t vertexSize,
	const size_t constDataSize
)
{

	FbxLoader::GetInstance()->LoadFbxModel("Resources/cube/cube.fbx",this);

	std::vector<size_t> verticesNum(1);
	verticesNum[0] = vertices.size();
	
	//バッファ作成
	CreateModelVertexResources
	(
		sizeof(Vertex),
		verticesNum,
		indices
	);


	Vertex* vertex;
	MapVertexBuffer
	(
		0,
		(void**)&vertex
	);

	auto vertexNum = vertices.size();
	for(int i = 0; i < vertexNum;i++)
		vertex[i] = vertices[i];
	UnmapVertexBuffer(0);


	auto texNum = textures.size();
	std::vector<Texture*>pTextures(texNum);
	for (int i = 0; i < texNum; i++)
		pTextures[i] = textures[i].get();



	CreateModelHeapResourcesSetTexture
	(
		pTextures,
		createNum,
		1,
		constDataSize
	);


	pipeline = defaultPipeline.GetPipelineState();


	//一時的に書いてる
	materials[0].ambient = { 0.1f,0.1f,0.1f };
	materials[0].diffuse = { 1.0f,1.0f,1.0f };

	return true;
}

bool FbxModel::Initialize()
{
	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_BACK;
	data.depthMode = DEPTH_TRUE;
	data.drawMode = DRAW_SOLID;
	auto result = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_MODEL,
		nullptr,
		typeid(FbxModel).name()
	);
	if (!result)
	{
		OutputDebugString(L"FbxModelの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

