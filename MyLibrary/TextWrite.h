#pragma once
#include<d2d1.h>
#pragma comment(lib,"D2d1.lib")
#include<dwrite.h>
#pragma comment(lib,"DWrite.lib")

#include<wrl.h>
#include<string>
#include<unordered_map>
using namespace Microsoft::WRL;

//DirectWrite�`���[�g���A��URL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormat�͕ʃN���X�ɂ���?
//�t�H���g�N���X�ƃe�L�X�g�`��N���X�𕪂���?
//�Ƃ肠�����`��܂ł���Ă݂Ă���݌v�l����

//D3D11On12���g��Ȃ��Ɩ������ۂ�
//�g�p��̂Ƃ���ɂ��������Ă�
//https://docs.microsoft.com/en-us/windows/win32/direct3d12/direct3d-11-on-12#initializing-d3d11on12

//DirectWrite���g�p�����e�L�X�g�`��N���X
class TextWrite
{
private:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//DirectWrite���g�p���邽�߂̂���
	static ComPtr<IDWriteFactory> dWriteFactory;
	static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

	//
	static ComPtr<ID2D1Factory> d2dFactory;
	//�����_�����O���邽�߂̂���
	static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
	//�̈��h��Ԃ�����(���������ăt�H���g�̓h��Ԃ��Ɏg��?)
	static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;

	static HWND hwnd;
public:
	static bool Initialize();
	static void LoopStartProcess();
	static void LoopEndProcess();

	static bool CreateFontData(const std::string& name);


	static void Draw(const std::wstring& text, const std::string& fontName);
};

