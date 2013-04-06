#include "GatePeerManager.h"
#include "GatePeer.h"
#include "LogManager.h"
#include "ServerNetwork.h"
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
	CGatePeer* Peer = GetReserveGatePeer();
	if ( Peer )
	{
		Peer->Reset(GeneratePeerUID(),pData->guid) ;
	}
	m_vAllGatePeers[pData->guid] = Peer ;
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
	m_vAllGatePeers.erase(iter) ;
	pPeer->OnDisconnected();
	// if it server remove server 
	iter = m_vServerPeers.find(pData->guid) ;
	if ( iter != m_vServerPeers.end() )
	{
		m_vServerPeers.erase(iter) ;
	}

	AddToReservePeers(pPeer);
}

unsigned int CGatePeerMgr::GeneratePeerUID()
{
	static unsigned int g_sPeerUID = 1 ;
	return ++g_sPeerUID ;
}

bool CGatePeerMgr::AddPeerToServer(CGatePeer* pClientPeer )
{
	CGatePeer* pServerPeer = GetProperGameServerToAddClient();
	if ( pServerPeer == NULL )
	{
		CLogMgr::SharedLogMgr()->PrintLog("Don't have Proper GameServer To Join") ;
		return false ;
	}
	pClientPeer->SetGameServerNetGUID(pServerPeer->GetSelfNetGUID()) ;
	pServerPeer->OnAddPeerToThisServer(pClientPeer);
	return true ;
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
	pServer = iter->second ;
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