#pragma once
//------------------------------
#include "ClientWinAPI.h"
//------------------------------
class SOCKETDLL SenderWinAPI :
	public ClientWinAPI
{
public:
	void Send(const char *buf, int len);
	SenderWinAPI();
	~SenderWinAPI();
};

