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
#pragma region �E�B���h�E
	OPENFILENAMEW save;
	//������Ԃ̂Ƃ��̕\��
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//������HWND���Z�b�g����ƃ_�C�A���O�̃n���h�����i�[�����?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = (LPWSTR)L"�}�b�v�`�b�v�f�[�^\0*.mc\0\0";
	save.nMaxCustFilter = sizeof("�}�b�v�`�b�v�f�[�^\0*.mc\0\0");
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

	return GetSaveFileName(&save);//�E�B���h�E�n���h���擾���ăv���V�[�W���Ń��b�Z�[�W�擾���āA�j��������GamePlay��isSave��false�ɕς���悤�ɂ���

#pragma endregion

}
