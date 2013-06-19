#pragma once
#include <map>
#include <list>
#include "NetWorkManager.h"
class CLoginPeer ;
class CLoginPeerMgr
{
public:
	typedef std::map<unsigned int , CLoginPeer* > MAP_LOGIN_PEER ;
	typedef std::list<CLoginPeer*> LIST_LOGIN_PEER ;
public:
	CLoginPeerMgr();
	~CLoginPeerMgr();
	void OnMessage( RakNet::Packet* pMsg ) ;
	CLoginPeer* GetPeerBySessionID(unsigned int nSessionID );
	CLoginPeer* GetReserverPeer();
	bool SendMsgToDB(const char* pBuffer , int nLen );
	bool SendMsgToGate(const char* pBuffer , int nLen );
protected:
	void ClearAll();
protected:
	MAP_LOGIN_PEER m_vAllPeers ;
	LIST_LOGIN_PEER m_vReseverLoginPeers ; 
};