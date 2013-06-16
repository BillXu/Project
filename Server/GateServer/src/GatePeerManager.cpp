#include "GatePeerManager.h"
#include "GatePeer.h"
#include "LogManager.h"
#include "ServerNetwork.h"
#include "ServerMessageDefine.h"
#include "CommonDefine.h"
#include "GameServerPeer.h"
#define SERVER_IDEAL_PLAYER_COUNT 2000
#define RESERVE_PEER_COUN 200
CGatePeerMgr* CGatePeerMgr::SharedGatePeerMgr()
{
	static CGatePeerMgr g_sGatePeerMgr ;
	return &g_sGatePeerMgr ;
}

CGatePeerMgr::CGatePeerMgr()
{
	ClearAll();
}

CGatePeerMgr::~CGatePeerMgr()
{
	ClearAll();
}

bool CGatePeerMgr::OnMessage( RakNet::Packet* pData )
{
	if ( ProcessGateLogicMsg(pData ) )
	{
		return true ;
	}

	CGatePeer* pMeessagePeer = GetAcitvePeer(pData->guid) ;
	if ( pMeessagePeer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Receive unknown message , Address : %s ",pData->systemAddress.ToString(true)) ;
		CServerNetwork::SharedNetwork()->ClosePeerConnection(pData->guid) ;
		return true ;
	}
	pMeessagePeer->OnMessage(pData) ;
	return false ;
}

void CGatePeerMgr::OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
{
	// send veryfiy msg ;
	stMsg msg ;
	msg.cSysIdentifer = ID_MSG_VERIFY ;
	msg.usMsgType = MSG_VERIFY_GA ;
	CServerNetwork::SharedNetwork()->SendMsg((char*)&msg,sizeof(msg),nNewPeer,false) ;
	CLogMgr::SharedLogMgr()->PrintLog("A Peer connected IP = %s",pData->systemAddress.ToString(true)) ;
}

void CGatePeerMgr::OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
{
	CGatePeer* pMeessagePeer = GetAcitvePeer(pData->guid) ;
	if ( pMeessagePeer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Unknown peer Disconnected , info : %s",pData->systemAddress.ToString(true)) ;
		return ;
	}
	pMeessagePeer->OnDisconnected();
	CLogMgr::SharedLogMgr()->PrintLog("A peer Disconnected : IP = %s",pData->systemAddress.ToString(true) ) ;
}

unsigned int CGatePeerMgr::GenerateSessionID()
{
	static unsigned int g_sPeerUID = 1 ;
	return ++g_sPeerUID ;
}

bool CGatePeerMgr::AddPeerToServer(CClientPeer* pClientPeer )
{
	CGameServerPeer* pServerPeer = (CGameServerPeer*)GetProperGameServerToAddClient();
	if ( pServerPeer == NULL )
	{
		CLogMgr::SharedLogMgr()->PrintLog("Don't have Proper GameServer To Join") ;
		return false ;
	}
	else
	{
		pClientPeer->JoinToServer(pServerPeer) ;
		pServerPeer->AddClientPeer(pClientPeer);
		return true ;
	}
	return false ;
}

int CGatePeerMgr::GetAllOnLinePeerCount()
{
	return m_vClientPeers.size();
}

CGatePeer* CGatePeerMgr::GetProperGameServerToAddClient()
{
	MAP_GATEPEER::iterator iter = m_vGameServerPeers.begin();
	CGameServerPeer* pServer = NULL ;
	for ( ; iter != m_vGameServerPeers.end(); ++iter )
	{
		pServer = (CGameServerPeer*)iter->second ;
		if ( pServer->GetClientPeerCount() >= SERVER_IDEAL_PLAYER_COUNT )
		{
			continue;
		}
		else
		{
			return pServer ;
		}
	}

	// find the idle server to join 
	iter = m_vGameServerPeers.begin() ;
	if ( iter != m_vGameServerPeers.end() )
		pServer = (CGameServerPeer*)iter->second ;
	else
		return NULL ;
	for ( ; iter != m_vGameServerPeers.end(); ++iter )
	{
		if ( pServer->GetClientPeerCount() > ((CGameServerPeer*)iter->second)->GetClientPeerCount() )
		{
			pServer = (CGameServerPeer*)iter->second ;
			continue; 
		}
	}
	return pServer ;
}

void  CGatePeerMgr::ClearPeers(MAP_GATEPEER&vPeers)
{
	MAP_GATEPEER::iterator iter = vPeers.begin();
	for ( ; iter != vPeers.end(); ++iter )
	{
		if ( iter->second )
		{
			delete iter->second ;
			iter->second = NULL ;
		}
	}
	vPeers.clear() ;
}

