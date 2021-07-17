#include "Library.h"
#include"LibMath.h"

#include"Input.h"

#pragma comment(lib,"winmm.lib")//timeGetTimeと
#include"ObjectManager.h"
#include"TimerManager.h"
#include"SceneManager.h"

#include"Audio.h"
#include"Sound.h"

#include"Random.h"
#include"LibWinAPI.h"


DirectX12* Library::dx12;
CreatePolygon* Library::createPolygon;

WNDCLASSEX Library::w;
MSG  Library::msg;
HWND  Library::hwnd;
bool Library::isEnd;

bool Library::isDestroy;
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


	auto coResult = CoInitialize(NULL);

	dx12 = DirectX12::GetInstance();
	createPolygon = CreatePolygon::GetInstance();
	createPolygon->Initialize(windowWidth, windowHeight);

	srand((unsigned int)time(NULL));
	Random::Initialize();
	ObjectManager::GetInstance()->Initialize();

	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region ウィンドウ処理

	hwnd = LibWinAPI::CreateNormalWindow
	(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW /*| WS_CLIPCHILDREN*/,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		WindowProc
	);


	/*w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = windowName;
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);
	RECT wrc = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		w.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);*/



#pragma endregion

#pragma region DC

	//DCを取得
	hdc = GetDC(hwnd);
	//リフレッシュレートを取得
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

#pragma region Input
	Input::Initialize(hwnd, windowWidth, windowHeight);
#pragma endregion

	Audio::GetInstance()->Initialize();
	

	dx12->SetScreenColor(screenColor);
	dx12->Initialize(hwnd, windowWidth, windowHeight);
	

	createPipelineCounter = dx12->getStartPipelineCreateNum();
	//スプライトフォント300を作るのでカウントを増やしておく
	createSpriteConter = 300;

	//べた塗用テクスチャを0にするために1にする(DrawBoxなどに使う)
	loadTextureCounter = 1;

	//modelDatas.reserve(99999);
}

void Library::LoopStartProcess()
{

#pragma region ウィンドウ処理

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		isEnd = true;
		isDestroy = true;
	}

#pragma endregion

	startProsessTime = timeGetTime();

	dx12->LoopStartProcess();

	Input::Update();
	Sound::Update();
}

void Library::LoopEndProcess()
{
	//ソート
	//dx12->sortModelData(modelDatas);

	//コマンドセット
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

		//60fpsの時の1フレームの処理時間を計測(この数値は、環境に依存しない)
		//これ毎回計測する必要ない
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//待機しないといけない時間
		count++;
		count = count >= fps ? 0 : count;

		//前ループからどのくらい時間が経過したかを計測(preを、startProsessのほうに移し、GetTickCount64入れる)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//待機時間を計測(taikiTimeとsabunの差だけsleeで待機)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//スリープ
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	TimerManager::GetInstance()->Update();


}

void Library::EndFlagTrue()
{
	isDestroy = false;
	isEnd = true;
}

bool Library::GetIsEnd()
{
	return isEnd;
}

void Library::Finalize()
{
	SceneManager::GetInstace()->Finitialize();
	ObjectManager::GetInstance()->Finitialize();

	Input::Finitialize();

	dx12->Finalize();

	if (!isDestroy)
	{
		DestroyWindow(hwnd);
	}

	UnregisterClass(w.lpszClassName, w.hInstance);

	CoUninitialize();
}


#pragma region WinAPI関係
HWND Library::GetHWND()
{
	return hwnd;
}
#pragma endregion


#pragma region 設定

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


