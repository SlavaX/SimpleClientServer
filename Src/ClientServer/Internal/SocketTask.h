#pragma once
class ConnectionPool;
//Этот объект является частью класса ConnectionPool
class SocketTask
{
private:
	//Ссылка для того, чтобы была возможность
	//определить состояние очереди задач для выполнения
	ConnectionPool &pool;
public:
	SocketTask(ConnectionPool &connPool);
	//Этот оператор должен работать все время
	//с момента создания потока
	void operator()();
	virtual ~SocketTask();
};
