//------------------------------
#pragma once
//------------------------------
#include <deque>
#include <mutex>
#include <condition_variable>
//------------------------------
#include "..\Public\SocketWinAPI.h"
//------------------------------
#define SENDERS_QUANTITY 100
#define RECEIVERS_QUANTITY 100
#define BUFFER_SIZE 1024 
//------------------------------
class SOCKETDLL ServerWinAPI :
	public SocketWinAPI
{
friend class ReceiveTransferTask;
friend class SendTransferTask;
private:
	//����� ��� ��������� ������������
	SOCKET ReceiversListenSocket;
	//����� ��� ��������� ����������
	SOCKET SendersListenSocket;
	//������� ������� ��� ������������
	std::deque<SOCKET> SendersQueue;
	std::condition_variable SendersCondition;
	//������� ������� ��� ������������
	std::deque<SOCKET> ReceiversQueue;
	std::condition_variable ReceiversCondition;
	std::mutex Queue_mutex;
	//������� ��� ���������
	std::deque<std::string> Messages;
	std::condition_variable MessagesCondition;
	std::mutex Messages_mutex;

private:
	//�������� ����� ��� �������������
	SOCKET GetListenSocket(PCSTR Port);
	void Accept(SOCKET &ConnectSocket);
	void SendersTransfer();
	void ReceiversTransfer();
public:
	virtual int GetState();
	void Start();
	ServerWinAPI();
	virtual ~ServerWinAPI();
};

