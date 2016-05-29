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
	//�������� ������ ��������� ����������
	this->AddrInfoForPort(Port, &addrInfoList);
	//� ������������ � ���������� ������� ������� ���������� � �������
	//�� ������� ���������
	for (addrinfo* ptr = addrInfoList; ptr != NULL; ptr = ptr->ai_next)
	{
		//������� ����� ��� ���������� � ��������
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
			std::wcout << L"socket failed with error: " << WSAGetLastError() << "\n";

		//����������� � �������
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
		std::wcout << L"����������� � �������� �� �������!\n";
}

int ClientWinAPI::GetState()
{
	return ConnectSocket == INVALID_SOCKET ? 0 : 1;
}

ClientWinAPI::~ClientWinAPI()
{
	//�������� ���������� � ��������
	if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR)
		std::wcout << L"�������� ���������� �� �������. ��� ������: " << WSAGetLastError() << "\n";

	closesocket(ConnectSocket);
}
