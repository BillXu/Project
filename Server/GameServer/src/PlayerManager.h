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
	virtual bool OnLostSever(RakNet::Packet* pMsg);
	void SendMsgToGateServer( unsigned int nUserUID , const char* pBuffer , int nLen, bool bBroadcast = false );
	void SendMsgToDBServer( unsigned int nUserUID , const char* pBuffer , int nLen );
	CPlayer* GetPlayerByUserUID( unsigned int nUserUID );
public:
	static char* s_pBuffer ;
protected:
	// server associate 
	RakNet::RakNetGUID m_nGateServerNetUID ;
	RakNet::RakNetGUID m_nDBServerNetUID ;
	bool m_bGateServerConnected ;
	bool m_bDBserverConnected ;
	// logic data ;
	MAP_PLAYERS m_vAllActivePlayers ;
	LIST_PLAYERS m_vAllReservePlayers ;
};