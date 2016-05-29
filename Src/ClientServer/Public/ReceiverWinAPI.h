#pragma once
//------------------------------
#include "..\Public\ClientWinAPI.h"
//------------------------------
class SOCKETDLL ReceiverWinAPI :
	public ClientWinAPI
{
public:
	void Receive(char *buf, int len);
	ReceiverWinAPI();
	virtual ~ReceiverWinAPI();
};

