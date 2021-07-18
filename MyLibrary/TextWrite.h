#pragma once
#include<d2d1.h>
#include<dwrite.h>
#include<wrl.h>
#include<string>
#include<unordered_map>
using namespace Microsoft::WRL;

//DirectWriteチュートリアルURL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormatは別クラスにする?
//フォントクラスとテキスト描画クラスを分ける?

//DirectWriteを使用したテキスト描画クラス
class TextWrite
{
private:
	//DirectWriteを使用するためのもの
	static ComPtr<IDWriteFactory> dWriteFactory;
	static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

	//
	static ComPtr<ID2D1Factory> d2dFactory;
	//レンダリングするためのもの
	static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
	//領域を塗りつぶすもの(もしかしてフォントの塗りつぶしに使う?)
	static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;


public:
	static bool Initialize();

	static bool CreateFontData(const std::string& name);


	void Write(const std::wstring& text);
};

