#include "Library.h"
#include"LibMath.h"

#include"Input.h"

#pragma comment(lib,"winmm.lib")//timeGetTime��
#include"GameObjectManager.h"
#include"SceneManager.h"

#include"Audio.h"
#include"Sound.h"

#include"Random.h"
#include"FrameTimer.h"

#include"LibWinAPI.h"
#include"TextWrite.h"


#include"SpriteFont2D.h"


DirectX12* Library::dx12;
CreatePolygon* Library::createPolygon;

WNDCLASSEX Library::w;
MSG  Library::msg;
HWND Library::hwnd;
bool Library::isEnd;

UINT Library::count;

int Library::createPointCount;

bool Library::isSetFPS60;
UINT Library::fps;

int Library::loadFontTextureCounter;
int Library::loadTextureCounter;
int Library::createSpriteConter;

int Library::createPipelineCounter;
int Library::createPostEffectPuiperineCounter;

HDC Library::hdc;
int Library::refReat;
DWORD Library::startProsessTime;
DWORD Library::nowTime;

std::vector<std::tuple<ModelDataAndKey, int>> Library::modelDatas;

int Library::winWidth;
int Library::winHeight;
Color Library::clearColor;
std::wstring Library::winName;

LRESULT Library::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Library::Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* windowName)
{
	 winWidth = windowWidth;
	 winHeight = windowHeight;
	 clearColor = screenColor;
 	 winName = windowName;

	auto coResult = CoInitialize(NULL);

	dx12 = DirectX12::GetInstance();
	createPolygon = CreatePolygon::GetInstance();
	createPolygon->Initialize(windowWidth, windowHeight);

	srand((unsigned int)time(NULL));
	Random::Initialize();
	GameObjectManager::GetInstance()->Initialize();

	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region �E�B���h�E����



	hwnd = LibWinAPI::CreateNormalWindow
	(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW /*| WS_MAXIMIZE *//*| WS_CLIPCHILDREN*/,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		WindowProc
	);


	

#pragma endregion

#pragma region DC

	//DC���擾
	hdc = GetDC(hwnd);
	//���t���b�V�����[�g���擾
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

	

	dx12->SetScreenColor(screenColor);
	dx12->Initialize(hwnd, windowWidth, windowHeight);
	

	createPipelineCounter = dx12->getStartPipelineCreateNum();
	//�X�v���C�g�t�H���g300�����̂ŃJ�E���g�𑝂₵�Ă���
	createSpriteConter = 300;

	//�ׂ��h�p�e�N�X�`����0�ɂ��邽�߂�1�ɂ���(DrawBox�ȂǂɎg��)
	loadTextureCounter = 1;

	//modelDatas.reserve(99999);



	Input::Initialize(hwnd, windowWidth, windowHeight);
	Audio::GetInstance()->Initialize();
	SpriteFont2D::GetInstance()->Initialize();
}

void Library::LoopStartProcess()
{

#pragma region �E�B���h�E����

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		isEnd = true;
	}

#pragma endregion

	startProsessTime = timeGetTime();

	dx12->LoopStartProcess();

	Input::Update();
	Sound::Update();
	SpriteFont2D::GetInstance()->Update();
}

void Library::LoopEndProcess()
{
	//�\�[�g
	//dx12->sortModelData(modelDatas);

	//�R�}���h�Z�b�g
	/*ModelData mData;
	int number;
	for(const auto& m : modelDatas)
	{
		mData = std::get<0>(m);
		number = std::get<1>(m);
		dx12->map(mData, number);
		dx12->setCmdList(mData.key, number);
	}*/

	//modelDatas.clear();

	dx12->LoopEndProcess();
	
	if (isSetFPS60)
	{

		//60fps�̎���1�t���[���̏������Ԃ��v��(���̐��l�́A���Ɉˑ����Ȃ�)
		//���ꖈ��v������K�v�Ȃ��Bfps�ς������ɕύX����΂���
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//�ҋ@���Ȃ��Ƃ����Ȃ�����
		count++;
		count = count >= fps ? 0 : count;

		//�O���[�v����ǂ̂��炢���Ԃ��o�߂��������v��(pre���AstartProsess�̂ق��Ɉڂ��AGetTickCount64�����)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//�ҋ@���Ԃ��v��(taikiTime��sabun�̍�����slee�őҋ@)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//�X���[�v
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	FrameTimer::AllUpdate();


}

void Library::EndFlagTrue()
{
	isEnd = true;
}

bool Library::GetIsEnd()
{
	return isEnd;
}

void Library::Finalize()
{
	SceneManager::GetInstace()->Finitialize();
	GameObjectManager::GetInstance()->Finitialize();

	Input::Finitialize();

	Sound::Finitialize();
	SoundData::Finitialize();

	dx12->Finalize();

	if(hwnd) DestroyWindow(hwnd);

	UnregisterClass(w.lpszClassName, w.hInstance);

	CoUninitialize();
}


#pragma region WinAPI�֌W
HWND Library::GetHWND()
{
	return hwnd;
}
#pragma endregion


#pragma region �ݒ�

void Library::SetFramesPerSecond60(bool flag)
{
	isSetFPS60 = flag;
}
//
//void Library::setScreenColor(Color color)
//{
//	dx12->setScreenColor(color);
//}

#pragma endregion


