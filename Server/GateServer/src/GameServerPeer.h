#pragma once 
#include "GatePeer.h"
#include <map>
class CClientPeer ;
class CGameServerPeer
	:public CGatePeer
{
public:
	typedef std::map<unsigned int,CClientPeer*> MAP_CLIENT_PEER;
public:
	CGameServerPeer();
	virtual void Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID) ;
	virtual void OnMessage( RakNet::Packet* pData );
	virtual void OnDisconnected();
	void OnClientPeerDisconnected(CClientPeer* pPeer );
	void AddClientPeer(CClientPeer* pClientPeer);
	int GetClientPeerCount(){ return m_vClientPeers.size() ;}
protected:
	CClientPeer* GetClientPeer(unsigned int nSessionID );
protected: 
	MAP_CLIENT_PEER m_vClientPeers ;
};