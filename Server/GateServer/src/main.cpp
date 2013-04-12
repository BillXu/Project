#include "ServerNetwork.h"
#include "GatePeerManager.h"
#include "LogManager.h"
#include "GatePeer.h"
int main()
{
	CLogMgr::SharedLogMgr()->PrintLog("Starting Gate Server");
	CServerNetwork::SharedNetwork()->StartupNetwork(3000,25535);
	CServerNetwork::SharedNetwork()->AddDelegate(CGatePeerMgr::SharedGatePeerMgr());
	while( true )
	{
		CServerNetwork::SharedNetwork()->RecieveMsg();
		Sleep(5);
	}
	CServerNetwork::SharedNetwork()->ShutDown();
	delete [] CGatePeer::s_pBuffer ;
	return 0 ; 
}