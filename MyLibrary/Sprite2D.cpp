#include "Sprite2D.h"

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

void Sprite2D::SelectDrawAreaDraw
(
	const Vector2& leftUpPos, 
	const Vector2& rightDownPos, 
	Texture* texture
)
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

#pragma region 頂点座標
	Vector2 spriteSize = rightDownPos - leftUpPos;

	vertices[0].pos = { -spriteSize.x / 2, spriteSize.y / 2, 0 };
	vertices[1].pos = { -spriteSize.x / 2 ,-spriteSize.y / 2,0 };
	vertices[2].pos = { spriteSize.x / 2,   spriteSize.y / 2 ,0 };
	vertices[3].pos = { spriteSize.x / 2 , -spriteSize.y / 2,0 };

#pragma endregion

#pragma region UV座標

	Vector2 textureSize = texture->GetTextureSize();
	Vector2 uvLeftUp = { 1.0f / textureSize.x * leftUpPos.x ,1.0f / textureSize.y * leftUpPos.y };
	Vector2 uvRightDown = { 1.0f / textureSize.x * rightDownPos.x ,1.0f / textureSize.y * rightDownPos.y };

	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
	vertices[1].uv = { uvLeftUp .x,uvLeftUp .y};
	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
	vertices[3].uv = { uvRightDown.x ,uvLeftUp.y };
#pragma endregion


	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i] = vertices[i];

	UnmapVertexBuffer();

	CommonDataMat();
	MatrixMap(texture);
	SetCmdList(texture);
}

void Sprite2D::Draw(Texture* texture)
{
	//ここで頂点のマップをする
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

	Vector2 textureSize = texture->GetTextureSize();
	
	vertices[0].pos = { 0 - textureSize.x / 2,textureSize.y - textureSize.y / 2,0, };
	vertices[1].pos = { 0 - textureSize.x / 2,0 - textureSize.y / 2, 0, };
	vertices[2].pos = { textureSize.x - textureSize.x / 2,textureSize.y - textureSize.y / 2,0 };
	vertices[3].pos = { textureSize.x - textureSize.x / 2,0 - textureSize.y / 2,0, };
	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i].pos = vertices[i].pos;

	UnmapVertexBuffer();

	CommonDataMat();
	MatrixMap(texture);
	SetCmdList(texture);
}


void Sprite2D::MatrixMap(Texture* texture)
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld *= DirectX::XMMatrixScaling
	(
		constData.scale.x,
		constData.scale.y,
		1
	);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(constData.angle.z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(constData.angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(constData.angle.y));

	Vector2 textureSize = texture->GetTextureSize();
	float width = textureSize.x;
	float height = textureSize.y;
	width /= 2;
	height /= 2;

	matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x + width * constData.scale.x,
		constData.position.y + height * constData.scale.y,
		0.0f
	);

	constBufferData->mat = matWorld * cameraMatrix;


	constBuffer->Unmap(0, nullptr);
}

