#pragma once
//------------------------------
//заголовочныq файл для работы с сокетами
#include <Winsock2.h>
//------------------------------
//Задача этого класса инициализировать работу с сокетсами
//и корректно завершить её
//------------------------------
#include "ClientServerExport.h"
//------------------------------
//Порты для соединения
#define RECEIVER_PORT "11111"
#define SENDER_PORT "22222"
//------------------------------
class SOCKETDLL SocketWinAPI
{
protected:
	//Для получения списка структур при помощи функции getaddrinfo
	void AddrInfoForPort(PCSTR Port, PADDRINFOA *Res);
private:
	//Для исключения ошибок перенесен в приваты
	SocketWinAPI(const SocketWinAPI &);
public:
	//1 - соединение установлено;
	//0 - произошла ошибка при создании сокета 
	virtual int GetState() = 0;
	SocketWinAPI();
	virtual ~SocketWinAPI();
};
