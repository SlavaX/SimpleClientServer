//------------------------------
#include "..\..\ClientServer\Public\ReceiverWinAPI.h"
//------------------------------
#include <iostream>
//------------------------------
#pragma comment (lib, "..\\..\\Debug\\ClientServer.lib")
//------------------------------
int main()
{
	ReceiverWinAPI Receiver;
	char buf[1] = { 0 };
	int len = sizeof(buf);
	Receiver.Receive(buf, len);
	return 0;
}