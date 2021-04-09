#include "LibWinAPI.h"

//���ꃁ���o�ϐ��ɂ��Ȃ���HWND��NULL�ɂȂ�
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
	//������Ԃ̂Ƃ��̕\��
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//������HWND���Z�b�g����ƃ_�C�A���O�̃n���h�����i�[�����?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = nullptr;
	save.nMaxCustFilter = 0;
	save.nFilterIndex = 1;
	save.lpstrFile = (LPTSTR)fileName.data();//�����ɓ��͂������̂��ۑ������
	save.nMaxFile = fileName.size();
	save.lpstrFileTitle = NULL;//NULL�ɂ���ƁA�p�X��C:����n�܂�
	save.nMaxFileTitle = 0;
	save.lpstrInitialDir = (LPCWSTR)L"C:";//�E�B���h�E�J���čŏ��ɕ\������ꏊ?�Ⴄ����
	save.lpstrTitle = NULL;
	save.Flags = OFN_HIDEREADONLY;
	save.nFileOffset = 0;
	save.nFileExtension = 8;
	save.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//�`������͂��Ȃ������ꍇ�A���̌`���ɂ��邩
	//saveName.lCustData = ram;

	bool result = GetSaveFileName(&save);//�E�B���h�E�n���h���擾���ăv���V�[�W���Ń��b�Z�[�W�擾���āA�j��������GamePlay��isSave��false�ɕς���悤�ɂ���

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
	//������Ԃ̂Ƃ��̕\��
	fileName.resize(400);

	load.lStructSize = sizeof(OPENFILENAME);
	load.hwndOwner = hwnd;//������HWND���Z�b�g����ƃ_�C�A���O�̃n���h�����i�[�����?
	load.hInstance = GetModuleHandle(nullptr);
	load.lpstrFilter = nullptr;
	load.lpstrCustomFilter = nullptr;
	load.nMaxCustFilter = 0;
	load.nFilterIndex = 1;
	load.lpstrFile = (LPTSTR)fileName.data();//�����ɓ��͂������̂��ۑ������
	load.nMaxFile = fileName.size();
	load.lpstrFileTitle = NULL;//NULL�ɂ���ƁA�p�X��C:����n�܂�
	load.nMaxFileTitle = 0;
	load.lpstrInitialDir = (LPCWSTR)L"C:";//�E�B���h�E�J���čŏ��ɕ\������ꏊ?�Ⴄ����
	load.lpstrTitle = NULL;
	load.Flags = OFN_HIDEREADONLY;
	load.nFileOffset = 0;
	load.nFileExtension = 8;
	load.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//�`������͂��Ȃ������ꍇ�A���̌`���ɂ��邩
	//saveName.lCustData = ram;

	bool result = GetOpenFileName(&load);//�E�B���h�E�n���h���擾���ăv���V�[�W���Ń��b�Z�[�W�擾���āA�j��������GamePlay��isSave��false�ɕς���悤�ɂ���

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
