#pragma once
#include "NetWorkManager.h"
class CMessageDelegate
	:public CNetMessageDelegate
{
public:
	CMessageDelegate():bCannected(false),bVerify(false),bRegisterOK(false){}
	bool OnMessage( RakNet::Packet* pMsg );
	bool OnConnectStateChanged( eConnectState eSate );
public:
	bool bCannected ;
	bool bVerify ;
	bool bRegisterOK ;
};