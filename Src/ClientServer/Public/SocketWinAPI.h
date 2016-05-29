#pragma once
//------------------------------
//�����������q ���� ��� ������ � ��������
#include <Winsock2.h>
//------------------------------
//������ ����� ������ ���������������� ������ � ���������
//� ��������� ��������� �
//------------------------------
#include "ClientServerExport.h"
//------------------------------
//����� ��� ����������
#define RECEIVER_PORT "11111"
#define SENDER_PORT "22222"
//------------------------------
class SOCKETDLL SocketWinAPI
{
protected:
	//��� ��������� ������ �������� ��� ������ ������� getaddrinfo
	void AddrInfoForPort(PCSTR Port, PADDRINFOA *Res);
private:
	//��� ���������� ������ ��������� � �������
	SocketWinAPI(const SocketWinAPI &);
public:
	//1 - ���������� �����������;
	//0 - ��������� ������ ��� �������� ������ 
	virtual int GetState() = 0;
	SocketWinAPI();
	virtual ~SocketWinAPI();
};
