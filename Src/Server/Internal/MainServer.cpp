//------------------------------
#include "..\..\ClientServer\Public\ServerWinAPI.h"
//------------------------------
#pragma comment (lib, "..\\..\\Debug\\ClientServer.lib")
//------------------------------
int main()
{
	ServerWinAPI Server;
	Server.Start();
	return 0;
}