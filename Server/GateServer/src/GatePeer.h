#pragma once
#include "RakNetTypes.h"
class CGatePeer
{
public:
	CGatePeer(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID );
	~CGatePeer();
	void Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	void OnMessage( RakNet::Packet* pData );
	void OnDisconnected();
	void SetGameServerNetGUID(RakNet::RakNetGUID& nNetGUID ){ m_nGameServerNetGUID = nNetGUID ; }
	void OnPeerDisconnect(CGatePeer* peer );
	RakNet::RakNetGUID& GetSelfNetGUID(){ return m_nSelfNetGUID ; }
	void OnAddPeerToThisServer(CGatePeer* peer );
	unsigned short GetOwnPlayers(){ return m_nPlayerOnThisGameServer ;}
protected:
	unsigned int m_nPeerUID ;
	RakNet::RakNetGUID m_nSelfNetGUID ;
	RakNet::RakNetGUID m_nGameServerNetGUID ; // int bServer = true , m_nSelfNetGUID = m_nGameServerNetGUID ;
	bool m_bServer ;
	unsigned short m_nPlayerOnThisGameServer ; 
	// static Data ;
	static char* s_pBuffer ;
	static char* s_nUsedBufferLen ;
};