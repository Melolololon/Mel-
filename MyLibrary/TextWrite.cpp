#include "TextWrite.h"
#include"Library.h"
#include"LibWinAPI.h"

ComPtr<IDWriteFactory> TextWrite::dWriteFactory;
std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>TextWrite::pTextFormat;

ComPtr<ID3D11Device> TextWrite::d3d11Device;
ComPtr<ID3D11DeviceContext>TextWrite::d3d11context;
ComPtr<ID3D11On12Device>TextWrite::d3d11On12device;
ComPtr<ID3D11Resource> TextWrite::wrappedBackBuffer[2];

ComPtr<ID2D1Factory3> TextWrite::d2dFactory;
ComPtr<ID2D1Device2>TextWrite::d2dDevice;
ComPtr<IDXGIDevice>TextWrite::dxgiDevice;
ComPtr<ID2D1DeviceContext> TextWrite::d2dContext;
ComPtr<ID2D1Bitmap1>TextWrite::bitmap;

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


bool TextWrite::Initialize
(
    ID3D12Device* pD3D12Device,
    ID3D12CommandQueue** pPD3D12Queue, 
    ID3D12Resource* pBuckBuffers[2],
    CD3DX12_CPU_DESCRIPTOR_HANDLE backBufferHandle[2]
)
{


    UINT flag =
      D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    auto result = D3D11On12CreateDevice
    (
        pD3D12Device,
        flag,
        nullptr,//nullptrにするとD3D12の方と同じレベルになる
        0,//上の引数の配列数(今回はここで指定しないから0)
        reinterpret_cast<IUnknown**>(pPD3D12Queue),//D3D12のキューのポインタをキャストしたものでいい?
        1,//キューの数
        0,//D3D12デバイスのノード
        &d3d11Device,
        &d3d11context,
        nullptr//選ばれたレベルを取得する場合ポインタを渡す
    );

    //これ調べる
    //https://docs.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(q)
    //https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
    //チュートリアルには「ID3D11Device の作成後、そこから ID3D11On12Device インターフェイスを照会できます。
    //これは、D2D を設定するために使用されるプライマリ デバイス オブジェクトです。」
    //と書かれている
    result = d3d11Device.As(&d3d11On12device);

    result = D2D1CreateFactory
    (
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory3),
        &d2dFactory
        //IID_PPV_ARGS(&d2dFactory)
    );

    result = d3d11On12device.As(&dxgiDevice);

    //D3D11_CREATE_DEVICE_DEBUGのせいで生成できてなかった
    result = d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);
    
    result = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,&d2dContext);

    IDWriteFactory** pPWriteFactory = &dWriteFactory;
    result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(pPWriteFactory)
    );  


    //1インチ(2.54cm)あたりのドット数の取得
    float dpiX, dpiY;
    d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    //ビットマップオプションろカラーコンテキスト情報の設定?
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1
    (
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,//ビットマップを生成するためのDXGI形式とアルファモード
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpiX,
        dpiY
    );

    //バックバッファ分ループ
    for(int i = 0; i < 2;i++)
    {
        D3D11_RESOURCE_FLAGS d3d11Flags = {};
        //D3D12のリソースを、D3D11が理解できるようにするためのもの(今回の場合はバックバッファだからD3D11_BIND_RENDER_TARGETを指定?)
        d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET;

        //12のバックバッファをもとに11のバックバッファを生成?
        //「このメソッドは、D3D11on12で使用するD3D11リソースを作成します。」ドキュメントより
        result = d3d11On12device->CreateWrappedResource
        (
            pBuckBuffers[i],
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&wrappedBackBuffer[i])
        );

        //「このバックバッファに直接描画するD2Dのレンダーターゲットを作成します。」チュートリアルの文
        //IDXGISurfaceインターフェイスは、画像データオブジェクトのメソッドを実装します。(ドキュメントの説明を翻訳した文)
        ComPtr<IDXGISurface>dxgiSurface;
        wrappedBackBuffer[i].As(&dxgiSurface);
        d2dContext->CreateBitmapFromDxgiSurface
        (
            dxgiSurface.Get(),
            &bitmapProperties,
            &bitmap
        );

        backBufferHandle[i].Offset(1, pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
        //アロケーターとレンダーターゲットはDirectX12クラスの使うかわからんから、一旦保留して先に進める
    }
   

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
