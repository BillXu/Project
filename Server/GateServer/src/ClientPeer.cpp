#include "ClientPeer.h"
#include "LogManager.h"
#include "ServerMessageDefine.h"
#include "GameServerPeer.h"
#include "CommonDefine.h"
#include "ServerNetwork.h"
#include "GatePeerManager.h"
CClientPeer::CClientPeer()
{
	m_ePeerType = eGatePeer_Client ;
	m_TimerWaitForReconnected = NULL ;
}

CClientPeer::~CClientPeer()
{
	if ( m_TimerWaitForReconnected )
		CTimerManager::SharedTimerManager()->RemoveTimer(m_TimerWaitForReconnected) ;
	m_TimerWaitForReconnected = NULL ;
}

void CClientPeer::Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	m_pGameServer = NULL ;
	m_bDisconnected = true ;
	m_nPlayerUID = 0 ;
	CGatePeer::Init(nPeerUID,nSelfNetGUID) ;
	m_TimerWaitForReconnected = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CClientPeer::OnRealRemove) ;
	m_TimerWaitForReconnected->SetDelayTime(TIME_WAIT_FOR_RECONNECTE) ;
}

void CClientPeer::Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	CGatePeer::Reset(nPeerUID,nSelfNetGUID) ;
	m_pGameServer = NULL ;
	m_bDisconnected = false ;
	m_TimerWaitForReconnected->Reset();
	m_TimerWaitForReconnected->Stop() ;
}

void CClientPeer::OnMessage( RakNet::Packet* pData )
{
	// warapper the message , then send the message to the game server ;
	if ( m_pGameServer == NULL )
	{ 
		CLogMgr::SharedLogMgr()->PrintLog( "No Game Server Connect Login " ) ;
		return ;
	}

	stMsgTransferData msgToSend ;
	msgToSend.cSysIdentifer = ID_MSG_GA2GM ;
	msgToSend.nSessionID = GetSessionID();
	memcpy(s_pBuffer,(void*)&msgToSend,sizeof(msgToSend));
	memcpy(s_pBuffer + sizeof(msgToSend),pData->data,pData->length) ;
	stMsg* pmsgReal = (stMsg*)pData->data ;
	if ( pmsgReal->cSysIdentifer == ID_MSG_C2LOGIN )
	{
		if ( !CGatePeerMgr::SharedGatePeerMgr()->TransferMsgToLoginServer(s_pBuffer,pData->length + sizeof(stMsgTransferData)) )
		{
			// send error msg ;
			CLogMgr::SharedLogMgr()->ErrorLog("Login Server invalid");
		}
	}
	else
	{
		CServerNetwork::SharedNetwork()->SendMsg(s_pBuffer,pData->length + sizeof(stMsgTransferData),m_pGameServer->GetSelfNetGUID(),false) ;
	}	
}

void CClientPeer::OnDisconnected()
{
	// CGatePeer::OnDisconnected(); // don't disconnect now immedely , wait for client reconnected ;
	m_nSelfNetGUID = RakNet::UNASSIGNED_RAKNET_GUID;
	m_bDisconnected = true ;
	if ( m_pGameServer )  // player in game server , so need wait to reconnected 
	{
		m_TimerWaitForReconnected->Reset();
		m_TimerWaitForReconnected->Start();
		CGatePeerMgr::SharedGatePeerMgr()->AddWaitForReconnected(this) ;
	}
	else
	{
		CGatePeer::OnDisconnected();
	}
}

void CClientPeer::JoinToServer(CGameServerPeer* pGameServer )
{
	if ( pGameServer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("cannot join to NULL GameServer") ;
	}
	m_pGameServer = pGameServer ;
}

void CClientPeer::OnGameServerCrashed(CGameServerPeer* pGameServer)
{
	m_pGameServer = NULL ;
	// send a message to client tell this message ;
	stMsg msg ;
	msg.cSysIdentifer = ID_MSG_S2C ;
	msg.usMsgType = MSG_GAME_SERVER_LOST ;
	CServerNetwork::SharedNetwork()->SendMsg((char*)&msg,sizeof(msg),GetSelfNetGUID(),false) ;
}

void CClientPeer::OnRealRemove(float fTimeElaps,unsigned int nTimerID )
{
	CGatePeer::OnDisconnected();
	if ( m_pGameServer)
	{
		m_pGameServer->OnClientPeerDisconnected(this) ;
	}
	m_TimerWaitForReconnected->Stop() ;
}

void CClientPeer::SetNewSelfNetUID( RakNet::RakNetGUID& nSelfNetGUID)
{
	m_nSelfNetGUID = nSelfNetGUID ;
	m_bDisconnected = false ;
	m_TimerWaitForReconnected->Stop() ;
}				