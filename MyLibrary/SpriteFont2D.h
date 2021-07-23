#pragma once
#include"Sprite2D.h"
class SpriteFont2D
{
private:
	static const int MAX_SPRITE_NUM = 500;
	std::vector<std::unique_ptr<Sprite2D>>pSprites;
public:
	SpriteFont2D(SpriteFont2D& spr) = delete;
	SpriteFont2D& operator=(SpriteFont2D& spr) = delete;
	static SpriteFont2D* GetInstance();

	void Initialize();
	//void DrawString(const std::string& text, );

};

