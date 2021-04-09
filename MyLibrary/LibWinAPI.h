#pragma once
#include<Windows.h>
#include<string>
#include"Vector.h"
class LibWinAPI
{
private:
	static WNDCLASSEX w;

public:


	/// <summary>
	/// 親ウィンドウを生成します。
	/// </summary>
	/// <param name="windowName">ウィンドウ名</param>
	/// <param name="windowWidth">横幅</param>
	/// <param name="windowHeighr">縦幅</param>
	/// <param name="windowStyle">ウィンドウスタイル</param>
	/// <param name="wndProc">ウィンドウプロシージャ</param>
	/// <returns>ウィンドウハンドル</returns>
	static HWND createParentWindow
	(
		const std::wstring& windowName, 
		const int& windowWidth,
		const int& windowHeighr,
		const int& windowStyle,
		const WNDPROC& wndProc
	);

	/// <summary>
	/// 子ウィンドウを作成します
	/// </summary>
	/// <param name="windowName"></param>
	/// <param name="windowWidth"></param>
	/// <param name="windowHeighr"></param>
	/// <param name="hwnd"></param>
	/// <param name="windowStyle"></param>
	/// <param name="wndProc"></param>
	/// <returns></returns>
	static HWND createChildWindow
	(
		const std::wstring& windowName,
		const int& windowWidth,
		const int& windowHeighr,
		HWND& hwnd,
		const int& windowStyle,
		const WNDPROC& wndProc
	);

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

	/// <summary>
	///		開くためのウィンドウを生成し、入力したファイル名を取得します
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="fileName">ファイル名を入力する部分の初期名を記述する。trueが返された場合、入力したファイル名が格納される。</param>
	/// <param name="defaultFormat">フォーマットを指定しなかった場合のフォーマット名</param>
	/// <returns></returns>
	static bool createLoadWindow
	(
		const HWND& hwnd,
		std::wstring& fileName,
		const std::wstring& defaultFormat
	);
	
};

