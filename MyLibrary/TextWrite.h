#pragma once
#include<d2d1.h>
#pragma comment(lib,"D2d1.lib")
#include<dwrite.h>
#pragma comment(lib,"DWrite.lib")

#include<wrl.h>
#include<string>
#include<unordered_map>
using namespace Microsoft::WRL;

//DirectWriteチュートリアルURL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormatは別クラスにする?
//フォントクラスとテキスト描画クラスを分ける?
//とりあえず描画までやってみてから設計考える

//D3D11On12を使わないと無理っぽい
//使用例のところにやり方書いてる
//https://docs.microsoft.com/en-us/windows/win32/direct3d12/direct3d-11-on-12#initializing-d3d11on12

//DirectWriteを使用したテキスト描画クラス
class TextWrite
{
private:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//DirectWriteを使用するためのもの
	static ComPtr<IDWriteFactory> dWriteFactory;
	static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

	//
	static ComPtr<ID2D1Factory> d2dFactory;
	//レンダリングするためのもの
	static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
	//領域を塗りつぶすもの(もしかしてフォントの塗りつぶしに使う?)
	static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;

	static HWND hwnd;
public:
	static bool Initialize();
	static void LoopStartProcess();
	static void LoopEndProcess();

	static bool CreateFontData(const std::string& name);


	static void Draw(const std::wstring& text, const std::string& fontName);
};

