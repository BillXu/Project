#pragma once
#include "RakNetTypes.h"
#include "GatePeerManager.h"
class CGatePeer
{
public:
	CGatePeer(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID );
	~CGatePeer();
	void Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	void OnMessage( RakNet::Packet* pData );
	void OnDisconnected();
	void SetGameServerPeer(CGatePeer* pServerPeer ){ m_pGameServerPeer = pServerPeer ; }
	void OnPeerDisconnect(CGatePeer* peer );
	RakNet::RakNetGUID& GetSelfNetGUID(){ return m_nSelfNetGUID ; }
	void OnAddPeerToThisServer(CGatePeer* peer );
	unsigned short GetOwnPlayers(){ return m_vClientOnThisServer.size() ;}
	void SetServer(bool bIsServer){m_bServer = bIsServer ;}
protected:
	bool IsServer(){ return m_bServer ;}
	CGatePeer* GetClientPeerOnThisByPeerUID( unsigned int nPeerUID );
protected:
	unsigned int m_nPeerUID ;
	RakNet::RakNetGUID m_nSelfNetGUID ;
	CGatePeer* m_pGameServerPeer ; // int bServer = true , m_pGameServerPeer = NULL ;
	bool m_bServer ;
	CGatePeerMgr::LIST_GATEPEER m_vClientOnThisServer ;
	// static Data ;
public:
	static char* s_pBuffer ;
};