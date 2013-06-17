#pragma once 
#include "GatePeer.h"
class CGatePeerMgr ;
class CLoginServerPeer
	:public CGatePeer
{
public:
	CLoginServerPeer();
	virtual void OnMessage( RakNet::Packet* pData );
	virtual void OnDisconnected(){ m_bActive = false ;}
	virtual void Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	bool IsActive(){return m_bActive;}
	void SetGatePeerMgr(CGatePeerMgr* peerMgr){ m_pGatePeerMgr = peerMgr ;}
protected:
	bool m_bActive ;
	CGatePeerMgr* m_pGatePeerMgr ;
};