#include "GatePeerManager.h"
#include "GatePeer.h"
#include "LogManager.h"
#include "ServerNetwork.h"
#include "ServerMessageDefine.h"
#include "CommonDefine.h"
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
	stMsg* pMsg = (stMsg*)pData->data ;
	if ( pMsg->cSysIdentifer == ID_MSG_VERIFY && MSG_VERIFY_GMS == pMsg->usMsgType )
	{
		// confirm this peer is game Server ;
		CGatePeer* Peer = GetReserveGatePeer();
		if ( Peer )
		{
			Peer->Reset(GeneratePeerUID(),pData->guid) ;
		}
		else
		{
			Peer = new CGatePeer(GeneratePeerUID(),pData->guid) ;
		}
		m_vAllGatePeers[pData->guid] = Peer ;
		Peer->SetServer(true) ;
		m_vServerPeers[Peer->GetSelfNetGUID()] = Peer ;
		CLogMgr::SharedLogMgr()->PrintLog("A GameServer Entered : %s", pData->systemAddress.ToString(true) );
		return true;
	}
	else if ( pMsg->cSysIdentifer == ID_MSG_VERIFY && MSG_VERIFY_CLIENT == pMsg->usMsgType )
	{
#ifdef DEBUG
		CLogMgr::SharedLogMgr()->PrintLog("A Client Entered : %s",pData->systemAddress.ToString(true) );
#endif
		if ( m_vAllGatePeers.find(pData->guid) != m_vAllGatePeers.end())
		{
			CLogMgr::SharedLogMgr()->PrintLog("Don't verify Client More than once , IP:%s",pData->systemAddress.ToString(false));
			return true ;
		}
		CGatePeer* Peer = GetReserveGatePeer();
		if ( Peer )
		{
			Peer->Reset(GeneratePeerUID(),pData->guid) ;
		}
		else
		{
			Peer = new CGatePeer(GeneratePeerUID(),pData->guid) ;
		}
		m_vAllGatePeers[pData->guid] = Peer ;
		AddPeerToServer(Peer);
		return true;
	}

	MAP_GATEPEER::iterator iter = m_vAllGatePeers.find(pData->guid) ;
	if ( iter == m_vAllGatePeers.end() )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Receive unknown message , Address : %s ",pData->systemAddress.ToString(true)) ;
		CServerNetwork::SharedNetwork()->ClosePeerConnection(pData->guid) ;
		return true ;
	}
	iter->second->OnMessage(pData) ;
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
	MAP_GATEPEER::iterator iter = m_vAllGatePeers.find(pData->guid) ;
	if ( iter == m_vAllGatePeers.end() || iter->second == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Unknown peer Disconnected , info : %s",pData->systemAddress.ToString(true)) ;
		return ;
	}
	CGatePeer* pPeer = iter->second ;
	pPeer->OnDisconnected();
	RemovePeer(pPeer);
	CLogMgr::SharedLogMgr()->PrintLog("A peer Disconnected : IP = %s",pData->systemAddress.ToString(true) ) ;
}

unsigned int CGatePeerMgr::GeneratePeerUID()
{
	static unsigned int g_sPeerUID = 1 ;
	return ++g_sPeerUID ;
}

bool CGatePeerMgr::AddPeerToServer(CGatePeer* pClientPeer )
{
	stMsgConnectRet msg ;
	CGatePeer* pServerPeer = GetProperGameServerToAddClient();
	if ( pServerPeer == NULL )
	{
		msg.bOk = false ;
		msg.nErr = 1 ;
		CLogMgr::SharedLogMgr()->PrintLog("Don't have Proper GameServer To Join") ;
	}
	else
	{
		pClientPeer->SetGameServerPeer(pServerPeer) ;
		pServerPeer->OnAddPeerToThisServer(pClientPeer);
		msg.bOk = true ;
		msg.nErr = 0 ;
	}
	CServerNetwork::SharedNetwork()->SendMsg((char*)&msg,sizeof(msg),pClientPeer->GetSelfNetGUID(),false) ;
	return msg.bOk ;
}

int CGatePeerMgr::GetAllOnLinePeerCount()
{
	return m_vAllGatePeers.size() - m_vServerPeers.size() ;
}

CGatePeer* CGatePeerMgr::GetProperGameServerToAddClient()
{
	MAP_GATEPEER::iterator iter = m_vServerPeers.begin();
	CGatePeer* pServer = NULL ;
	for ( ; iter != m_vServerPeers.end(); ++iter )
	{
		pServer = iter->second ;
		if ( pServer->GetOwnPlayers() >= SERVER_IDEAL_PLAYER_COUNT )
		{
			continue;
		}
		else
		{
			return pServer ;
		}
	}

	// find the idle server to join 
	iter = m_vServerPeers.begin() ;
	if ( iter != m_vServerPeers.end() )
		pServer = iter->second ;
	else
		return NULL ;
	for ( ; iter != m_vServerPeers.end(); ++iter )
	{
		if ( pServer->GetOwnPlayers() > iter->second->GetOwnPlayers() )
		{
			pServer = iter->second ;
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
	ClearPeers(m_vAllGatePeers);
	LIST_GATEPEER::iterator iter = m_vReservePeers.begin() ;
	for ( ; iter != m_vReservePeers.end(); ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vReservePeers.clear() ;
}

CGatePeer* CGatePeerMgr::GetReserveGatePeer()
{
	CGatePeer* pPeer = NULL ;
	LIST_GATEPEER::iterator iter = m_vReservePeers.begin() ;
	if ( iter == m_vReservePeers.end() )
		return NULL ;
	pPeer = *iter ;
	m_vReservePeers.erase(iter) ;
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
	if ( !pPeer)
		return ;
	MAP_GATEPEER::iterator iter = m_vServerPeers.find(pPeer->GetSelfNetGUID()) ;
	if ( iter != m_vServerPeers.end() )
	{
		m_vServerPeers.erase(iter) ;
	}
	
	iter = m_vAllGatePeers.find(pPeer->GetSelfNetGUID()) ;
	if ( iter != m_vAllGatePeers.end() )
	{
		m_vAllGatePeers.erase(iter) ;
	}
	AddToReservePeers(pPeer) ;
}