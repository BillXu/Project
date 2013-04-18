#include "main.h"
#include "MessageDefine.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include <iostream>
#include "Client.h"

BOOL WINAPI ConsoleHandler(DWORD msgType)
{    

	return TRUE;
} 

int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	CClient* pClient = NULL;
	int i = 0 ;
	while (  i++ < 1 )
	{
		pClient = new CClient ;
		pClient->Init() ;
		pClient->Start() ;
		Sleep(5);
	}
	getchar();
}