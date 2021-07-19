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
    //L"EDIT"とかじゃない子ウィンドウは、親の名前渡さないといけない?
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
        L"Gabriola",//フォントファミリーの名前を含む文字列
        NULL,//フォントコレクションオブジェクトへのポインタ
        DWRITE_FONT_WEIGHT_REGULAR,//太さ(enumで指定)
        DWRITE_FONT_STYLE_NORMAL,//スタイル(enumで指定)。ここで文字を斜めにしたりできる
        DWRITE_FONT_STRETCH_NORMAL,//ストレッチ(enumで指定)。文字の引き伸ばし具合
        72.0f,//ピクセルが単位のフォントサイズ?
        L"en_us",//ロケール名を含む文字列
        &pWriteTextFormat
    );

    //文字の揃え方設定。この場合、中央揃え
    //左右位置
    result = pWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    //上下位置
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

    //テキスト描画
    d2dRenderTarget->DrawTextW
    (
        text.c_str(),
        text.size(),
        pTextFormat[fontName].Get(),
        layoutRect,
        d2dSolidColorBrush.Get()
    );
}
