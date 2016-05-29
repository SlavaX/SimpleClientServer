//------------------------------
#include "ConnectionPool.h"
//------------------------------
#include "SocketTask.h"
//------------------------------
ConnectionPool::ConnectionPool(size_t HandlersQuantity) :
	Stop(false)
{
	//����� �� ������� ���� ��� �������
	for (size_t i = 0; i<HandlersQuantity; ++i)
		Handlers.push_back(std::thread(SocketTask(*this))); 
}

void ConnectionPool::AddTask(std::function<void()> f)
{
	{
		std::unique_lock<std::mutex> lock(Queue_mut);
		//��������� ������
		Connections.push_back(std::function<void()>(f));
	} 
	//����� �� ��������� ���� �� ������ �������
	Condition.notify_one();
}

ConnectionPool::~ConnectionPool()
{
	//���������� ������
	this->Stop = true;
	Condition.notify_all();
	//������� �� � �������� ������
	for (auto it = this->Handlers.begin(); it != this->Handlers.end(); ++it)
		it->join();
}
