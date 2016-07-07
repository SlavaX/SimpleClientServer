#pragma once
//------------------------------
#include <thread>
#include <vector>
#include <deque>
#include <condition_variable>
//------------------------------
//Сервер будет работать на основе пула потоков
//каждое соединение в своем потоке
//------------------------------
class ConnectionPool
{
friend class SocketTask;
public:
	ConnectionPool(size_t HandlersQuantity);
	virtual ~ConnectionPool();
	//Добавить задачу в очередь
	void AddTask(std::function<void()> f);
private:
	//Потоки для работы с соединениями
	std::vector< std::thread > Handlers;
	//Очередь соединений для обработки
	std::deque< std::function<void()> > Connections;
	//Признак того, что сервер остановлен
	//и задачи из очереди забирать больше не надо
	bool Stop;
	std::mutex Queue_mut;
	std::condition_variable Condition;
};

