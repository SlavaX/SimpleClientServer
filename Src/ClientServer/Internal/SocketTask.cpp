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
	//Создать экземпляр задачи
	//куда будем копировать ее из очереди
	std::function<void()> sTask;
	for (;;)//бесконечный цикл поиска задач
	{
		{
			std::unique_lock<std::mutex> lock(pool.Queue_mut);

			//Здесь мы ждем когда в очереди задач что-нибудь появится
			while (!pool.Stop && pool.Connections.empty())
				pool.Condition.wait(lock);//усыпить поток

			if (pool.Stop)
				break;//остановка сервера
			sTask = pool.Connections.front();//забрать задачу из очереди
			pool.Connections.pop_front();//удалить ее из очереди
		}
		sTask();//запустить полученную задачу на исполнение
	}
}

SocketTask::~SocketTask()
{
}
