//------------------------------
#include <fstream>
#include <iostream>
//------------------------------
#include "..\..\ClientServer\Public\SenderWinAPI.h"
//------------------------------
#pragma comment (lib, "..\\..\\Debug\\ClientServer.lib")
//------------------------------
int main(int argc, char *argv[])
{
	SenderWinAPI Sender;
	if (Sender.GetState())
	{
		char buf[] = "Что-то на русском\0I am a sender\0Hello\0";
		Sender.Send(buf, sizeof(buf));

		std::string AppName = argv[0];
		AppName = AppName.substr(0, AppName.find_last_of("/\\"));
		AppName += "\\en_US-custom.dic";

		std::fstream dicfile(AppName);
		if (dicfile.is_open())
		{
			char buf[256] = { 0 };

			while (dicfile.getline(buf, sizeof(buf), 0x0A))
			{
				std::cout << buf << '\n';
				std::string str = buf;
				Sender.Send(str.c_str(), str.size() + 1);
				//Sleep(1000);
			}
			dicfile.close();
		}
	}

	return 0;
}