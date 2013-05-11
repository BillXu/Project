#pragma once
#include "NetWorkManager.h"
#include "RoomManager.h"
class CGameServerApp
	:public CNetMessageDelegate
{
public:
	static CGameServerApp* SharedGameServerApp();
	void Init();
	virtual bool OnMessage( RakNet::Packet* pMsg );
	virtual bool OnLostSever(RakNet::Packet* pMsg);
	bool OnConnectStateChanged( eConnectState eSate );
	bool Run();
	void ShutDown();

	void SendMsgToGateServer( unsigned int nUserUID , const char* pBuffer , int nLen, bool bBroadcast = false );
	void SendMsgToDBServer( const char* pBuffer , int nLen );
	CRoomManager* GetRoomMgr(){ return &m_pRoomMgr;}
protected:
	void ConnectToOtherServer();
public:
	static char* s_pBuffer ;
protected:
	// server associate 
	std::string m_strGateIP;
	unsigned short m_nGatePort ;
	
	std::string m_strDBIP ;
	unsigned short m_nDBPort ;

	RakNet::RakNetGUID m_nGateServerNetUID ;
	RakNet::RakNetGUID m_nDBServerNetUID ;
	bool m_bGateServerConnected ;
	bool m_bDBserverConnected ;
	// conpentent ;
	CRoomManager m_pRoomMgr ;
};