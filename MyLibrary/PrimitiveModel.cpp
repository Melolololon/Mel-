#include "PrimitiveModel.h"

PipelineState PrimitiveModel::defaultPipeline;

PrimitiveModel::PrimitiveModel()
{
	materials.resize(1);
}

PrimitiveModel::~PrimitiveModel()
{
}

void PrimitiveModel::MapVertices()
{
	Vertex* vertex;
	MapVertexBuffer
	(
		0,
		(void**)&vertex
	);

	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i] = vertices[i];
	UnmapVertexBuffer(0);

}

void PrimitiveModel::CalcNormal()
{
	for (int i = 0; i < (int)indices[0].size() / 3; i++)
	{
		DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices[indices[0][i * 3 + 0]].pos);
		DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices[indices[0][i * 3 + 1]].pos);
		DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices[indices[0][i * 3 + 2]].pos);

		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);

		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);
		normal = DirectX::XMVector3Normalize(normal);

		DirectX::XMStoreFloat3(&vertices[indices[0][i * 3 + 0]].normal, normal);
		DirectX::XMStoreFloat3(&vertices[indices[0][i * 3 + 1]].normal, normal);
		DirectX::XMStoreFloat3(&vertices[indices[0][i * 3 + 2]].normal, normal);
	}

	
}

bool PrimitiveModel::Initialize()
{
	PipelineData pipelineData;
	pipelineData.alphaWriteMode = ALPHA_WRITE_TRUE;
	pipelineData.blendMode = BLEND_ADD;
	pipelineData.cullMode = CULL_BACK;
	pipelineData.depthMode = DEPTH_TRUE;
	pipelineData.drawMode = DRAW_SOLID;

	auto result = defaultPipeline.CreatePipeline
	(
		pipelineData,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_MODEL,
		nullptr,
		typeid(PrimitiveModel).name()
	);

	if (!result)
	{
		OutputDebugString(L"PrimitiveModelの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void PrimitiveModel::CreateBox
(
	const Vector3& size, 
	const Color& color, 
	const int modelNum
)
{

#pragma region 頂点インデックスバッファ


#pragma region 頂点決定
	float x = size.x;
	float y = size.y;
	float z = size.z;
	x /= 2;
	y /= 2;
	z /= 2;

	vertices.resize(24);

	vertices[0].pos = { -x,-y,-z };
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].pos = { -x,y,-z };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].pos = { x,-y,-z };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].pos = { x,y,-z };
	vertices[3].uv = { 1.0f,0.0f };



	//正面の上
	vertices[4].pos = { -x, y,-z };
	vertices[4].uv = { 0.0f,1.0f };
	vertices[5].pos = { -x, y,z };
	vertices[5].uv = { 0.0f,0.0f };
	vertices[6].pos = { x,y,-z };
	vertices[6].uv = { 1.0f,1.0f };
	vertices[7].pos = { x,y,z };
	vertices[7].uv = { 1.0f,0.0f };

	//正面の裏
	vertices[8].pos = { -x, y,z };
	vertices[8].uv = { 0.0f,1.0f };
	vertices[9].pos = { -x, -y,z };
	vertices[9].uv = { 0.0f,0.0f };
	vertices[10].pos = { x,y,z };
	vertices[10].uv = { 1.0f,1.0f };
	vertices[11].pos = { x,-y,z };
	vertices[11].uv = { 1.0f,0.0f };

	//正面の下
	vertices[12].pos = { -x, -y,z };
	vertices[12].uv = { 0.0f,1.0f };
	vertices[13].pos = { -x, -y,-z };
	vertices[13].uv = { 0.0f,0.0f };
	vertices[14].pos = { x,-y,z };
	vertices[14].uv = { 1.0f,1.0f };
	vertices[15].pos = { x,-y,-z };
	vertices[15].uv = { 1.0f,0.0f };

	//正面の右
	vertices[16].pos = { x,y,-z };
	vertices[16].uv = { 0.0f,0.0f };
	vertices[17].pos = { x,y,z };
	vertices[17].uv = { 1.0f,0.0f };
	vertices[18].pos = { x,-y,-z };
	vertices[18].uv = { 0.0f,1.0f };
	vertices[19].pos = { x,-y,z };
	vertices[19].uv = { 1.0f,1.0f };

	//正面の左
	vertices[20].pos = { -x,-y,-z };
	vertices[20].uv = { 1.0f,1.0f };
	vertices[21].pos = { -x,-y,z };
	vertices[21].uv = { 0.0f,1.0f };
	vertices[22].pos = { -x,y,-z };
	vertices[22].uv = { 1.0f,0.0f };
	vertices[23].pos = { -x,y,z };
	vertices[23].uv = { 0.0f,0.0f };
#pragma endregion

#pragma region インデックス決定
	indices.resize(1);
	indices[0] = 
	{
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		8,9,10,10,9,11,
		12,13,14,14,13,15,
		16,17,18,18,17,19,
		20,21,22,22,21,23,
	};

#pragma endregion

	CalcNormal();

	std::vector<size_t> verticesNum(1);
	verticesNum[0] = vertices.size();
	CreateModelVertexResources
	(
		sizeof(Vertex),
		verticesNum,
		indices
	);
	MapVertices();
#pragma endregion

#pragma region ヒープとそこにビュー作るバッファ
	CreateModelHeapResourcesSelectColor
	(
		color,
		modelNum,
		1
	);
#pragma endregion

	pipeline = defaultPipeline.GetPipelineState();
}


