#include "SocketTask.h"
//------------------------------
#include <mutex>
//------------------------------
#include "ConnectionPool.h"
//------------------------------
SocketTask::SocketTask(ConnectionPool &connPool) :
	pool(connPool)
{
}

void SocketTask::operator()()
{
	//������� ��������� ������
	//���� ����� ���������� �� �� �������
	std::function<void()> sTask;
	for (;;)//����������� ���� ������ �����
	{
		{
			std::unique_lock<std::mutex> lock(pool.Queue_mut);

			//����� �� ���� ����� � ������� ����� ���-������ ��������
			while (!pool.Stop && pool.Connections.empty())
				pool.Condition.wait(lock);//������� �����

			if (pool.Stop)
				break;//��������� �������
			sTask = pool.Connections.front();//������� ������ �� �������
			pool.Connections.pop_front();//������� �� �� �������
		}
		sTask();//��������� ���������� ������ �� ����������
	}
}

SocketTask::~SocketTask()
{
}
