#pragma once
#include<Windows.h>
#include<string>
#include"Vector.h"
class LibWinAPI
{
public:

	static void createMainWindow(const std::wstring& windowName, const Vector2& size);

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
	
};

