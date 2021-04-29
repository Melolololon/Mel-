#include "Sprite3D.h"
#include"CreatePolygon.h"
PipelineState Sprite3D::defaultPipeline;
DirectX::XMMATRIX Sprite3D::viewAndProjectionMatrix;

Sprite3D::Sprite3D()
{
}

Sprite3D::~Sprite3D()
{
}

void Sprite3D::CreateSprite(const Vector2& size)
{
	CreateBuffer();

	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

	vertices[0].pos = { -size.x + size.x / 2 ,-size.y + size.y / 2 ,0.0f };
	vertices[1].pos = { -size.x + size.x / 2 ,size.y - size.y / 2 ,0.0f };
	vertices[2].pos = { size.x - size.x / 2 ,-size.y + size.y / 2 ,0.0f };
	vertices[3].pos = { size.x - size.x / 2 ,size.y - size.y / 2 ,0.0f };

	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i].pos = vertices[i].pos;

	UnmapVertexBuffer();


	pipeline = defaultPipeline.GetPipelineState();
}

bool Sprite3D::Initialize()
{
	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_NONE;
	data.depthMode = DEPTH_NONE;
	data.drawMode = DRAW_SOLID;
	auto result = defaultPipeline.CreateModelPipeline
	(
		data,
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_SPRITE,
		nullptr,
		typeid(Sprite3D).name()
	);
	if (!result)
	{
		OutputDebugString(L"Sprite3Dの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void Sprite3D::Draw(Texture* texture)
{
	DataMap(viewAndProjectionMatrix, false, texture);
	SetCmdList(texture);
}
