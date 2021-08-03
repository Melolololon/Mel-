#pragma once
#include "Sprite2D.h"


//文字数文スプライト用意するのもったいないからどうにかする?

//今のSprite2DをSprite2DBaseにして、これをSprite2Dにする?

//スプライトフォント用スプライト
class FontSprite2D :
    public Sprite2D
{
private:

public:

    bool SetFontTexture(Texture* pTexture);
};

