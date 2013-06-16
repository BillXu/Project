#include "GameServerPeer.h"
#include "ClientPeer.h"
#include "LogManager.h"
#include "ServerMessageDefine.h"
#include "ServerNetwork.h"
CGameServerPeer::CGameServerPeer()
{
	m_ePeerType = eGatePeer_GameServer ;
}

void CGameServerPeer::Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	CGatePeer::Init(nPeerUID,nSelfNetGUID) ;
	m_vClientPeers.clear() ;
}

void CGameServerPeer::OnMessage( RakNet::Packet* pData )
{
	// transfer this message 
	// parse message , then send it the specail client ;
	stMsgTransferData* pRealMsg = (stMsgTransferData*)pData->data ;
	CClientPeer* pClientPeer = GetClientPeer(pRealMsg->nTargetPeerUID);
	if ( pClientPeer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Can not find the peerUID = %d on the server %s " ,pRealMsg->nTargetPeerUID,m_nSelfNetGUID.ToString());
		return ;
	}

	if ( pClientPeer->IsDisconnected() )
	{
		CLogMgr::SharedLogMgr()->PrintLog("client lost state , don't send this message") ;
		return ;
	}

	char * pSendData = (char*)pData->data + sizeof(stMsgTransferData) ;
	if ( pRealMsg->bBroadCast == false )
	{
		CServerNetwork::SharedNetwork()->SendMsg(pSendData,pData->length - sizeof(stMsgTransferData),pClientPeer->GetSelfNetGUID(),false) ;
	}
	else
	{
		RakNet::RakNetGUID guidNone = RakNet::UNASSIGNED_RAKNET_GUID;
		CServerNetwork::SharedNetwork()->SendMsg(pSendData,pData->length - sizeof(stMsgTransferData),guidNone,true) ;
	}
}

void CGameServerPeer::OnDisconnected()
{
	CGatePeer::OnDisconnected();
	// tell all client peer;
	MAP_CLIENT_PEER::iterator iter = m_vClientPeers.begin() ;
	for ( ; iter != m_vClientPeers.end(); ++iter )
	{
		if ( iter->second != NULL )
		{
			(iter->second)->OnGameServerCrashed(this) ;
		}
	}
	m_vClientPeers.clear() ;
}

void CGameServerPeer::OnClientPeerDisconnected(CClientPeer* pPeer )
{
	if ( pPeer == NULL )
		return ;
	MAP_CLIENT_PEER::iterator iter = m_vClientPeers.find(pPeer->GetSessionID());
	if ( iter != m_vClientPeers.end() )
	{
		m_vClientPeers.erase(iter) ;

		stMsgPeerDisconnect msg ;
		msg.nSessionID = pPeer->GetSessionID() ;
		CServerNetwork::SharedNetwork()->SendMsg((char*)&msg,sizeof(msg),GetSelfNetGUID(),false) ;
	}
	else
	{
		CLogMgr::SharedLogMgr()->PrintLog("this Peer is not on this server ") ;
	}
}

CClientPeer* CGameServerPeer::GetClientPeer(unsigned int nSessionID )
{
	MAP_CLIENT_PEER::iterator iter = m_vClientPeers.find(nSessionID) ;
	if ( iter != m_vClientPeers.end() )
		return iter->second ;
	return NULL ;
}

void CGameServerPeer::AddClientPeer(CClientPeer* pClientPeer)
{
	if ( pClientPeer == NULL )
		return ;
	m_vClientPeers[pClientPeer->GetSessionID()] = pClientPeer ;
	// send message to server to add peer ;
}

