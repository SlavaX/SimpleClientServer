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
	//Функция ничего не делает просто выводит принятые сообщения на консоль
	//char *buf, int len не используются
	int recvResult = 0;
	std::string tmp;
	do {
		char bufLocal[512] = { 0 };
		int lenLocal = sizeof(bufLocal);
		recvResult = recv(ConnectSocket, bufLocal, lenLocal, 0);
		if (recvResult > 0)
		{
			//Сообщение это то, что заканчивается '\0'
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
			std::wcout << L"Принято байт: " << recvResult << "\n";
#endif
			for (auto &str : BufVect)
				std::wcout << str.c_str() << "\n";
		}
		else if (recvResult == 0)
		{
			if (tmp.size())//вывести остаток
			{
				std::wcout << tmp.c_str() << "\n";
				tmp.clear();
			}
			std::wcout << L"Соединение прервано\n";
		}
		else
			std::wcout << L"Функция recv выполнилась с ошибкой: " << WSAGetLastError() << "\n";

	} while (recvResult > 0);
}

ReceiverWinAPI::~ReceiverWinAPI()
{
}
