//------------------------------
#include "..\Public\ServerWinAPI.h"
//------------------------------
#include <iostream>
#include <thread>
#include <Ws2tcpip.h>
//------------------------------
#include "ConnectionPool.h"
//------------------------------
ServerWinAPI::ServerWinAPI() :
ReceiversListenSocket(INVALID_SOCKET),
SendersListenSocket(INVALID_SOCKET)
{
	//Здесь необходимо создать сокеты для прослушивания передатчиков и приемников
	ReceiversListenSocket = this->GetListenSocket(RECEIVER_PORT);
	SendersListenSocket = this->GetListenSocket(SENDER_PORT);
}

SOCKET ServerWinAPI::GetListenSocket(PCSTR Port)
{
	addrinfo *addrInfoList = 0;
	//Получить список адресов для соединения
	this->AddrInfoForPort(Port, &addrInfoList);

	//Создать сокет для прослушивания
	SOCKET ListenSocket = socket(addrInfoList->ai_family, addrInfoList->ai_socktype, addrInfoList->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::wcout << L"Сокет не создан. Код ошибки: " << WSAGetLastError() << "\n";
	}
	else
	{
		//Связать с адресом сокет для прослушивания
		if (bind(ListenSocket, addrInfoList->ai_addr, static_cast<int>(addrInfoList->ai_addrlen)) == SOCKET_ERROR)
		{
			std::wcout << L"Связь сокета с адресом не удалось. Код ошибки: " << WSAGetLastError() << "\n";
		}
		else
		{
			//Начать прослушивание порта
			if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
				std::wcout << L"Прослушивание не удалось. Код ошибки: " << WSAGetLastError() << "\n";
		}

	}
	freeaddrinfo(addrInfoList);
	return ListenSocket;
}

int ServerWinAPI::GetState()
{
	return	ReceiversListenSocket == INVALID_SOCKET &&
			SendersListenSocket == INVALID_SOCKET ?
			0 : 1;
}

void ServerWinAPI::Accept(SOCKET &ListenSocket)
{
	for (;;)
	{
		SOCKET ConnectSocket = INVALID_SOCKET;
		ConnectSocket = accept(ListenSocket, NULL, NULL);
		if (ConnectSocket == INVALID_SOCKET)
		{
			std::wcout << L"Прием не состоялся. Код ошибки: " << WSAGetLastError() << "\n";
		}
		else
		{
			{
				std::unique_lock<std::mutex> lock(Queue_mutex);
				if (ListenSocket == SendersListenSocket)
					this->SendersQueue.push_back(ConnectSocket);
				if (ListenSocket == ReceiversListenSocket)
					this->ReceiversQueue.push_back(ConnectSocket);
			}
			this->SendersCondition.notify_all();
			this->ReceiversCondition.notify_all();
		}
	}
}


class ReceiveTransferTask
{
private:
	SOCKET Socket;
	ServerWinAPI *Server;
public:
	ReceiveTransferTask(SOCKET NewSocket, ServerWinAPI *Server) : Socket(NewSocket), Server(Server){}
	virtual ~ReceiveTransferTask(){}
	void operator()()
	{
		int recvResult = 0;
		std::string tmp;//будем использовать при парсинге входного буфера (конец буфера не означает конец строки)
		do {
			char buf[BUFFER_SIZE] = { 0 };
			int len = sizeof(buf);
			recvResult = recv(Socket, buf, len, 0);
			if (recvResult > 0)
			{
				//Поскольку не определено, что такое сообщение
				//будем считать что сообщение это то, что заканчивается '\0'
				std::vector<std::string> BufVect;
				for (auto &ch : buf)
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
				std::wcout << L"Количество байт: " << recvResult << "\n";
				std::wcout << L"Исходный буфер: " << buf << "\n";//на нуле строка обрывается и не выводится
				std::wcout << L"Парсенный буфер: " << "\n";
				for (auto &str : BufVect)
					std::wcout << str.c_str() << "\n";
#endif
				{
					std::unique_lock<std::mutex> lock(Server->Messages_mutex);
					for (auto it : BufVect)
					{
						Server->Messages.push_back(it);
					}
#ifdef _DEBUG
					//for (auto &str : Server->Messages)
					//	std::wcout << str.c_str() << "\n";
#endif
				}
				Server->MessagesCondition.notify_all();
			}
			else if (recvResult == 0)
			{
				std::wcout << L"Прием закончен\n";
				if (shutdown(Socket, SD_SEND) == SOCKET_ERROR)
				{
					std::wcout << L"Отключение не удалось. Код ошибки: " << WSAGetLastError() << "\n";
					closesocket(Socket);
				}
			}
			else 
			{
				std::wcout << L"Функчия recv выполнилась с ошибкой: " << WSAGetLastError() << "\n";
				closesocket(Socket);
			}
		} while (recvResult > 0);
	}
};