void CGatePeerMgr::ClearAll()
{
	ClearPeers(m_vGameServerPeers);
	ClearPeers(m_vClientPeers);
	MAP_UID_CLIENT_PEER::iterator iter = m_vWaitReconnectedClientPeers.begin() ;
	for ( ; iter != m_vWaitReconnectedClientPeers.end(); ++iter )
	{
		if ( iter->second )
		{
			delete iter->second ;
			iter->second = NULL ;
		}
	}
	m_vWaitReconnectedClientPeers.clear() ;
}

CGatePeer* CGatePeerMgr::GetReserveGatePeer(CGatePeer::eGatePeerType eType)
{
	CGatePeer* pPeer = NULL ;
	LIST_GATEPEER::iterator iter = m_vReservePeers.begin() ;
	for ( ; iter != m_vReservePeers.end(); ++iter )
	{
		if ( *iter != NULL && (*iter)->GetPeerType() == eType )
		{
			pPeer = *iter ;
			m_vReservePeers.erase(iter) ;
			return pPeer ;
		}
	}
	return pPeer ;
}

void CGatePeerMgr::AddToReservePeers(CGatePeer* pPeer )
{
	if ( m_vReservePeers.size() >= RESERVE_PEER_COUN )
	{
		delete pPeer ;
		return ;
	}
	m_vReservePeers.push_back(pPeer) ;
}

void CGatePeerMgr::RemovePeer(CGatePeer* pPeer )
{
	m_vWillRemove.push_back(pPeer) ;
}

void CGatePeerMgr::Update(float fTimeElsps )
{
	// process m_vWillRemove ;
	LIST_GATEPEER::iterator iter = m_vWillRemove.begin() ;
	CGatePeer* pPeer = NULL ;
	bool bFind = false ;
	for ( ; iter != m_vWillRemove.end(); ++iter )
	{
		bFind = false  ;
		pPeer =  *iter ;
		assert(pPeer->IsRemove() && "this peer may not shouldn't remove !" );
		if ( pPeer->IsRemove() == false )
		{
			CLogMgr::SharedLogMgr()->ErrorLog("this peer may not shouldn't remove !");
		}

		if ( pPeer->GetPeerType() == CGatePeer::eGatePeer_Client )
		{
			MAP_GATEPEER::iterator iter_map = m_vClientPeers.begin();
			for ( ; iter_map != m_vClientPeers.end(); ++iter_map )
			{
				if ( pPeer == iter_map->second )
				{
					AddToReservePeers(pPeer) ;
					m_vClientPeers.erase(iter_map) ;
					bFind = true ;
					break; 
				}
			}
		}
		else if ( pPeer->GetPeerType() == CGatePeer::eGatePeer_GameServer )
		{
			MAP_GATEPEER::iterator iter_map = m_vGameServerPeers.begin();
			for ( ; iter_map != m_vGameServerPeers.end(); ++iter_map )
			{
				if ( pPeer == iter_map->second )
				{
					AddToReservePeers(pPeer) ;
					m_vGameServerPeers.erase(iter_map) ;
					bFind = true ;
					break; 
				}
			}
		}

		// wait reconnected 
		MAP_UID_CLIENT_PEER::iterator iter_UID = m_vWaitReconnectedClientPeers.begin();
		for ( ; iter_UID != m_vWaitReconnectedClientPeers.end(); ++iter_UID )
		{
			if (  pPeer == iter_UID->second )
			{
				bFind = true ;
				AddToReservePeers(pPeer) ;
				m_vWaitReconnectedClientPeers.erase(iter_UID) ;
				break; 
			}
		}
	}
	m_vWillRemove.clear() ;
}

CGatePeer* CGatePeerMgr::GetAcitvePeer(RakNet::RakNetGUID& nNetUID )
{
	MAP_GATEPEER::iterator iter = m_vGameServerPeers.find(nNetUID) ;
	if ( iter != m_vGameServerPeers.end() )
	{
		return iter->second ;
	}

	iter = m_vClientPeers.find(nNetUID);
	if ( iter != m_vGameServerPeers.end() )
	{
		return iter->second ;
	}
	return NULL ;
}

void CGatePeerMgr::AddNewPeer(CGatePeer* pNewPeer )
{
	CGatePeer* pOld = GetAcitvePeer(pNewPeer->GetSelfNetGUID() );
	assert(pOld == NULL && "add one peer more than once !" );
	if ( pOld )
	{
		RemovePeer(pNewPeer) ;
		CLogMgr::SharedLogMgr()->ErrorLog("add one peer more than once !, IP = %s",pNewPeer->GetSelfNetGUID().ToString());
		return ;
	}

	if ( pNewPeer->GetPeerType() == CGatePeer::eGatePeer_Client )
	{
		m_vClientPeers[pNewPeer->GetSelfNetGUID()] = pNewPeer ;	
	}
	else if ( pNewPeer->GetPeerType() == CGatePeer::eGatePeer_GameServer )
	{
		m_vGameServerPeers[pNewPeer->GetSelfNetGUID()] = pNewPeer ;
	}
	else
	{
		assert( 0 && "UNknow peer type" );
	}
}

