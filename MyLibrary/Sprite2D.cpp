#include "Sprite2D.h"
#include"CreatePolygon.h"

DirectX::XMMATRIX Sprite2D::cameraMatrix;
PipelineState Sprite2D::defaultPipeline;
Sprite2D::Sprite2D()
{

}

Sprite2D::~Sprite2D(){}


void Sprite2D::CreateSprite()
{
	CreateBuffer();

	pipeline = defaultPipeline.GetPipelineState();
}

bool Sprite2D::Initialize(const int winWidth, const int winHeight)
{
	cameraMatrix = DirectX::XMMatrixOrthographicOffCenterLH
	(
		0.0f,
		winWidth,
		winHeight,
		0.0f,
		0.0f,
		1.0f
	);

	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_NONE;
	data.depthMode = DEPTH_NONE;
	data.drawMode = DRAW_SOLID;
	auto result = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_SPRITE,
		nullptr,
		typeid(Sprite2D).name()
	);
	if (!result)
	{
		OutputDebugString(L"Sprite2Dの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void Sprite2D::Draw(Texture* texture)
{
	//ここで頂点のマップをする
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

	Vector2 textureSize = texture->GetTextureSize();
	
	float width = textureSize.x;
	float height = textureSize.y;
	width /= 2;
	height /= 2;

	vertices[0].pos = { 0 - width,textureSize.y - height,0, };
	vertices[1].pos = { 0 - width,0 - height, 0, };
	vertices[2].pos = { textureSize.x - width,textureSize.y - height,0 };
	vertices[3].pos = { textureSize.x - width,0 - height,0, };
	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i].pos = vertices[i].pos;

	UnmapVertexBuffer();

	DataMap(cameraMatrix, true, texture);
	SetCmdList(texture);
}


