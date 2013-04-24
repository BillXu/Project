#include "GameServerApp.h"
#include "CommonDefine.h"
#include "MessageDefine.h"
#include "LogManager.h"
#include "PlayerManager.h"
#include "ServerMessageDefine.h"
#include <ctime>

#ifndef USHORT_MAX
#define USHORT_MAX 65535 
#endif

char* CGameServerApp::s_pBuffer = NULL ;
CGameServerApp* CGameServerApp::SharedGameServerApp()
{
	static CGameServerApp g_sApp ;
	return &g_sApp ;
}

#include "CardPoker.h"
#include "PeerCard.h"
void CGameServerApp::Init()
{
	srand(time(0));
	if ( s_pBuffer == NULL )
	{
		s_pBuffer = new char[MAX_MSG_BUFFER_LEN] ;
	}
	m_bGateServerConnected = m_bDBserverConnected = false ;
	// test begin
	int nItems = 100 ;
	CPoker pocker ;
	int A1Win = 0 ;
	int A2Win = 0 ;
	while( --nItems )
	{
		CPeerCard* pd = new CPeerCard ;
		pocker.ComfirmKeepCard(6) ;
		pd->SetPeerCardByNumber(pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum());
		pd->LogInfo();
		CPeerCard* pd2 = new CPeerCard ;
		pd2->SetPeerCardByNumber(pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum(),pocker.GetCardWithCompositeNum());
		pd2->LogInfo();
		bool bWin = pd2->PKPeerCard(pd);
		if ( bWin )
		{
			printf("A2 win\n");
			++A2Win;
		}
		else
		{
			printf("A1 win\n");
			++A1Win;
		}
	}
	printf( "A1 win %d , A2 win %d \n" ,A1Win,A2Win);
	// test end ;
	m_strDBIP = "" ;
	m_strGateIP = "";
	m_nDBPort = 8000;
	m_nGatePort = 3000;
	CNetWorkMgr::SharedNetWorkMgr()->SetupNetwork(2);
	CNetWorkMgr::SharedNetWorkMgr()->AddMessageDelegate(this,USHORT_MAX); // process first ;
	CNetWorkMgr::SharedNetWorkMgr()->AddMessageDelegate(CPlayerManager::SharedPlayerMgr());
	ConnectToOtherServer();
}

bool CGameServerApp::OnMessage( RakNet::Packet* pMsg )
{
	stMsg* pMessage = (stMsg*)pMsg->data ;
	if ( ID_MSG_VERIFY == pMessage->cSysIdentifer && pMessage->usMsgType == MSG_VERIFY_GA )
	{
		m_bGateServerConnected = true ;
		m_nGateServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to gate Server : %s ", pMsg->systemAddress.ToString());

		// verify self 
		stMsg msg ;
		msg.cSysIdentifer = ID_MSG_VERIFY ;
		msg.usMsgType = MSG_VERIFY_GMS ;
		CNetWorkMgr::SharedNetWorkMgr()->SendMsg((char*)&msg,sizeof(msg),pMsg->guid) ;
		return  true;
	}
	else if ( ID_MSG_VERIFY == pMessage->cSysIdentifer && pMessage->usMsgType == MSG_VERIFY_DB )
	{
		m_bDBserverConnected = true ;
		m_nDBServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to DBServer : %s ", pMsg->systemAddress.ToString());
		return  true;
	}
	return false ;
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
	CPlayerManager::SharedPlayerMgr()->OnLostSever( m_bGateServerConnected == false ) ;
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
	CNetWorkMgr::SharedNetWorkMgr()->ReciveMessage() ;
	return true ;
}

void CGameServerApp::ShutDown()
{
	CNetWorkMgr::SharedNetWorkMgr()->ShutDown() ;
}

void CGameServerApp::SendMsgToGateServer( unsigned int nUserUID , const char* pBuffer , int nLen, bool bBroadcast )
{
	if ( m_bGateServerConnected == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("can not send msg to gate , because gate is not connecting !") ;
		return ;
	}
	stMsgTransferData msg ;
	msg.cSysIdentifer = ID_MSG_GM2GA ;
	msg.bBroadCast = bBroadcast ;
	msg.nTargetPeerUID = nUserUID ;
	memcpy(s_pBuffer,&msg,sizeof(stMsgTransferData));
	memcpy((void*)(s_pBuffer + sizeof(stMsgTransferData)),pBuffer,nLen);
	CNetWorkMgr::SharedNetWorkMgr()->SendMsg(s_pBuffer,nLen + sizeof(stMsgTransferData),m_nGateServerNetUID) ;
}

void CGameServerApp::SendMsgToDBServer( const char* pBuffer , int nLen )
{
	if ( m_bDBserverConnected == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("can not send msg to gate , because DBServer is not connecting !") ;
		return ;
	}
	CNetWorkMgr::SharedNetWorkMgr()->SendMsg(pBuffer,nLen,m_nDBServerNetUID) ;
}

void CGameServerApp::ConnectToOtherServer()
{
	if (! m_bGateServerConnected )
	{
		CLogMgr::SharedLogMgr()->PrintLog("connecting to GateServer ") ;
		CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer(m_strGateIP.c_str(),m_nGatePort) ; // gate server ;
	}
	
	if ( !m_bDBserverConnected )
	{
		CLogMgr::SharedLogMgr()->PrintLog("connecting to DBServer ") ;
		CNetWorkMgr::SharedNetWorkMgr()->ConnectToServer(m_strDBIP.c_str(),m_nDBPort) ; // DB server ;
	}
}