#include "PlayerManager.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
#include "Player.h"
#include "CommonDefine.h"
#include "GameServerApp.h"
CPlayerManager* CPlayerManager::SharedPlayerMgr()
{
	static CPlayerManager g_sPlayerMgr ;
	return &g_sPlayerMgr ;
}

CPlayerManager::CPlayerManager()
{
	m_vAllReservePlayers.clear() ;
	m_vAllActivePlayers.clear();
}

CPlayerManager::~CPlayerManager()
{
	MAP_PLAYERS::iterator iter = m_vAllActivePlayers.begin();
	for ( ; iter != m_vAllActivePlayers.end() ; ++iter )
	{
		delete iter->second ;
		iter->second = NULL ;
	}
	m_vAllActivePlayers.clear() ;

	LIST_PLAYERS::iterator iter_R = m_vAllReservePlayers.begin() ;
	for ( ; iter_R != m_vAllReservePlayers.end(); ++iter_R )
	{
		delete *iter_R ;
		*iter_R = NULL ;
	}
	m_vAllReservePlayers.clear() ;
}

bool CPlayerManager::OnMessage( RakNet::Packet* pMsg )
{
	stMsg* pMessage = (stMsg*)pMsg->data ;
	if ( pMessage->cSysIdentifer ==  ID_MSG_GA2GM )
	{
		processMsgFromGateServer(pMessage,pMsg) ;
	}
	else if ( ID_MSG_DB2GM == pMessage->cSysIdentifer )
	{
		ProcessMsgFromDBServer(pMessage,pMsg) ;
	}
	else
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Receive Unknown message cSysIdentifer = %d ,usMsgType = %d ",pMessage->cSysIdentifer,pMessage->usMsgType ) ;
	}
	return false ;
}

void CPlayerManager::ProcessMsgFromDBServer(stMsg* pMessage ,RakNet::Packet* pMsg  )
{
	if ( MSG_TRANSER_DATA == pMessage->usMsgType ) // game server don't prcess tranfer msg from DBServer ;
	{
		stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMessage ;
		stMsg* pTargetMessage = (stMsg*)(pMsg->data + sizeof(stMsgTransferData));
		pMsgTransfer->cSysIdentifer = ID_MSG_GM2GA ;
		if ( pTargetMessage->usMsgType == MSG_LOGIN )  // if login ok , we alloct player ;
		{
			stMsgLoginRet* pTarget = (stMsgLoginRet*)pTargetMessage ;
			if ( pTarget->bOk )
			{
				CPlayer* pPlayer = GetReserverPlayer();
				pPlayer->Reset(pMsgTransfer->nTargetPeerUID) ;
				m_vAllActivePlayers[pPlayer->GetUserUID()] = pPlayer ;
			}
		}
		SendMsgToGateServer(pMsgTransfer->nTargetPeerUID,(char*)pTargetMessage,pMsg->length - sizeof(stMsgTransferData) ) ;
	}
	else
	{
		stMsgDB2GM* pMsgFromDB = (stMsgDB2GM*)pMessage ;
		CPlayer* pTargetPlayer = GetPlayerByUserUID(pMsgFromDB->nTargetUserUID ) ;
		if ( !pTargetPlayer )
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Can not find target Player, so message From DBServer will not be processed" );
			return ;
		}
		pTargetPlayer->OnMessage(pMessage);
	}
}

