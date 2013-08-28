#pragma once
#include "NetWorkManager.h"
#include "RoomManager.h"
#include "Timer.h"
#include "PlayerManager.h"
class CGameServerApp
	:public CNetMessageDelegate
{
public:
	static CGameServerApp* SharedGameServerApp();
	~CGameServerApp();
	void Init();
	virtual bool OnMessage( RakNet::Packet* pMsg );
	virtual bool OnLostSever(RakNet::Packet* pMsg);
	bool OnConnectStateChanged( eConnectState eSate );
	bool Run();
	void ShutDown();

	void SendMsgToGateServer( unsigned int nSessionID , const char* pBuffer , int nLen, bool bBroadcast = false );
	void SendMsgToDBServer(const char* pBuffer , int nLen );
	CRoomManager* GetRoomMgr(){ return &m_pRoomMgr;}
	CTimerManager* GetTimerMgr(){ return m_pTimerMgr ; }
	CPlayerManager* GetPlayerMgr(){ return m_pPlayerManager ; }
	void Stop(){ m_bRunning = false ;}
protected:
	void ConnectToOtherServer();
public:
	static char* s_pBuffer ;
	static CGameServerApp* s_GameServerApp ;
protected:
	bool m_bRunning  ;
	// server associate 
	std::string m_strGateIP;
	std::string m_strDBIP ;
	RakNet::RakNetGUID m_nGateServerNetUID ;
	RakNet::RakNetGUID m_nDBServerNetUID ;
	bool m_bGateServerConnected ;
	bool m_bDBserverConnected ;
	// conpentent ;
	CRoomManager m_pRoomMgr ;
	CTimerManager* m_pTimerMgr ;
	CNetWorkMgr* m_pNetWork ;
	CPlayerManager* m_pPlayerManager ;
};