void CGatePeerMgr::AddWaitForReconnected(CClientPeer* peerWait)
{
	if ( !peerWait )
		return ;

	MAP_UID_CLIENT_PEER::iterator iter = m_vWaitReconnectedClientPeers.find(peerWait->GetPlayerUID()) ;
	if ( iter != m_vWaitReconnectedClientPeers.end() )
	{
		assert(0 &&"cannot add twice ");
		return ;
	}
	m_vWaitReconnectedClientPeers[peerWait->GetPlayerUID()] =  peerWait ;
}

bool CGatePeerMgr::ProcessGateLogicMsg(RakNet::Packet* pData)
{
	stMsg* pMsg = (stMsg*)pData->data ;
	if ( pMsg->cSysIdentifer == ID_MSG_VERIFY && MSG_VERIFY_GMS == pMsg->usMsgType )
	{
		// confirm this peer is game Server ;
		CGatePeer* Peer = GetReserveGatePeer(CGatePeer::eGatePeer_GameServer);
		if ( Peer )
		{
			Peer->Reset(GenerateSessionID(),pData->guid) ;
		}
		else
		{
			Peer = new CGameServerPeer() ;
			Peer->Init(GenerateSessionID(),pData->guid);
		}
		AddNewPeer(Peer) ;
		CLogMgr::SharedLogMgr()->PrintLog("A GameServer Entered : %s", pData->systemAddress.ToString(true) );
		return true;
	}
	else if ( pMsg->cSysIdentifer == ID_MSG_VERIFY && MSG_VERIFY_CLIENT == pMsg->usMsgType )
	{
#ifdef DEBUG
		CLogMgr::SharedLogMgr()->PrintLog("A Client Entered : %s",pData->systemAddress.ToString(true) );
		if ( m_vClientPeers.find(pData->guid) != m_vAllGatePeers.end())
		{
			CLogMgr::SharedLogMgr()->PrintLog("Don't verify Client More than once , IP:%s",pData->systemAddress.ToString(false));
			return true ;
		}
#endif
		CGatePeer* Peer = GetReserveGatePeer(CGatePeer::eGatePeer_Client);
		if ( Peer )
		{
			Peer->Reset(GenerateSessionID(),pData->guid) ;
		}
		else
		{
			Peer = new CClientPeer;
			Peer->Init(GenerateSessionID(),pData->guid) ;
		}
		AddNewPeer(Peer);
		return true;
	}
	else if ( MSG_RECONNECT == pMsg->usMsgType && ID_MSG_S2C == pMsg->cSysIdentifer )
	{
		stMsgReconnectRet msgBack ;
		stMsgReconnect* pRetMsg = (stMsgReconnect*)pMsg ;
		MAP_UID_CLIENT_PEER::iterator iter = m_vWaitReconnectedClientPeers.find(pRetMsg->nPlayerUID);
		if ( iter->second )
		{
			iter->second->SetNewSelfNetUID(pData->guid) ;
			RemovePeer(GetAcitvePeer(pData->guid)) ;
			AddNewPeer(iter->second) ;
			m_vWaitReconnectedClientPeers.erase(iter) ;
			msgBack.bSuccess = true ;
		}
		else
		{
			msgBack.bSuccess = false ;
		}
		CServerNetwork::SharedNetwork()->SendMsg((char*)&msgBack,sizeof(msgBack),pData->guid,false) ;
	}
	else if (  MSG_UID_LOGIN == pMsg->usMsgType && ID_MSG_S2C == pMsg->cSysIdentifer  )
	{
		stMsgUIDLogin* pMsgRet = (stMsgUIDLogin*)pMsg ;

		stMsgUIDLoginRet MsgBack ;
		CClientPeer* pClientPeer = (CClientPeer*)GetAcitvePeer(pData->guid) ;
		assert(pClientPeer && "UID Login , client peer is NULL " ) ;
		if ( !pClientPeer )
		{
			CLogMgr::SharedLogMgr()->ErrorLog("UID login client peer = null , ip = %s",pData->systemAddress.ToString(false));
			MsgBack.nRet = 1 ;
		}
		else if ( AddPeerToServer(pClientPeer) )
		{
			MsgBack.nRet = 0 ;
			// send msg to server to tell crate new player ;
			stMsgPlayerUIDLogin msgToGameServer ;
			msgToGameServer.nPlayerUID = pMsgRet->nPlayerUID ;
			msgToGameServer.nSessionID = pClientPeer->GetSessionID() ;
			CServerNetwork::SharedNetwork()->SendMsg((char*)&msgToGameServer,sizeof(msgToGameServer),pClientPeer->GetGameServerPeer()->GetSelfNetGUID(),false) ;
		}
		else
		{
			MsgBack.nRet = 2 ;
		}
		CServerNetwork::SharedNetwork()->SendMsg((char*)&MsgBack,sizeof(MsgBack),pData->guid,false) ;
	}
	return false ;
}
