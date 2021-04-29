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

	std::vector<Vertex>vertexVector;
	vertexVector = CreatePolygon::GetInstance()->SetBoardPolygonVertex
	(
		{ 0 - width,textureSize.y - height,0, },
		{ 0 - width,0 - height, 0, },
		{ textureSize.x - width,textureSize.y - height,0 },
		{ textureSize.x - width,0 - height,0, }
	);

	vertices[0].pos = vertexVector[0].pos;
	vertices[1].pos = vertexVector[1].pos;
	vertices[2].pos = vertexVector[2].pos;
	vertices[3].pos = vertexVector[3].pos;
	vertex[0].pos = vertexVector[0].pos;
	vertex[1].pos = vertexVector[1].pos;
	vertex[2].pos = vertexVector[2].pos;
	vertex[3].pos = vertexVector[3].pos;

	UnmapVertexBuffer();

	DataMap(cameraMatrix, true, texture);
	SetCmdList(texture);
}
