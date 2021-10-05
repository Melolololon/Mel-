#pragma once
#include<winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

namespace MelLib 
{
	//�`���[�g���A��
	// https://docs.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

	// WinSock2�𗘗p�����ʐM�N���X
	class Network
	{
	public:
		enum NetWorkError
		{
			ERROR_NONE,
			FAILED_SOCKET_OPEN,

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

#pragma region ��M��(�T�[�o�[)
		NetWorkError StartServer(const int portNum);
#pragma endregion

#pragma region ���M��

#pragma endregion


	};

}