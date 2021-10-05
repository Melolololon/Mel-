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
		ErrorProcess::GetInstance()->StartErroeProcess(L"Networkクラスの初期化に失敗しました。", true);
#ifdef _DEBUG
		OutputDebugString(L"Networkクラスの初期化に失敗しました。\n");
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
	// IPアドレスやポート番号を保持する構造体
	SOCKADDR_IN serverDDR = {};
	SOCKADDR_IN clientDDR = {};

	// リスンソケットオープン
	serverPreAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverPreAcceptSocket == INVALID_SOCKET)
	{
#ifdef _DEBUG
		OutputDebugString(L"ソケットのオープンに失敗しました。\n");
#endif // _DEBUG
		return NetWorkError::FAILED_SOCKET_OPEN;
	}

	// ソケットに名前を付ける
	memset(&serverDDR, 0, sizeof(serverDDR));
	serverDDR.sin_family = AF_INET;

}
