#pragma once 
#include "GatePeer.h"
#include "Timer.h"
class CGameServerPeer;
class CClientPeer
	:public CTimerDelegate
	,public CGatePeer
{
public:
	CClientPeer();
	~CClientPeer();
	void Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	virtual void Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID);
	virtual void OnMessage( RakNet::Packet* pData );
	virtual void OnDisconnected();
	void JoinToServer(CGameServerPeer* pGameServer );
	void OnGameServerCrashed(CGameServerPeer* pGameServer);
	unsigned int GetPlayerUID(){ return m_nPlayerUID ;}
	void SetPlayerUID( unsigned int nPlayerUID ){ m_nPlayerUID = nPlayerUID ;}
	bool IsDisconnected(){ return m_bDisconnected ;}
	void OnRealRemove(float fTimeElaps,unsigned int nTimerID );
	void SetNewSelfNetUID( RakNet::RakNetGUID& nSelfNetGUID); // invoked then reconnect success ;
	CGameServerPeer* GetGameServerPeer(){ return m_pGameServer ;}
protected:
	CGameServerPeer* m_pGameServer ;
	unsigned int m_nPlayerUID ;
	bool m_bDisconnected ;
	CTimer* m_TimerWaitForReconnected ;
};