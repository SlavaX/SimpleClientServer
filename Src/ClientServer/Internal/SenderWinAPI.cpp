#include "..\Public\SenderWinAPI.h"
//------------------------------
#include <iostream>
//------------------------------
SenderWinAPI::SenderWinAPI()
{
	this->GetConnectSocket(SENDER_PORT);
}

void SenderWinAPI::Send(const char *buf, int len)
{
	//�������� ����������� ������ �������
	if (send(ConnectSocket, buf, len, 0) == SOCKET_ERROR)
		std::wcout << L"�������� �� �������. ��� ������: " << WSAGetLastError() << "\n";
}

SenderWinAPI::~SenderWinAPI()
{
}
