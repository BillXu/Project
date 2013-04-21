#include "GameServerApp.h"
#include <iostream>
BOOL WINAPI ConsoleHandler(DWORD msgType)
{    
	CGameServerApp::SharedGameServerApp()->ShutDown();
	return TRUE;
} 
int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	CGameServerApp::SharedGameServerApp()->Init();
	while ( CGameServerApp::SharedGameServerApp()->Run())
	{
		Sleep(10) ;
	}
	CGameServerApp::SharedGameServerApp()->ShutDown();
	return 0 ;
}