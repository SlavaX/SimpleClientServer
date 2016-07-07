#include "..\Public\SocketWinAPI.h"
//------------------------------
#include <iostream>
//------------------------------
//заголовочный файл для работы с сокетами
#include <ws2tcpip.h>
//------------------------------
//Подключение библиотеки которая будет обеспечивать работу с сокетами. 
//В msdn для работы с сокетами используется эта библитека
#pragma comment(lib, "ws2_32.lib")
//------------------------------
//Адресс для соединения
#define DEFAULT_ADDRESS "127.0.0.1"
//------------------------------
SocketWinAPI::SocketWinAPI()
{
	//Для корректного вывода русских букв в консоль
	std::wcout.imbue(std::locale("rus_rus.866"));
	std::wcin.imbue(std::locale("rus_rus.866"));
	//сначала необходимо инициализаровать библиотеку
	WSADATA wsaData = { 0 };//объект с информацией о реализации сокета
	//здесь происходит проверка на возможность создания
	if (int WSAStartupResult = //получаем код ошибки 
		WSAStartup(MAKEWORD(2, 2), //поскольку в msdn версия 2.2 является крайней пытаемся получить ее
		&wsaData))//структура в случае удачного выполнения должна быть заполнена
	{
		//В msdn написано буквально следующее
		//The WSAStartup function directly returns the extended error code
		//in the return value for this function.
		//A call to the WSAGetLastError function is not needed and should not be used.
		//Поскольку я пока не все понимаю в значениях этих ошибок, скопирую описание как есть из msdn
		switch (WSAStartupResult)
		{
		case WSASYSNOTREADY:
			std::wcout << L"The underlying network subsystem is not ready for network communication.\n";
			break;
		case WSAVERNOTSUPPORTED:
			std::wcout << L"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.\n";
			break;
		case WSAEINPROGRESS:
			std::wcout << L"A blocking Windows Sockets 1.1 operation is in progress.\n";
			break;
		case WSAEPROCLIM:
			std::wcout << L"A limit on the number of tasks supported by the Windows Sockets implementation has been reached.\n";
			break;
		case WSAEFAULT:
			std::wcout << L"The lpWSAData parameter is not a valid pointer.\n";
			break;
		default://Вероятнее всего никогда не выполнится.
			std::wcout << L"Какая то ошибка, не относящаяся к функции WSAStartup. Смотреть по коду ошибки.\n";
			break;
		}
		std::wcout << L"Код ошибки: " << WSAStartupResult << "\n";
	}
	else
	{
#ifdef _DEBUG
		std::wcout << L"Структура инициализирована:\n"
			<< L"wsaData.iMaxSockets:\t" << wsaData.iMaxSockets << "\n"
			<< L"wsaData.iMaxUdpDg:\t" << wsaData.iMaxUdpDg << "\n"
			<< L"wsaData.lpVendorInfo:\t" << static_cast<const void*>(wsaData.lpVendorInfo) << "\n"
			<< L"wsaData.szDescription:\t" << static_cast<const char*>(wsaData.szDescription) << "\n"
			<< L"wsaData.szSystemStatus:\t" << static_cast<const char*>(wsaData.szSystemStatus) << "\n"
			<< L"wsaData.wHighVersion:\t" << wsaData.wHighVersion << "\n"
			<< L"wsaData.wVersion:\t" << wsaData.wVersion << "\n";
#endif
	}
}

void SocketWinAPI::AddrInfoForPort(PCSTR Port, PADDRINFOA *Res)
{
	//Если структура инициализировалась
	//Назначить адрес и порт
	addrinfo hints = { 0 };
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (int getaddrinfoResult = getaddrinfo(DEFAULT_ADDRESS, Port, &hints, Res))
	{
		switch (getaddrinfoResult)
		{
		case EAI_AGAIN:
			std::wcout << L"A temporary failure in name resolution occurred.\n";
			break;
		case EAI_BADFLAGS:
			std::wcout << L"An invalid value was provided for the ai_flags member of the pHints parameter.\n";
			break;
		case EAI_FAIL:
			std::wcout << L"A nonrecoverable failure in name resolution occurred.\n";
			break;
		case EAI_FAMILY:
			std::wcout << L"The ai_family member of the pHints parameter is not supported.\n";
			break;
		case EAI_MEMORY:
			std::wcout << L"A memory allocation failure occurred.\n";
			break;
		case EAI_NONAME:
			std::wcout << L"The name does not resolve for the supplied parameters or the pNodeName and pServiceName parameters were not provided.\n";
			break;
		case EAI_SERVICE:
			std::wcout << L"The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter.\n";
			break;
		case EAI_SOCKTYPE:
			std::wcout << L"The ai_socktype member of the pHints parameter is not supported.\n";
			break;
		default://Вероятнее всего никогда не выполнится.
			std::wcout << L"Какая то ошибка, не относящаяся к функции WSAStartup. Смотреть по коду ошибки.\n";
			break;
		}
		std::wcout << L"Код ошибки: " << getaddrinfoResult << "\n";
	}
}

SocketWinAPI::~SocketWinAPI()
{
	WSACleanup();
}
