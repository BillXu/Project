#include "NetWorkManager.h"
class CMessageDelegate
	:public CNetMessageDelegate
{
public:
	bool OnMessage( RakNet::Packet* pMsg );
	bool OnConnectStateChanged( eConnectState eSate );
};