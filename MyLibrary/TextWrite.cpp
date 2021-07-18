#include "TextWrite.h"
#include"Library.h"

bool TextWrite::Initialize()
{
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

   
    
    d2dFactory->CreateHwndRenderTarget
    (
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(Library::GetHWND(), D2D1::SizeU(Library::GetWindowWidth(), Library::GetWindowHeight())),
        &d2dRenderTarget
    );

    d2dRenderTarget->CreateSolidColorBrush
    (
        D2D1::ColorF(D2D1::ColorF::Black),
        &d2dSolidColorBrush
    );

}

bool TextWrite::CreateFontData(const std::string& name)
{
    ComPtr<IDWriteTextFormat>pWriteTextFormat;
    IDWriteTextFormat** pPWriteTextFormat = &pWriteTextFormat;
    auto result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteTextFormat),
        reinterpret_cast<IUnknown**>(pPWriteTextFormat)
    );


    //https://docs.microsoft.com/ja-jp/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createtextformat
    result = dWriteFactory->CreateTextFormat
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
}
