#include "GameServerApp.h"
#include <iostream>
BOOL WINAPI ConsoleHandler(DWORD msgType)
{    
	CGameServerApp::SharedGameServerApp()->Stop();
	return TRUE;
} 
int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	CGameServerApp theApp ;
	theApp.Init() ;
	theApp.Run() ;
	return 0 ;
}