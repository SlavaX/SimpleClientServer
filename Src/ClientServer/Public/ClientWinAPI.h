#pragma once
//------------------------------
#include "..\Public\SocketWinAPI.h"
//------------------------------
class SOCKETDLL ClientWinAPI :
	public SocketWinAPI
{
protected:
	SOCKET ConnectSocket;
protected:
	//�������� ����� ��� ����������
	void GetConnectSocket(PCSTR Port);
public:
	virtual int GetState();
	ClientWinAPI();
	virtual ~ClientWinAPI();
};

