#include "Sprite2D.h"

DirectX::XMMATRIX Sprite2D::cameraMatrix;
PipelineState Sprite2D::defaultPipeline;


Sprite2D::Sprite2D(const Color& color)
{
	Create(color);
	InitializeVertices();
}

Sprite2D::Sprite2D(Texture* pTexture)
{
	Create(pTexture);
	InitializeVertices();
}


Sprite2D::~Sprite2D(){}

void Sprite2D::InitializeVertices()
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);
	vertices[0].pos = { -0.5f,0.5f ,0.0f };
	vertices[1].pos = { -0.5f,-0.5f ,0.0f };
	vertices[2].pos = { 0.5f,0.5f ,0.0f };
	vertices[3].pos = { 0.5f,-0.5f,0.0f };
	vertex[0].pos = vertices[0].pos;
	vertex[1].pos = vertices[1].pos;
	vertex[2].pos = vertices[2].pos;
	vertex[3].pos = vertices[3].pos;

	vertices[0].uv = { 0,1 };
	vertices[1].uv = { 0,0 };
	vertices[2].uv = { 1,1 };
	vertices[3].uv = { 1,0 };
	vertex[0].uv = vertices[0].uv;
	vertex[1].uv = vertices[1].uv;
	vertex[2].uv = vertices[2].uv;
	vertex[3].uv = vertices[3].uv;
	UnmapVertexBuffer();
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
	data.depthMode = DEPTH_FALSE;
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
		typeid(Sprite2D).name(),
		1
	);
	if (!result)
	{
		OutputDebugString(L"Sprite2Dの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void Sprite2D::Create(const Color& color)
{
	CreateBuffer();
	SetOneColorSpriteColor(color);
	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite2D::Create(Texture* pTexture)
{
	this->pTexture = pTexture;
	//テクスチャがあったら描画範囲変更
	if (pTexture) drawRightDownPosition = pTexture->GetTextureSize();
	CreateBuffer();
	pipeline = defaultPipeline.GetPipelineState();
}

void Sprite2D::Draw()
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

#pragma region 頂点座標
	Vector2 spriteSize = drawRightDownPosition - drawLeftUpPosition;

	vertices[0].pos = { -spriteSize.x / 2 , spriteSize.y / 2, 0 };
	vertices[1].pos = { -spriteSize.x / 2 ,-spriteSize.y / 2,0 };
	vertices[2].pos = { spriteSize.x / 2  ,   spriteSize.y / 2 ,0 };
	vertices[3].pos = { spriteSize.x / 2  , -spriteSize.y / 2,0 };

#pragma endregion

#pragma region UV座標

	Vector2 textureSize = 1;
	if (pTexture)  textureSize = pTexture->GetTextureSize();
	Vector2 uvLeftUp = { 1.0f / textureSize.x * drawLeftUpPosition.x ,1.0f / textureSize.y * drawLeftUpPosition.y };
	Vector2 uvRightDown = { 1.0f / textureSize.x * drawRightDownPosition.x ,1.0f / textureSize.y * drawRightDownPosition.y };

	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
	vertices[1].uv = { uvLeftUp.x,uvLeftUp.y };
	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
	vertices[3].uv = { uvRightDown.x ,uvLeftUp.y };
#pragma endregion


	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i] = vertices[i];

	UnmapVertexBuffer();

	ConstDataMat();
	MatrixMap(pTexture);
	SetCmdList(pTexture);
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

	Vector2 textureSize = 1.0f;
	if(texture)textureSize = texture->GetTextureSize();
	float width = textureSize.x;
	float height = textureSize.y;
	width /= 2;
	height /= 2;



	//左上基準拡縮
	matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x + (width * constData.scale.x) + (vertices[2].pos.x - width),
		constData.position.y + (height * constData.scale.y) + (vertices[0].pos.y - height),
		0.0f
	);

	//中心基準拡縮
	/*matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x + (vertices[2].pos.x - width) + width,
		constData.position.y + (vertices[0].pos.y - height) + height,
		0.0f
	);*/


	constBufferData->mat = matWorld * cameraMatrix;


	constBuffer->Unmap(0, nullptr);
}

