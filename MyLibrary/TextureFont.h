#pragma once
#include"Texture.h"
#include"Values.h"

//SpriteFont�Ŏg�p����e�N�X�`���N���X
class TextureFont
{
private:
	static std::unordered_map < std::string, std::unique_ptr<TextureFont >> pTextureFonts;

	Texture* pTextures = nullptr;

	//�c���̐�
	Value2<UINT>fonsLineNum;

public:

	Value2<UINT>GetFontLineNumber() { return fonsLineNum; }


	static bool Load(std::string& path, Value2<UINT>lineNum, std::string& name);
	static TextureFont* Get(const std::string& name) { return pTextureFonts[name].get(); }
	static void Delete(const std::string& name);
};

