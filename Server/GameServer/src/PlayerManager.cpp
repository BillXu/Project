#include "PlayerManager.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
#include "Player.h"
#include "CommonDefine.h"
char* CPlayerManager::s_pBuffer = NULL ;
CPlayerManager* CPlayerManager::SharedPlayerMgr()
{
	static CPlayerManager g_sPlayerMgr ;
	return &g_sPlayerMgr ;
}

CPlayerManager::CPlayerManager()
{
	if ( s_pBuffer == NULL )
	{
		s_pBuffer = new char[MAX_MSG_BUFFER_LEN] ;
	}
	m_bGateServerConnected = m_bDBserverConnected = false ;
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
	if ( pMessage->cSysIdentifer ==  ID_MSG_GA2GM  || ( ID_MSG_VERIFY == pMessage->cSysIdentifer && pMessage->usMsgType == MSG_VERIFY_GA))
	{
		processMsgFromGateServer(pMessage,pMsg) ;
	}
	else if ( ID_MSG_DB2GM == pMessage->cSysIdentifer || ID_MSG_VERIFY == pMessage->cSysIdentifer && pMessage->usMsgType == MSG_VERIFY_DB )
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
	if ( MSG_VERIFY_DB == pMessage->usMsgType )
	{
		m_bDBserverConnected = true ;
		m_nDBServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to DBServer : %s ", pMsg->systemAddress.ToString());
		return  ;
	}
	else if ( MSG_TRANSER_DATA == pMessage->usMsgType ) // game server don't prcess tranfer msg from DBServer ;
	{
		stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMessage ;
		stMsg* pTargetMessage = (stMsg*)(pMsg->data + sizeof(stMsgTransferData));
		pMsgTransfer->cSysIdentifer = ID_MSG_GM2GA ;
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
	if ( pMessage->usMsgType == MSG_VERIFY_GA )
	{
		m_bGateServerConnected = true ;
		m_nGateServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to gate Server : %s ", pMsg->systemAddress.ToString());

		// verify self 
		stMsg msg ;
		msg.cSysIdentifer = ID_MSG_VERIFY ;
		msg.usMsgType = MSG_VERIFY_GMS ;
		CNetWorkMgr::SharedNetWorkMgr()->SendMsg((char*)&msg,sizeof(msg),pMsg->guid) ;
		return  ;
	}
	else if ( MSG_TRANSER_DATA == pMessage->usMsgType )
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

bool CPlayerManager::OnLostSever(RakNet::Packet* pMsg)
{
	bool bGateDown = false ;
	if ( pMsg->guid == m_nGateServerNetUID )
	{
		bGateDown = true ;
		m_bGateServerConnected = false ;
	}
	else
	{
		m_bDBserverConnected = false ;
	}

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
				SendMsgToDBServer((char*)pMsg->data,pMsg->length) ;
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

void CPlayerManager::SendMsgToDBServer( const char* pBuffer , int nLen )
{
	if ( m_bDBserverConnected == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("can not send msg to gate , because DBServer is not connecting !") ;
		return ;
	}
	CNetWorkMgr::SharedNetWorkMgr()->SendMsg(pBuffer,nLen,m_nDBServerNetUID) ;
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