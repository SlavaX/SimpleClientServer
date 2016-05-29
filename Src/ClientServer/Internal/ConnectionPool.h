#pragma once
//------------------------------
#include <thread>
#include <vector>
#include <deque>
#include <condition_variable>
//------------------------------
//—ервер будет работать на основе пула потоков
//каждое соединение в своем потоке
//------------------------------
class ConnectionPool
{
friend class SocketTask;
public:
	ConnectionPool(size_t HandlersQuantity);
	virtual ~ConnectionPool();
	//ƒобавить задачу в очередь
	void AddTask(std::function<void()> f);
private:
	//ѕотоки дл€ работы с соединени€ми
	std::vector< std::thread > Handlers;
	//ќчередь соединений дл€ обработки
	std::deque< std::function<void()> > Connections;
	//ѕризнак того, что сервер остановлен
	//и задачи из очереди забирать больше не надо
	bool Stop;
	std::mutex Queue_mut;
	std::condition_variable Condition;
};

