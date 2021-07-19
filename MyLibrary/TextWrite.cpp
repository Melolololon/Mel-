#include "TextWrite.h"
#include"Library.h"
#include"LibWinAPI.h"

ComPtr<IDWriteFactory> TextWrite::dWriteFactory;
std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>TextWrite::pTextFormat;


ComPtr<ID2D1Factory> TextWrite::d2dFactory;
ComPtr<ID2D1HwndRenderTarget> TextWrite::d2dRenderTarget;
ComPtr<ID2D1SolidColorBrush> TextWrite::d2dSolidColorBrush;
HWND TextWrite::hwnd;

LRESULT TextWrite::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
   /* switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }*/
    return DefWindowProc(hwnd, msg, wparam, lparam);

}


bool TextWrite::Initialize()
{
    //L"EDIT"�Ƃ�����Ȃ��q�E�B���h�E�́A�e�̖��O�n���Ȃ��Ƃ����Ȃ�?
  /*  hwnd = LibWinAPI::CreateExpansionWindow
    (
        Library::GetWindowName(),
        L"DirectWrite",
        0,
        WS_CHILD | WS_VISIBLE,
        0,
        0,
        Library::GetWindowWidth(),
        Library::GetWindowHeight(),
       ,
        WindowProc
    );*/

    auto result = D2D1CreateFactory
    (
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        IID_PPV_ARGS(&d2dFactory)
    );
    
    IDWriteFactory** pPWriteFactory = &dWriteFactory;
    result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(pPWriteFactory)
    );  

   
    
    result = d2dFactory->CreateHwndRenderTarget
    (
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(Library::GetHWND(), D2D1::SizeU(Library::GetWindowWidth(), Library::GetWindowHeight())),
        &d2dRenderTarget
    );

    result = d2dRenderTarget->CreateSolidColorBrush
    (
        D2D1::ColorF(D2D1::ColorF::Black),
        &d2dSolidColorBrush
    );

    return true;
}

void TextWrite::LoopStartProcess()
{
    d2dRenderTarget->BeginDraw();
    d2dRenderTarget->SetTransform(D2D1::IdentityMatrix());
    d2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White, 0.0f));
}

void TextWrite::LoopEndProcess()
{
    d2dRenderTarget->EndDraw(); 
 

}

bool TextWrite::CreateFontData(const std::string& name)
{
    ComPtr<IDWriteTextFormat>pWriteTextFormat;

    //https://docs.microsoft.com/ja-jp/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createtextformat
    auto result = dWriteFactory->CreateTextFormat
    (
        L"Gabriola",//�t�H���g�t�@�~���[�̖��O���܂ޕ�����
        NULL,//�t�H���g�R���N�V�����I�u�W�F�N�g�ւ̃|�C���^
        DWRITE_FONT_WEIGHT_REGULAR,//����(enum�Ŏw��)
        DWRITE_FONT_STYLE_NORMAL,//�X�^�C��(enum�Ŏw��)�B�����ŕ������΂߂ɂ�����ł���
        DWRITE_FONT_STRETCH_NORMAL,//�X�g���b�`(enum�Ŏw��)�B�����̈����L�΂��
        72.0f,//�s�N�Z�����P�ʂ̃t�H���g�T�C�Y?
        L"en_us",//���P�[�������܂ޕ�����
        &pWriteTextFormat
    );

    //�����̑������ݒ�B���̏ꍇ�A��������
    //���E�ʒu
    result = pWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    //�㉺�ʒu
    result = pWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


    pTextFormat.emplace(name, pWriteTextFormat);
    return true;
}

void TextWrite::Draw(const std::wstring& text, const std::string& fontName)
{
    D2D1_RECT_F layoutRect = D2D1::RectF
    (
        0,
        0,
        300,
        300
    );

    //�e�L�X�g�`��
    d2dRenderTarget->DrawTextW
    (
        text.c_str(),
        text.size(),
        pTextFormat[fontName].Get(),
        layoutRect,
        d2dSolidColorBrush.Get()
    );
}
