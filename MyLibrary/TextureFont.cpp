#include "TextureFont.h"

bool TextureFont::Load(std::string& path, Value2<UINT> lineNum, std::string& name)
{
    bool result = Texture::Load(path, name);
    if (!result)return false;

    pTextureFonts.emplace(name, std::make_unique<TextureFont>());
    pTextureFonts[name]->pTextures = Texture::Get(name);
    pTextureFonts[name]->fonsLineNum = lineNum;

    return true;
}

void TextureFont::Delete(const std::string& name)
{
    Texture::Delete(name);
    pTextureFonts.erase(name);
}
