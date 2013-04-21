#pragma once 
#include "NetWorkManager.h"
#include <map>
#include <list>
class CPlayer ;
class CPlayerManager
	:public CNetMessageDelegate
{
public:
	typedef std::map<unsigned int, CPlayer*> MAP_PLAYERS ;
	typedef std::list<CPlayer*> LIST_PLAYERS ;
public:
	static CPlayerManager* SharedPlayerMgr();
	CPlayerManager();
	~CPlayerManager();
	virtual bool OnMessage( RakNet::Packet* pMsg );
	bool OnLostSever(bool bGateDown);
	void SendMsgToGateServer( unsigned int nUserUID , const char* pBuffer , int nLen, bool bBroadcast = false );
	void SendMsgToDBServer( const char* pBuffer , int nLen );
	CPlayer* GetPlayerByUserUID( unsigned int nUserUID );
protected:
	void ProcessMsgFromDBServer(stMsg* pMessage ,RakNet::Packet* pMsg );
	void processMsgFromGateServer(stMsg* pMessage ,RakNet::Packet* pMsg );
	void PushReserverPlayers( CPlayer* ) ;
	CPlayer* GetReserverPlayer();
protected:
	// logic data ;
	MAP_PLAYERS m_vAllActivePlayers ;
	LIST_PLAYERS m_vAllReservePlayers ;
};