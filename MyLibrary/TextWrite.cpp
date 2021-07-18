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
}
