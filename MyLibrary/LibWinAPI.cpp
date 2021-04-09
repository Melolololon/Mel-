#include "LibWinAPI.h"

//これメンバ変数にしないとHWNDがNULLになる
WNDCLASSEX LibWinAPI::w;

HWND LibWinAPI::createParentWindow
(
	const std::wstring& windowName,
	const int& windowWidth,
	const int& windowHeighr,
	const int& windowStyle,
	const WNDPROC& wndProc
)
{
	
	//WNDCLASSEX w;
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)wndProc;
	w.lpszClassName = windowName.c_str();
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);
	RECT wrc = { 0,0,windowWidth,windowHeighr };
	AdjustWindowRect(&wrc, windowStyle, false);

	HWND hwnd;
	hwnd = CreateWindow
	(
		w.lpszClassName,
		windowName.c_str(),
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}

HWND LibWinAPI::createChildWindow
(
	const std::wstring& windowName,
	const int& windowWidth,
	const int& windowHeighr,
	HWND& hwnd,
	const int& windowStyle,
	const WNDPROC& wndProc
)
{
	return nullptr;
}


bool LibWinAPI::createSaveWindow
(
	const HWND& hwnd, 
	std::wstring& fileName,
	const std::wstring & defaultFormat
)
{
	OPENFILENAMEW save;
	//初期状態のときの表示
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//ここにHWNDをセットするとダイアログのハンドルが格納される?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = nullptr;
	save.nMaxCustFilter = 0;
	save.nFilterIndex = 1;
	save.lpstrFile = (LPTSTR)fileName.data();//ここに入力したものが保存される
	save.nMaxFile = fileName.size();
	save.lpstrFileTitle = NULL;//NULLにすると、パスがC:から始まる
	save.nMaxFileTitle = 0;
	save.lpstrInitialDir = (LPCWSTR)L"C:";//ウィンドウ開いて最初に表示する場所?違うかも
	save.lpstrTitle = NULL;
	save.Flags = OFN_HIDEREADONLY;
	save.nFileOffset = 0;
	save.nFileExtension = 8;
	save.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//形式を入力しなかった場合、何の形式にするか
	//saveName.lCustData = ram;

	bool result = GetSaveFileName(&save);//ウィンドウハンドル取得してプロシージャでメッセージ取得して、破棄したらGamePlayのisSaveをfalseに変えるようにする

	if (!result)return result;
	int strCount = 0;
	for(auto& wStr : fileName)
	{
		if(wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}
	return result;

}

bool LibWinAPI::createLoadWindow
(
	const HWND& hwnd,
	std::wstring& fileName,
	const std::wstring& defaultFormat
)
{
	OPENFILENAMEW load;
	//初期状態のときの表示
	fileName.resize(400);

	load.lStructSize = sizeof(OPENFILENAME);
	load.hwndOwner = hwnd;//ここにHWNDをセットするとダイアログのハンドルが格納される?
	load.hInstance = GetModuleHandle(nullptr);
	load.lpstrFilter = nullptr;
	load.lpstrCustomFilter = nullptr;
	load.nMaxCustFilter = 0;
	load.nFilterIndex = 1;
	load.lpstrFile = (LPTSTR)fileName.data();//ここに入力したものが保存される
	load.nMaxFile = fileName.size();
	load.lpstrFileTitle = NULL;//NULLにすると、パスがC:から始まる
	load.nMaxFileTitle = 0;
	load.lpstrInitialDir = (LPCWSTR)L"C:";//ウィンドウ開いて最初に表示する場所?違うかも
	load.lpstrTitle = NULL;
	load.Flags = OFN_HIDEREADONLY;
	load.nFileOffset = 0;
	load.nFileExtension = 8;
	load.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//形式を入力しなかった場合、何の形式にするか
	//saveName.lCustData = ram;

	bool result = GetOpenFileName(&load);//ウィンドウハンドル取得してプロシージャでメッセージ取得して、破棄したらGamePlayのisSaveをfalseに変えるようにする

	if (!result)return result;
	int strCount = 0;
	for (auto& wStr : fileName)
	{
		if (wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}

	return result;
}
