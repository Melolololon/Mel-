#pragma once
#include<Windows.h>
#include<string>
#include"Vector.h"
class LibWinAPI
{
public:

	static void createMainWindow(const std::wstring& windowName, const Vector2& size);

	/// <summary>
	/// 保存するためのウィンドウを生成し、入力したファイル名を取得します
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="fileName">ファイル名を入力する部分の初期名を記述する。trueが返された場合、入力したファイル名が格納される。</param>
	/// <param name="defaultFormat">フォーマットを指定しなかった場合のフォーマット名</param>
	/// <returns></returns>
	static bool createSaveWindow
	(
		const HWND& hwnd, 
		std::wstring& fileName,
		const std::wstring & defaultFormat
	);
	
};

