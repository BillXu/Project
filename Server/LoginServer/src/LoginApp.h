#pragma once 
#include "NetWorkManager.h"
#include "Timer.h"
class CLoginPeerMgr;
class CLoginApp
	:public CNetMessageDelegate
	,public CTimerDelegate
{
public:
	struct stServerInfo 
	{
		bool m_bConnected ;
		RakNet::RakNetGUID m_nServerNetID ;
		std::string m_strIPAddress ;
		unsigned short m_nPort ;
	};

public:
	CLoginApp();
	~CLoginApp();
	void Init();
	void MainLoop();
	virtual bool OnMessage( RakNet::Packet* pMsg );
	virtual bool OnLostSever(RakNet::Packet* pMsg);
	bool SendMsg( const char* pBuffer , unsigned int nLen , bool bGate );
protected:
	bool SendMsgToGate(const char* pBuffer , unsigned int nLen);
	bool SendMsgToDB(const char* pBuffer , unsigned int nLen);
protected:
	stServerInfo m_stGateServer ;

	stServerInfo m_stDBServer ;

	CNetWorkMgr* m_pNetWork;

	CLoginPeerMgr* m_pPeerMgr ;

	CTimer* m_pReconnctGate ;
	CTimer* m_pReconnectDB ;
};