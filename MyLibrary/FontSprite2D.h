#pragma once
#include "Sprite2D.h"


//���������X�v���C�g�p�ӂ���̂��������Ȃ�����ǂ��ɂ�����?

//����Sprite2D��Sprite2DBase�ɂ��āA�����Sprite2D�ɂ���?

//�X�v���C�g�t�H���g�p�X�v���C�g
class FontSprite2D :
    public Sprite2D
{
private:

public:

    bool SetFontTexture(Texture* pTexture);
};

