#include "Network.h"
#include"ErrorProcess.h"

#include<Windows.h>

MelLib::Network* MelLib::Network::GetInstance()
{
	static Network n;
	return &n;
}

bool MelLib::Network::Initialize()
{
	int result = 0;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(result != 0)
	{
		ErrorProcess::GetInstance()->StartErroeProcess(L"Network�N���X�̏������Ɏ��s���܂����B", true);
#ifdef _DEBUG
		OutputDebugString(L"Network�N���X�̏������Ɏ��s���܂����B\n");
#endif // _DEBUG

		return false;
	}
}

void MelLib::Network::Finalize()
{
	WSACleanup();
}

MelLib::Network::NetWorkError MelLib::Network::StartServer(const int portNum)
{
	// IP�A�h���X��|�[�g�ԍ���ێ�����\����
	SOCKADDR_IN serverDDR = {};
	SOCKADDR_IN clientDDR = {};

	// ���X���\�P�b�g�I�[�v��
	serverPreAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverPreAcceptSocket == INVALID_SOCKET)
	{
#ifdef _DEBUG
		OutputDebugString(L"�\�P�b�g�̃I�[�v���Ɏ��s���܂����B\n");
#endif // _DEBUG
		return NetWorkError::FAILED_SOCKET_OPEN;
	}

	// �\�P�b�g�ɖ��O��t����
	memset(&serverDDR, 0, sizeof(serverDDR));
	serverDDR.sin_family = AF_INET;

}
