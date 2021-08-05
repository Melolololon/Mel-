#include "Sprite2D.h"

using namespace melLib;

std::unordered_map<std::string, std::unique_ptr<Sprite2D>> Sprite2D::pSprite2D;

Sprite2D::Sprite2D()
{
}

Sprite2D::Sprite2D(const Color& color)
{
	CreateSetColor(color);
}

Sprite2D::Sprite2D(Texture* pTexture)
{
	CreateSetTexture(pTexture);
}

Sprite2D::~Sprite2D()
{
}

bool Sprite2D::CreateSetColor(const Color& color)
{
	SpriteInitialize();
	SetOneColorSpriteColor(color);
	drawMode = DrawMode::DRAW_COLOR;

	pipeline = defaultPipeline.GetPipelineState();
	return true;
}

bool Sprite2D::CreateSetTexture(Texture* pTexture)
{
	if (pTexture)
	{
		//�e�N�X�`���ɍ��킹�ăT�C�Y�ύX
		drawRightDownPosition = pTexture->GetTextureSize();
	}
	this->pTexture = pTexture;
	

	SpriteInitialize();
	SetColor(Color(0, 0, 0, 0));

	drawMode = DrawMode::DRAW_TEXTURE;
	pipeline = defaultPipeline.GetPipelineState();
	return true;
}

bool Sprite2D::Create(const Color& color, const std::string& name)
{
	pSprite2D.emplace(name, std::make_unique<Sprite2D>());
	bool result = pSprite2D[name]->CreateSetColor(color);

#ifdef _DEBUG
	if (!result)
	{
		OutputDebugStringA(name.c_str());
		OutputDebugStringW(L"�̐����Ɏ��s���܂����B\n");
	}
#endif // _DEBUG

	return result;

}

bool Sprite2D::Create(Texture* pTexture, const std::string& name)
{
	pSprite2D.emplace(name, std::make_unique<Sprite2D>());
	bool result = pSprite2D[name]->CreateSetTexture(pTexture);

#ifdef _DEBUG
	if(!result)
	{
		OutputDebugStringA(name.c_str());
		OutputDebugStringW(L"�̐����Ɏ��s���܂����B\n");
	}
#endif // _DEBUG

	return result;
}

void Sprite2D::Delete(const std::string& name)
{
	pSprite2D.erase(name);
}

void Sprite2D::Draw(const std::string& rtName)
{
	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

	Vector2 textureSize = 1;
	if (pTexture)  textureSize = pTexture->GetTextureSize();

#pragma region ���_���W

	vertices[0].pos = { -textureSize.x / 2 , textureSize.y / 2, 0 };
	vertices[1].pos = { -textureSize.x / 2 ,-textureSize.y / 2,0 };
	vertices[2].pos = { textureSize.x / 2  ,   textureSize.y / 2 ,0 };
	vertices[3].pos = { textureSize.x / 2  , -textureSize.y / 2,0 };

#pragma endregion

#pragma region UV���W


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
	
	Texture* pTex = pTexture;
	if (drawMode == DrawMode::DRAW_COLOR)pTex = nullptr;
	MatrixMap(pTex);
	SetCmdList(pTex);
}

void Sprite2D::SetColor(const Color& color)
{
	this->color = color;
	SetOneColorSpriteColor(color);
}
