#include "SpriteFont2D.h"

//Sprite��r���Ńe�N�X�`���ς�����A�ׂ��h��ɂ�����ł���悤�ɂ���
//�������Ȃ��ƃX�v���C�g�t�H���g�̃X�v���C�g���ɗp�ӂł��Ȃ�

void SpriteFont2D::Initialize()
{
	pSprites.resize(MAX_SPRITE_NUM);
	for(auto& p : pSprites)
	{
		//p = std::make_unique<Sprite2D>();
	}
}