void ServerWinAPI::SendersTransfer()
{
	ConnectionPool SendersPool(SENDERS_QUANTITY);
	for (;;)
	{
		SOCKET NewSocket = INVALID_SOCKET;
		{
			std::unique_lock<std::mutex> lock(Queue_mutex);

			while (this->SendersQueue.empty())
			{
				this->SendersCondition.wait(lock);
			}

			NewSocket = this->SendersQueue.front();
			this->SendersQueue.pop_back();
		}
		SendersPool.AddTask(ReceiveTransferTask(NewSocket, this));
	}
}

class SendTransferTask
{
private:
	SOCKET Socket;
	ServerWinAPI *Server;
public:
	SendTransferTask(SOCKET NewSocket, ServerWinAPI *Server) : Socket(NewSocket), Server(Server){}
	virtual ~SendTransferTask(){}
	void operator()()
	{
		for (int index = 0;; ++index)
		{
			
			std::string TransferStr;
			{
				std::unique_lock<std::mutex> lock(Server->Messages_mutex);
				while (Server->Messages.size() == index)
				{
					this->Server->MessagesCondition.wait(lock);
				}
				TransferStr = Server->Messages[index];
			}
			if (send(Socket, TransferStr.c_str(), TransferStr.size() + 1, 0) == SOCKET_ERROR)
			{
				//TODO При отключении приемника никто об этом не знает. Придумать как разбудить поток в этом случае.
				//Надо чтобы он прервал соединение и не занимал поток из пула.
				//Пока он просыпается только при поступлении нового сообщения.
				std::wcout << L"Функция send выполнилась с ошибкой: " << WSAGetLastError() << "\n";
				closesocket(Socket);
				break;
			}
		}

	}
};

void ServerWinAPI::ReceiversTransfer()
{
	ConnectionPool ReceiversPool(RECEIVERS_QUANTITY);
	for (;;)
	{
		SOCKET NewSocket = INVALID_SOCKET;
		{
			std::unique_lock<std::mutex> lock(Queue_mutex);

			while (this->ReceiversQueue.empty())
			{
				this->ReceiversCondition.wait(lock);
			}

			NewSocket = this->ReceiversQueue.front();
			this->ReceiversQueue.pop_back();
		}
		ReceiversPool.AddTask(SendTransferTask(NewSocket, this));
	}
}

void ServerWinAPI::Start()
{
	//Здесь мы создадим две пары потока один для отправителя другой для получателя
	//Будем пользоваться отдельными портами и потоками, поскольку в противном случае
	//трудно разлечить кто кем является из клиентов
	std::thread SendersAcceptonThread(&ServerWinAPI::Accept, this, std::ref(this->SendersListenSocket));
	std::thread RecieversAcceptonThread(&ServerWinAPI::Accept, this, std::ref(this->ReceiversListenSocket));
	std::thread SendersTransferThread(&ServerWinAPI::SendersTransfer, this);
	std::thread ReceiversTransferThread(&ServerWinAPI::ReceiversTransfer, this);

	if (SendersAcceptonThread.joinable())
		SendersAcceptonThread.join();
	if (RecieversAcceptonThread.joinable())
		RecieversAcceptonThread.join();
	if (SendersTransferThread.joinable())
		SendersTransferThread.join();
	if (ReceiversTransferThread.joinable())
		ReceiversTransferThread.join();
}

ServerWinAPI::~ServerWinAPI()
{
	closesocket(ReceiversListenSocket);
	closesocket(SendersListenSocket);
}
