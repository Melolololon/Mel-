#pragma once
#include "Sprite2DBase.h"
class Sprite2D :
    public Sprite2DBase
{
public:
	enum class DrawMode
	{
		DRAW_TEXTURE,
		DRAW_COLOR,
	};

private:

	static std::unordered_map<std::string, std::unique_ptr<Sprite2D>> pSprite2D;

	DrawMode drawMode = DrawMode::DRAW_TEXTURE;

public:
	Sprite2D();
	Sprite2D(const Color& color);
	Sprite2D(Texture* pTexture);
	~Sprite2D();

	bool CreateSetColor(const Color& color);
	bool CreateSetTexture(Texture* pTexture);

	static bool Create(const Color& color, const std::string& name);
	static bool Create(Texture* pTexture, const std::string& name);
	static void Delete(const std::string& name);
	static Sprite2D* Get(const std::string& name) { return pSprite2D[name].get(); }

	void Draw(const std::string& rtName = "");

	void SetColor(const Color& color);
	void SetTexture(Texture* pTexture) { this->pTexture = pTexture; }
	//void SetDrawType(const DrawMode mode) { drawMode = mode; }
};

