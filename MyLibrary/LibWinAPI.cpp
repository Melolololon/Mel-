#include "LibWinAPI.h"

void LibWinAPI::createMainWindow(const std::wstring& windowName, const Vector2& size)
{

}


bool LibWinAPI::createSaveWindow
(
	const HWND& hwnd, 
	std::wstring& fileName,
	const std::wstring & defaultFormat
)
{
#pragma region ウィンドウ
	OPENFILENAMEW save;
	//初期状態のときの表示
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//ここにHWNDをセットするとダイアログのハンドルが格納される?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = (LPWSTR)L"マップチップデータ\0*.mc\0\0";
	save.nMaxCustFilter = sizeof("マップチップデータ\0*.mc\0\0");
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

	return GetSaveFileName(&save);//ウィンドウハンドル取得してプロシージャでメッセージ取得して、破棄したらGamePlayのisSaveをfalseに変えるようにする

#pragma endregion

}
