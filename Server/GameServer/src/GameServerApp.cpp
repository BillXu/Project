#include "GameServerApp.h"
#include "CommonDefine.h"
#include "MessageDefine.h"
#include "LogManager.h"
#include "PlayerManager.h"
#include "ServerMessageDefine.h"
#include <ctime>
#include "Timer.h"

#ifndef USHORT_MAX
#define USHORT_MAX 65535 
#endif

char* CGameServerApp::s_pBuffer = NULL ;
CGameServerApp* CGameServerApp::s_GameServerApp = NULL ;
CGameServerApp* CGameServerApp::SharedGameServerApp()
{
	 
	return s_GameServerApp ;
}

#include "CardPoker.h"
#include "PeerCard.h"
CGameServerApp::~CGameServerApp()
{
	delete m_pTimerMgr ;
	delete m_pNetWork ;
	delete m_pPlayerManager ;
}

void CGameServerApp::Init()
{
	m_bRunning = true ;
	if ( s_GameServerApp == NULL )
	{
		s_GameServerApp = this ;
	}
	else
	{
		CLogMgr::SharedLogMgr()->ErrorLog("Game Server App can not be init more than once !") ;
	}

	srand((unsigned int)time(0));
	if ( s_pBuffer == NULL )
	{
		s_pBuffer = new char[MAX_MSG_BUFFER_LEN] ;
	}
	m_bGateServerConnected = m_bDBserverConnected = false ;
	// test begin
	//int nItems = 100 ;
	//CPoker pocker ;
	//int A1Win = 0 ;
	//int A2Win = 0 ;
	//while( --nItems )
	//{
	//	CPeerCard* pd = new CPeerCard ;
	//	pocker.ComfirmKeepCard(6) ;
	//	pd->SetPeerCardByNumber(pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum());
	//	pd->LogInfo();
	//	CPeerCard* pd2 = new CPeerCard ;
	//	pd2->SetPeerCardByNumber(pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum());
	//	pd2->LogInfo();
	//	bool bWin = pd2->PKPeerCard(pd);
	//	if ( bWin )
	//	{
	//		printf("A2 win\n");
	//		++A2Win;
	//	}
	//	else
	//	{
	//		printf("A1 win\n");
	//		++A1Win;
	//	}
	//}
	//printf( "A1 win %d , A2 win %d \n" ,A1Win,A2Win);
	// test end ;
	m_strDBIP = "127.0.0.1" ;
	m_strGateIP = "127.0.0.1";
	m_pNetWork = new CNetWorkMgr ;
	m_pNetWork->SetupNetwork(2);
	m_pNetWork->AddMessageDelegate(this);
	ConnectToOtherServer();
	// init component ;
	m_pTimerMgr = new CTimerManager ;

	m_pRoomMgr.Init() ;
	m_pPlayerManager = new CPlayerManager ;
	
}

bool CGameServerApp::OnMessage( RakNet::Packet* pMsg )
{
	stMsg* MsgVer = (stMsg*)pMsg->data;
	if ( MsgVer->cSysIdentifer == ID_MSG_VERIFY )
	{
		// send back 
		stMsg msg ;
		msg.cSysIdentifer = ID_MSG_VERIFY ;
		msg.usMsgType = MSG_VERIFY_GAME ;
		m_pNetWork->SendMsg((char*)&msg,sizeof( msg),pMsg->guid ) ;

		// check idenditfy ;
		if ( MSG_VERIFY_GATE == MsgVer->usMsgType )
		{
			m_nGateServerNetUID = pMsg->guid ;
			m_bGateServerConnected = true ;
			return true ;
		}
		else if ( MSG_VERIFY_DB == MsgVer->usMsgType )
		{
			m_nDBServerNetUID = pMsg->guid ;
			m_bDBserverConnected = true ;
			return true ;
		}
	}
	return m_pPlayerManager->OnMessage(pMsg) ;
}

bool CGameServerApp::OnLostSever(RakNet::Packet* pMsg)
{
	if ( pMsg->guid == m_nGateServerNetUID )
	{
		m_bGateServerConnected = false ;
		CLogMgr::SharedLogMgr()->PrintLog("Gate Server Lost");
	}
	else
	{
		m_bDBserverConnected = false ;
		CLogMgr::SharedLogMgr()->PrintLog("DB Server Lost");
	}
	ConnectToOtherServer();
	return false ;
}

bool CGameServerApp::OnConnectStateChanged( eConnectState eSate )
{
	if ( eConnect_Failed == eSate || eConnect_SeverFull == eSate )
	{
		CLogMgr::SharedLogMgr()->PrintLog("connected Failed, Try Again !");
		ConnectToOtherServer();
	}
	return false;
}

bool CGameServerApp::Run()
{
	while (m_bRunning )
	{
		m_pNetWork->ReciveMessage() ;
		m_pTimerMgr->Update() ;
		Sleep(6);
	}
	ShutDown() ;
	return true ;
}

void CGameServerApp::ShutDown()
{
	m_pNetWork->ShutDown() ;
}

void CGameServerApp::SendMsgToGateServer( unsigned int nSessionID , const char* pBuffer , int nLen, bool bBroadcast )
{
	if ( m_bGateServerConnected == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("can not send msg to gate , because gate is not connecting !") ;
		return ;
	}
	stMsgTransferData msg ;
	msg.cSysIdentifer = ID_MSG_GM2GA ;
	msg.bBroadCast = bBroadcast ;
	msg.nSessionID = nSessionID ;
	memcpy(s_pBuffer,&msg,sizeof(stMsgTransferData));
	memcpy((void*)(s_pBuffer + sizeof(stMsgTransferData)),pBuffer,nLen);
	m_pNetWork->SendMsg(s_pBuffer,nLen + sizeof(stMsgTransferData),m_nGateServerNetUID) ;
}

void CGameServerApp::SendMsgToDBServer(const char* pBuffer , int nLen )
{
	if ( m_bDBserverConnected == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("can not send msg to gate , because DBServer is not connecting !") ;
		return ;
	}
	m_pNetWork->SendMsg(pBuffer,nLen,m_nDBServerNetUID) ;
}

void CGameServerApp::ConnectToOtherServer()
{
	if (! m_bGateServerConnected )
	{
		CLogMgr::SharedLogMgr()->PrintLog("connecting to GateServer ") ;
		m_pNetWork->ConnectToServer(m_strGateIP.c_str(),GATE_SERVER_PORT) ; // gate server ;
	}
	
	if ( !m_bDBserverConnected && m_strDBIP.empty() == false)
	{
		CLogMgr::SharedLogMgr()->PrintLog("connecting to DBServer ") ;
		m_pNetWork->ConnectToServer(m_strDBIP.c_str(),DB_PORT) ; // DB server ;
	}
}