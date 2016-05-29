#pragma once
class ConnectionPool;
//���� ������ �������� ������ ������ ConnectionPool
class SocketTask
{
private:
	//������ ��� ����, ����� ���� �����������
	//���������� ��������� ������� ����� ��� ����������
	ConnectionPool &pool;
public:
	SocketTask(ConnectionPool &connPool);
	//���� �������� ������ �������� ��� �����
	//� ������� �������� ������
	void operator()();
	virtual ~SocketTask();
};
