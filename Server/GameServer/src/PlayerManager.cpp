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
	if ( pMessage->cSysIdentifer ==  ID_MSG_GA2GM && pMessage->usMsgType == MSG_VERIFY_GA )
	{
		m_bGateServerConnected = true ;
		m_nGateServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to gate Server : %s ", m_nGateServerNetUID.ToString());
		return true ;
	}
	else if ( pMessage->cSysIdentifer == ID_MSG_DB2GM && MSG_VERIFY_DB == pMessage->usMsgType )
	{
		m_bDBserverConnected = true ;
		m_nDBServerNetUID = pMsg->guid ;
		CLogMgr::SharedLogMgr()->PrintLog("Success connected to DBServer : %s ", m_nDBServerNetUID.ToString());
		return true ;
	}
	else if ( pMessage->cSysIdentifer == ID_MSG_GA2GM && MSG_TRANSER_DATA == pMessage->usMsgType )
	{
		stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMessage ;
		CPlayer* pTargetPlayer = GetPlayerByUserUID(pMsgTransfer->nTargetPeerUID) ;
		if ( !pTargetPlayer )
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Can not find target Player, so message From Gate will not be processed" );
			return true ;
		}
		stMsg* pTargetMessage = (stMsg*)(pMsg->data + sizeof(stMsgTransferData));
		pTargetPlayer->OnMessage(pTargetMessage);
	}
	else if ( ID_MSG_DB2GM == pMessage->cSysIdentifer )
	{
		stMsgDB2GM* pMsgFromDB = (stMsgDB2GM*)pMessage ;
		CPlayer* pTargetPlayer = GetPlayerByUserUID(pMsgFromDB->nTargetUserUID ) ;
		if ( !pTargetPlayer )
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Can not find target Player, so message From DBServer will not be processed" );
			return true ;
		}
		pTargetPlayer->OnMessage(pMessage);
	}
	else
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Receive Unknown message cSysIdentifer = %d ,usMsgType = %d ",pMessage->cSysIdentifer,pMessage->usMsgType ) ;
	}
	return false ;
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
			}
			else 
			{
				iter->second->OnDBServerLost();
			}
		}
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

void CPlayerManager::SendMsgToDBServer( unsigned int nUserUID , const char* pBuffer , int nLen )
{

}

CPlayer* CPlayerManager::GetPlayerByUserUID( unsigned int nUserUID )
{
	MAP_PLAYERS::iterator iter = m_vAllActivePlayers.find(nUserUID) ;
	if ( iter != m_vAllActivePlayers.end())
		return iter->second ;
	return NULL ;
}