void CPlayerManager::processMsgFromGateServer(stMsg* pMessage ,RakNet::Packet* pMsg  )
{
	if ( MSG_TRANSER_DATA == pMessage->usMsgType )
	{
		stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMessage ;
		stMsg* pTargetMessage = (stMsg*)(pMsg->data + sizeof(stMsgTransferData));
		// special msg ; game server don't process , just send it to DBServer ;
		if ( pTargetMessage->usMsgType == MSG_REGISTE || MSG_LOGIN == pTargetMessage->usMsgType )
		{
			SendMsgToDBServer((char*)pMsg->data,pMsg->length) ; 
			return  ;
		}
		// special msg 
		CPlayer* pTargetPlayer = GetPlayerByUserUID(pMsgTransfer->nTargetPeerUID) ;
		if ( !pTargetPlayer )
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Can not find target Player, so message From Gate will not be processed" );
			return  ;
		}
		pTargetPlayer->OnMessage(pTargetMessage);
	}
	else if (MSG_DISCONNECT == pMessage->usMsgType)
	{
		stMsgPeerDisconnect* pRealMsg = (stMsgPeerDisconnect*)pMessage ;
		MAP_PLAYERS::iterator iter = m_vAllActivePlayers.find(pRealMsg->nPeerUID) ;
		//CPlayer* pTargetPlayer = GetPlayerByUserUID(pRealMsg->nPeerUID) ;
		if ( iter == m_vAllActivePlayers.end())
		{
			//CLogMgr::SharedLogMgr()->ErrorLog( "Can not find target Player, so Disconnected message From Gate will not be processed" );
		}
		else
		{
			CPlayer* pTargetPlayer = iter->second ;
			if ( pTargetPlayer)
			{
				pTargetPlayer->OnDisconnect();
				PushReserverPlayers( pTargetPlayer );
			}
			m_vAllActivePlayers.erase(iter) ;
		}
		// tell DBserver this peer discannected ;
		pRealMsg->cSysIdentifer = ID_MSG_GM2DB ;
		SendMsgToDBServer((char*)pMsg->data,pMsg->length) ;
	}
}

bool CPlayerManager::OnLostSever(bool bGateDown)
{
	MAP_PLAYERS::iterator iter = m_vAllActivePlayers.begin();
	for ( ; iter != m_vAllActivePlayers.end(); ++iter )
	{
		if ( iter->second )
		{
			if ( bGateDown )
			{
				iter->second->OnGateServerLost();
				stMsgPeerDisconnect pRealMsg;
				// tell DBserver this peer discannected ;
				pRealMsg.cSysIdentifer = ID_MSG_GM2DB ;
				pRealMsg.nPeerUID = iter->first ;
				SendMsgToDBServer((char*)&pRealMsg,sizeof(stMsgPeerDisconnect)) ;
				// remove the peer ;
				PushReserverPlayers(iter->second) ;
			}
			else 
			{
				iter->second->OnDBServerLost();
			}
		}
	}

	if ( bGateDown )
	{
		m_vAllActivePlayers.clear();
	}
	return false ;
}

void CPlayerManager::SendMsgToGateServer( unsigned int nUserUID , const char* pBuffer , int nLen, bool bBroadcast )
{
	CGameServerApp::SharedGameServerApp()->SendMsgToGateServer(nUserUID,pBuffer,nLen,bBroadcast) ;
}

void CPlayerManager::SendMsgToDBServer( const char* pBuffer , int nLen )
{
	CGameServerApp::SharedGameServerApp()->SendMsgToDBServer(pBuffer,nLen);
}

CPlayer* CPlayerManager::GetPlayerByUserUID( unsigned int nUserUID )
{
	MAP_PLAYERS::iterator iter = m_vAllActivePlayers.find(nUserUID) ;
	if ( iter != m_vAllActivePlayers.end())
		return iter->second ;
	return NULL ;
}

void CPlayerManager::PushReserverPlayers( CPlayer* pPlayer )
{
	 if ( m_vAllReservePlayers.size() >= RESEVER_GAME_SERVER_PLAYERS )
	 {
		 delete pPlayer ;
		 return ;
	 }
	 m_vAllReservePlayers.push_back(pPlayer);
}

CPlayer* CPlayerManager::GetReserverPlayer()
{
	CPlayer* pPlayer = NULL ;
	LIST_PLAYERS::iterator iter = m_vAllReservePlayers.begin();
	if ( iter != m_vAllReservePlayers.end() )
	{
		CPlayer* pPlayer = *iter ;
		m_vAllReservePlayers.erase(iter) ;
		return pPlayer ;
	}
	pPlayer = new CPlayer(0) ;
	return pPlayer ;
}