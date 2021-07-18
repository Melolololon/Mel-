#pragma once
#include<d2d1.h>
#include<dwrite.h>
#include<wrl.h>
#include<string>
#include<unordered_map>
using namespace Microsoft::WRL;

//DirectWrite�`���[�g���A��URL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormat�͕ʃN���X�ɂ���?
//�t�H���g�N���X�ƃe�L�X�g�`��N���X�𕪂���?

//DirectWrite���g�p�����e�L�X�g�`��N���X
class TextWrite
{
private:
	//DirectWrite���g�p���邽�߂̂���
	static ComPtr<IDWriteFactory> dWriteFactory;
	static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

	//
	static ComPtr<ID2D1Factory> d2dFactory;
	//�����_�����O���邽�߂̂���
	static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
	//�̈��h��Ԃ�����(���������ăt�H���g�̓h��Ԃ��Ɏg��?)
	static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;


public:
	static bool Initialize();

	static bool CreateFontData(const std::string& name);


	void Write(const std::wstring& text);
};

