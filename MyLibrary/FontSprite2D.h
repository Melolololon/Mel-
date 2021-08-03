#pragma once
#include "Sprite2DBase.h"


//文字数文スプライト用意するのもったいないからどうにかする?

//今のSprite2DをSprite2DBaseにして、これをSprite2Dにする?
//Drawのときにセットしてる色べた塗りか、テクスチャを描画するか選べるようにする?
//とりあえずこれいらん。SetColor関数あるけど、利用者はSpriteFont2Dクラスを仲介してSpriteクラスいじるから
//でもそれだとRenderTargetクラスに影響出る。やっぱBaseクラス作る

//スプライトフォント用スプライト
class FontSprite2D :
    public Sprite2DBase
{
private:

public:

    bool SetFontTexture(Texture* pTexture);
};

