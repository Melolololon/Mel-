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
	/// �e�E�B���h�E�𐶐����܂��B
	/// </summary>
	/// <param name="windowName">�E�B���h�E��</param>
	/// <param name="windowWidth">����</param>
	/// <param name="windowHeighr">�c��</param>
	/// <param name="windowStyle">�E�B���h�E�X�^�C��</param>
	/// <param name="wndProc">�E�B���h�E�v���V�[�W��</param>
	/// <returns>�E�B���h�E�n���h��</returns>
	static HWND createParentWindow
	(
		const std::wstring& windowName, 
		const int& windowWidth,
		const int& windowHeighr,
		const int& windowStyle,
		const WNDPROC& wndProc
	);

	/// <summary>
	/// �q�E�B���h�E���쐬���܂�
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
	/// �ۑ����邽�߂̃E�B���h�E�𐶐����A���͂����t�@�C�������擾���܂�
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="fileName">�t�@�C��������͂��镔���̏��������L�q����Btrue���Ԃ��ꂽ�ꍇ�A���͂����t�@�C�������i�[�����B</param>
	/// <param name="defaultFormat">�t�H�[�}�b�g���w�肵�Ȃ������ꍇ�̃t�H�[�}�b�g��</param>
	/// <returns></returns>
	static bool createSaveWindow
	(
		const HWND& hwnd, 
		std::wstring& fileName,
		const std::wstring & defaultFormat
	);

	/// <summary>
	///		�J�����߂̃E�B���h�E�𐶐����A���͂����t�@�C�������擾���܂�
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="fileName">�t�@�C��������͂��镔���̏��������L�q����Btrue���Ԃ��ꂽ�ꍇ�A���͂����t�@�C�������i�[�����B</param>
	/// <param name="defaultFormat">�t�H�[�}�b�g���w�肵�Ȃ������ꍇ�̃t�H�[�}�b�g��</param>
	/// <returns></returns>
	static bool createLoadWindow
	(
		const HWND& hwnd,
		std::wstring& fileName,
		const std::wstring& defaultFormat
	);
	
};

