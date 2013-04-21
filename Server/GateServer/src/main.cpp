#include "ServerNetwork.h"
#include "GatePeerManager.h"
#include "LogManager.h"
#include "GatePeer.h"

BOOL WINAPI ConsoleHandler(DWORD msgType)
{    
	CServerNetwork::SharedNetwork()->ShutDown();
	delete [] CGatePeer::s_pBuffer ;
	//if (msgType == CTRL_C_EVENT)
	//{        
	//	printf("Ctrl-C!\n");        
	//	_ExitFlag = 1;        
	//	return TRUE;    
	//}
	//else if (msgType == CTRL_CLOSE_EVENT)
	//{ 
	//	printf("Close console window!\n");
	//	_ExitFlag = 1; 
	//	/* Note: The system gives you very limited time to exit in this condition */        
	//	return TRUE;
	//}    
	/*
	Other messages: 
	CTRL_BREAK_EVENT
	Ctrl-Break pressed
	CTRL_LOGOFF_EVENT
	User log off
	CTRL_SHUTDOWN_EVENT
	System shutdown  
	*/    
	return TRUE;
} 


int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	CLogMgr::SharedLogMgr()->PrintLog("Starting Gate Server");
	CServerNetwork::SharedNetwork()->StartupNetwork(3000,25535);
	CServerNetwork::SharedNetwork()->AddDelegate(CGatePeerMgr::SharedGatePeerMgr());
	while( true )
	{
		CServerNetwork::SharedNetwork()->RecieveMsg();
		Sleep(5);
	}
	return 0 ; 
}