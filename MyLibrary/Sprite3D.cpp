#include "Sprite3D.h"
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
	data.depthMode = DEPTH_TRUE;
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


void Sprite3D::SelectDrawAreaDraw
(
	const Vector2& leftUpPos,
	const Vector2& rightDownPos,
	Texture* texture
)
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

#pragma region UV座標

	Vector2 textureSize = texture->GetTextureSize();
	Vector2 uvLeftUp = { 1.0f / textureSize.x * leftUpPos.x ,1.0f / textureSize.y * leftUpPos.y };
	Vector2 uvRightDown = { 1.0f / textureSize.x * rightDownPos.x ,1.0f / textureSize.y * rightDownPos.y };

	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
	vertices[1].uv = { uvLeftUp.x,uvLeftUp.y };
	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
	vertices[3].uv = { uvRightDown.y ,uvLeftUp.y };
#pragma endregion


	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i].uv = vertices[i].uv;

	UnmapVertexBuffer();

	DataMap(viewAndProjectionMatrix, true, texture);
	SetCmdList(texture);
}
