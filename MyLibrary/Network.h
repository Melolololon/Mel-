#pragma once
#include<winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

namespace MelLib 
{
	//チュートリアル
	// https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

	// WinSock2を利用した通信クラス
	class Network
	{
	public:
		enum NetWorkError
		{

		};

	private:
		WSADATA wsaData = {};

		SOCKET serverPreAcceptSocket = {};
		SOCKET serverAcceptSocket = {};

		SOCKET clientPreAcceptSocket = {};
		SOCKET clientAcceptSocket = {};

	private:
		Network() {}
		~Network() {}
	public:
		Network(Network& n) = delete;
		Network operator=(Network& n) = delete;
		static Network* GetInstance();

		bool Initialize();
		void Finalize();

#pragma region 受信側(サーバー)
		void StartServer(const int portNum);
#pragma endregion

#pragma region 送信側

#pragma endregion


	};

}