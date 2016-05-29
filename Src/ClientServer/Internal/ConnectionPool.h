#pragma once
//------------------------------
#include <thread>
#include <vector>
#include <deque>
#include <condition_variable>
//------------------------------
//������ ����� �������� �� ������ ���� �������
//������ ���������� � ����� ������
//------------------------------
class ConnectionPool
{
friend class SocketTask;
public:
	ConnectionPool(size_t HandlersQuantity);
	virtual ~ConnectionPool();
	//�������� ������ � �������
	void AddTask(std::function<void()> f);
private:
	//������ ��� ������ � ������������
	std::vector< std::thread > Handlers;
	//������� ���������� ��� ���������
	std::deque< std::function<void()> > Connections;
	//������� ����, ��� ������ ����������
	//� ������ �� ������� �������� ������ �� ����
	bool Stop;
	std::mutex Queue_mut;
	std::condition_variable Condition;
};

