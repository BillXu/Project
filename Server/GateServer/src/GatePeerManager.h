#pragma once 
#include "ServerNetwork.h"
#include "ClientPeer.h"
#include <map>
#include <list>
class CLoginServerPeer ;
class CGatePeer ;
class CGatePeerMgr
	:public CServerNetworkDelegate
{
public:
	typedef std::map<RakNet::RakNetGUID,CGatePeer*> MAP_GATEPEER ;
	typedef std::map<unsigned int,CClientPeer* > MAP_UID_CLIENT_PEER ;
	typedef std::list<CGatePeer*> LIST_GATEPEER ; 
public:
	static CGatePeerMgr* SharedGatePeerMgr();
	CGatePeerMgr();
	~CGatePeerMgr();
	virtual bool OnMessage( RakNet::Packet* pData );
	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData );
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData );
	bool AddPeerToServer(CClientPeer* pClientPeer );
	int GetAllOnLinePeerCount();
	void RemovePeer(CGatePeer* pPeer );
	void Update(float fTimeElsps );
	void AddWaitForReconnected(CClientPeer* peerWait);
	CGatePeer* GetAcitveClientPeerBySessionID(unsigned int nSessionID );
	bool TransferMsgToLoginServer(char* pBuffer, unsigned short nLen );
protected:
	unsigned int GenerateSessionID();
	CGatePeer* GetProperGameServerToAddClient();  // ¸ºÔØ¾ùºâ²ßÂÔ£»
	CGatePeer* GetReserveGatePeer(CGatePeer::eGatePeerType eType );
	void AddToReservePeers(CGatePeer* pPeer );
	void  ClearPeers(MAP_GATEPEER&vPeers);
	void ClearAll();
	void AddNewPeer(CGatePeer* pNewPeer );
	CGatePeer* GetAcitvePeer(RakNet::RakNetGUID& nNetUID );
	bool ProcessGateLogicMsg(RakNet::Packet* pData);
protected:
	CLoginServerPeer* m_pLoginServer ;
	MAP_GATEPEER m_vGameServerPeers ;
	MAP_GATEPEER m_vClientPeers ;

	MAP_UID_CLIENT_PEER m_vWaitReconnectedClientPeers ; // most for reconnected ;

	LIST_GATEPEER  m_vReservePeers ;
	LIST_GATEPEER m_vWillRemove;
};