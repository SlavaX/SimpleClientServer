//------------------------------
#include "..\Public\ReceiverWinAPI.h"
//------------------------------
#include <iostream>
#include <vector>
//------------------------------
ReceiverWinAPI::ReceiverWinAPI()
{
	this->GetConnectSocket(RECEIVER_PORT);
}

void ReceiverWinAPI::Receive(char *buf, int len)
{
	//������� ������ �� ������ ������ ������� �������� ��������� �� �������
	//char *buf, int len �� ������������
	int recvResult = 0;
	std::string tmp;
	do {
		char bufLocal[512] = { 0 };
		int lenLocal = sizeof(bufLocal);
		recvResult = recv(ConnectSocket, bufLocal, lenLocal, 0);
		if (recvResult > 0)
		{
			//��������� ��� ��, ��� ������������� '\0'
			std::vector<std::string> BufVect;
			for (auto &ch : bufLocal)
			{
				if (ch)
				{
					tmp.push_back(ch);
				}
				else
				{
					if (tmp.size())
					{
						BufVect.push_back(tmp);
						tmp.clear();
					}
				}
			}
#ifdef _DEBUG
			std::wcout << L"������� ����: " << recvResult << "\n";
#endif
			for (auto &str : BufVect)
				std::wcout << str.c_str() << "\n";
		}
		else if (recvResult == 0)
		{
			if (tmp.size())//������� �������
			{
				std::wcout << tmp.c_str() << "\n";
				tmp.clear();
			}
			std::wcout << L"���������� ��������\n";
		}
		else
			std::wcout << L"������� recv ����������� � �������: " << WSAGetLastError() << "\n";

	} while (recvResult > 0);
}

ReceiverWinAPI::~ReceiverWinAPI()
{
}
