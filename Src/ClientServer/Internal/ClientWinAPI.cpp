//------------------------------
#include "..\Public\ClientWinAPI.h"
//------------------------------
#include <iostream>
#include <ws2tcpip.h>
//------------------------------
ClientWinAPI::ClientWinAPI() :
	ConnectSocket(INVALID_SOCKET)
{
}

void ClientWinAPI::GetConnectSocket(PCSTR Port)
{
	addrinfo *addrInfoList = 0;
	//Получить список возможных соединений
	this->AddrInfoForPort(Port, &addrInfoList);
	//В соответствии с полученным списком пробуем соединитья к серверу
	//до первого успешного
	for (addrinfo* ptr = addrInfoList; ptr != NULL; ptr = ptr->ai_next)
	{
		//Создать сокет для соединения с сервером
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
			std::wcout << L"socket failed with error: " << WSAGetLastError() << "\n";

		//Соединиться к серверу
		int connectResult = connect(ConnectSocket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
		if (connectResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(addrInfoList);

	if (ConnectSocket == INVALID_SOCKET)
		std::wcout << L"Соединиться с сервером не удалось!\n";
}

int ClientWinAPI::GetState()
{
	return ConnectSocket == INVALID_SOCKET ? 0 : 1;
}

ClientWinAPI::~ClientWinAPI()
{
	//Закрытие соединения с сервером
	if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR)
		std::wcout << L"Закрытие соединения не удалось. Код ошибки: " << WSAGetLastError() << "\n";

	closesocket(ConnectSocket);
}
