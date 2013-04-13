#pragma once 
#include "ServerNetwork.h"
#include <map>
#include <list>
class CGatePeer ;
class CGatePeerMgr
	:public CServerNetworkDelegate
{
public:
	typedef std::map<RakNet::RakNetGUID,CGatePeer*> MAP_GATEPEER ;
	typedef std::list<CGatePeer*> LIST_GATEPEER ; 
public:
	static CGatePeerMgr* SharedGatePeerMgr();
	CGatePeerMgr();
	~CGatePeerMgr();
	virtual bool OnMessage( RakNet::Packet* pData );
	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData );
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData );
	unsigned int GeneratePeerUID();
	bool AddPeerToServer(CGatePeer* pClientPeer );
	int GetAllOnLinePeerCount();
	void RemovePeer(CGatePeer* pPeer );
protected:
	CGatePeer* GetProperGameServerToAddClient();  // ¸ºÔØ¾ùºâ²ßÂÔ£»
	CGatePeer* GetReserveGatePeer();
	void AddToReservePeers(CGatePeer* pPeer );
	void  ClearPeers(MAP_GATEPEER&vPeers);
	void ClearAll();
protected:
	MAP_GATEPEER m_vAllGatePeers ;
	MAP_GATEPEER m_vServerPeers ;
	LIST_GATEPEER  m_vReservePeers ;
};