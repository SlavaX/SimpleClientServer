//------------------------------
#include "ConnectionPool.h"
//------------------------------
#include "SocketTask.h"
//------------------------------
ConnectionPool::ConnectionPool(size_t HandlersQuantity) :
	Stop(false)
{
	//Здесь мы создаем весь пул потоков
	for (size_t i = 0; i<HandlersQuantity; ++i)
		Handlers.push_back(std::thread(SocketTask(*this))); 
}

void ConnectionPool::AddTask(std::function<void()> f)
{
	{
		std::unique_lock<std::mutex> lock(Queue_mut);
		//Добавляем задачу
		Connections.push_back(std::function<void()>(f));
	} 
	//Здесь мы оповещаем один из спящих потоков
	Condition.notify_one();
}

ConnectionPool::~ConnectionPool()
{
	//остановить потоки
	this->Stop = true;
	Condition.notify_all();
	//вернуть их к главному потоку
	for (auto it = this->Handlers.begin(); it != this->Handlers.end(); ++it)
		it->join();
}
