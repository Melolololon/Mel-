#pragma once
#include "Sprite2DBase.h"


//���������X�v���C�g�p�ӂ���̂��������Ȃ�����ǂ��ɂ�����?

//����Sprite2D��Sprite2DBase�ɂ��āA�����Sprite2D�ɂ���?
//Draw�̂Ƃ��ɃZ�b�g���Ă�F�ׂ��h�肩�A�e�N�X�`����`�悷�邩�I�ׂ�悤�ɂ���?
//�Ƃ肠�������ꂢ���BSetColor�֐����邯�ǁA���p�҂�SpriteFont2D�N���X�𒇉��Sprite�N���X�����邩��
//�ł����ꂾ��RenderTarget�N���X�ɉe���o��B�����Base�N���X���

//�X�v���C�g�t�H���g�p�X�v���C�g
class FontSprite2D :
    public Sprite2DBase
{
private:

public:

    bool SetFontTexture(Texture* pTexture);
};

