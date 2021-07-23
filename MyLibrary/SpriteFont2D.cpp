#include "SpriteFont2D.h"

//Spriteを途中でテクスチャ変えたり、べた塗りにしたりできるようにする
//そうしないとスプライトフォントのスプライトを先に用意できない

void SpriteFont2D::Initialize()
{
	pSprites.resize(MAX_SPRITE_NUM);
	for(auto& p : pSprites)
	{
		//p = std::make_unique<Sprite2D>();
	}
}
