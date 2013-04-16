#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include <iostream>
#include "MessageDefine.h"
#include "NetWorkManager.h"
#include "PlayerManager.h"
void TestGameServer()
{
	CNetWorkMgr::SharedNetWorkMgr()->SetupNetwork(2);
	CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer("127.0.0.1",3000) ; // gate server ;
	CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer("127.0.0.1",8000) ; // DB server ;
	CNetWorkMgr::SharedNetWorkMgr()->AddMessageDelegate(CPlayerManager::SharedPlayerMgr());
	while ( 1 )
	{
		CNetWorkMgr::SharedNetWorkMgr()->ReciveMessage() ;
		Sleep(10) ;
	}
	
}

BOOL WINAPI ConsoleHandler(DWORD msgType)
{    
	CNetWorkMgr::SharedNetWorkMgr()->ShutDown(); 
	return TRUE;
} 
int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	//RakNet::RakPeerInterface *client = RakNet::RakPeerInterface::GetInstance();
	//RakNet::SocketDescriptor socketDescriptor;
	//socketDescriptor.port = 0;
	//client->Startup(1, &socketDescriptor, 1 );
	//bool rs = (client->Connect("127.0.0.1", 3000, NULL, 0, 0)== RakNet::CONNECTION_ATTEMPT_STARTED);

	//getchar();
	//static unsigned int count = 1 ;
	//while ( 1 )
	//{
	//	unsigned char pbuffer[5];
	//	pbuffer[0] = ID_USER_PACKET_ENUM ;
	//	memcpy(&pbuffer[1],&count,sizeof(int)); 
	//	stMsg msg ;
	//	msg.cSysIdentifer = ID_MSG_VERIFY ;
	//	msg.usMsgType = MSG_VERIFY_GMS ;
	//	client->Send((const char*)&msg,sizeof(msg),IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	//	++count ;
	//	Sleep(10);
	//}
	//client->Shutdown(1);
	//getchar();
	TestGameServer();
	return 0 ;
}