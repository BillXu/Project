#pragma once
#include "NetWorkManager.h"
class CPlayerData
	:public CNetMessageDelegate
{
public:
	CPlayerData();
	~CPlayerData(){}
	bool OnMessage( RakNet::Packet* pMsg ) ;
};