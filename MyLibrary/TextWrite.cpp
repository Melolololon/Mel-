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
        nullptr,//nullptr�ɂ����D3D12�̕��Ɠ������x���ɂȂ�
        0,//��̈����̔z��(����͂����Ŏw�肵�Ȃ�����0)
        reinterpret_cast<IUnknown**>(pPD3D12Queue),//D3D12�̃L���[�̃|�C���^���L���X�g�������̂ł���?
        1,//�L���[�̐�
        0,//D3D12�f�o�C�X�̃m�[�h
        &d3d11Device,
        &d3d11context,
        nullptr//�I�΂ꂽ���x�����擾����ꍇ�|�C���^��n��
    );

    //���꒲�ׂ�
    //https://docs.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(q)
    //https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
    //�`���[�g���A���ɂ́uID3D11Device �̍쐬��A�������� ID3D11On12Device �C���^�[�t�F�C�X���Ɖ�ł��܂��B
    //����́AD2D ��ݒ肷�邽�߂Ɏg�p�����v���C�}�� �f�o�C�X �I�u�W�F�N�g�ł��B�v
    //�Ə�����Ă���
    result = d3d11Device.As(&d3d11On12device);

    result = D2D1CreateFactory
    (
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory3),
        &d2dFactory
        //IID_PPV_ARGS(&d2dFactory)
    );

    result = d3d11On12device.As(&dxgiDevice);

    //D3D11_CREATE_DEVICE_DEBUG�̂����Ő����ł��ĂȂ�����
    result = d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);
    
    result = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,&d2dContext);

    IDWriteFactory** pPWriteFactory = &dWriteFactory;
    result = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(pPWriteFactory)
    );  


    //1�C���`(2.54cm)������̃h�b�g���̎擾
    float dpiX, dpiY;
    d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    //�r�b�g�}�b�v�I�v�V������J���[�R���e�L�X�g���̐ݒ�?
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1
    (
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,//�r�b�g�}�b�v�𐶐����邽�߂�DXGI�`���ƃA���t�@���[�h
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpiX,
        dpiY
    );

    //�o�b�N�o�b�t�@�����[�v
    for(int i = 0; i < 2;i++)
    {
        D3D11_RESOURCE_FLAGS d3d11Flags = {};
        //D3D12�̃��\�[�X���AD3D11�������ł���悤�ɂ��邽�߂̂���(����̏ꍇ�̓o�b�N�o�b�t�@������D3D11_BIND_RENDER_TARGET���w��?)
        d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET;

        //12�̃o�b�N�o�b�t�@�����Ƃ�11�̃o�b�N�o�b�t�@�𐶐�?
        //�u���̃��\�b�h�́AD3D11on12�Ŏg�p����D3D11���\�[�X���쐬���܂��B�v�h�L�������g���
        result = d3d11On12device->CreateWrappedResource
        (
            pBuckBuffers[i],
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&wrappedBackBuffer[i])
        );

        //�u���̃o�b�N�o�b�t�@�ɒ��ڕ`�悷��D2D�̃����_�[�^�[�Q�b�g���쐬���܂��B�v�`���[�g���A���̕�
        //IDXGISurface�C���^�[�t�F�C�X�́A�摜�f�[�^�I�u�W�F�N�g�̃��\�b�h���������܂��B(�h�L�������g�̐�����|�󂵂���)
        ComPtr<IDXGISurface>dxgiSurface;
        wrappedBackBuffer[i].As(&dxgiSurface);
        d2dContext->CreateBitmapFromDxgiSurface
        (
            dxgiSurface.Get(),
            &bitmapProperties,
            &bitmap
        );

        backBufferHandle[i].Offset(1, pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
        //�A���P�[�^�[�ƃ����_�[�^�[�Q�b�g��DirectX12�N���X�̎g�����킩��񂩂�A��U�ۗ����Đ�ɐi�߂�
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
