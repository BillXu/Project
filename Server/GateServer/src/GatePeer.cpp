#include "GatePeer.h"
#include "ServerNetwork.h"
#include "LogManager.h"
#include "ServerMessageDefine.h"
#define MAX_MSG_BUFFER 1024*4
char* CGatePeer::s_pBuffer = NULL ;
CGatePeer::CGatePeer(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID )
{
	Reset(nPeerUID,nSelfNetGUID);
	if ( s_pBuffer == NULL )
	{
		s_pBuffer = new char[MAX_MSG_BUFFER];
		memset(s_pBuffer,0,sizeof(char)*(MAX_MSG_BUFFER)) ;
	}
}

CGatePeer::~CGatePeer()
{

}

void CGatePeer::Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	m_nPeerUID = nPeerUID ;
	m_nSelfNetGUID = nSelfNetGUID ;
	m_bServer = false ;
	m_vClientOnThisServer.clear();
	m_pGameServerPeer = NULL ;
}

void CGatePeer::OnMessage( RakNet::Packet* pData )
{
	if ( IsServer())
	{
		// parse message , then send it the specail client ;
		stMsgTransferData* pRealMsg = (stMsgTransferData*)pData->data ;
		CGatePeer* pClientPeer = GetClientPeerOnThisByPeerUID(pRealMsg->nTargetPeerUID);
		if ( pClientPeer == NULL )
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "Can not find the peerUID = %d on the server %s " ,pRealMsg->nTargetPeerUID,m_nSelfNetGUID.ToString());
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
	else
	{
		// warapper the message , then send the message to the game server ;
		stMsgTransferData msgToSend ;
		msgToSend.cSysIdentifer = ID_MSG_GA2GM ;
		msgToSend.nTargetPeerUID = m_nPeerUID ;
		memcpy(s_pBuffer,(void*)&msgToSend,sizeof(msgToSend));
		memcpy(s_pBuffer + sizeof(msgToSend),pData->data,pData->length) ;
	    CServerNetwork::SharedNetwork()->SendMsg(s_pBuffer,pData->length + sizeof(stMsgTransferData),m_pGameServerPeer->GetSelfNetGUID(),false) ;
	}
}

void CGatePeer::OnDisconnected()
{
	if ( m_bServer ) // server crash down , tell all clients on this sever 
	{
		CGatePeerMgr::LIST_GATEPEER::iterator iter = m_vClientOnThisServer.begin();
		CGatePeer* pClient = NULL ;
		for ( ; iter != m_vClientOnThisServer.end(); ++iter )
		{
			pClient = *iter ;
			if ( pClient )
			{
				pClient->OnPeerDisconnect(this) ;
			}
		}
	}
	else  // tell the server that this client disconnect ; 
	{
		if ( m_pGameServerPeer )
		{
			m_pGameServerPeer->OnPeerDisconnect(this) ;
		}
	}
}

void CGatePeer::OnPeerDisconnect(CGatePeer* peer )
{
	assert(peer->IsServer() != IsServer() && "the same type can not inform each other" );
	if ( peer->IsServer() == IsServer() )
	{
		CLogMgr::SharedLogMgr()->PrintLog("the same type can not inform each other, one is %s , other is ",m_nSelfNetGUID.ToString(),peer->GetSelfNetGUID().ToString());
		return ;
	}

	if ( peer->IsServer() == false ) // an client of this server lost connect ;
	{
		// send msg to tell game server that this peer disconnect ;
		stMsg2GMPeerDisconnect msg ;
		msg.nPeerUID = peer->m_nPeerUID ;
		CServerNetwork::SharedNetwork()->SendMsg((char*)&msg,sizeof(msg),m_nSelfNetGUID,false) ;
		// remove client peer 
		CGatePeerMgr::LIST_GATEPEER::iterator iter = m_vClientOnThisServer.begin();
		for ( ; iter != m_vClientOnThisServer.end(); ++iter)
		{
			if ( *iter && (*iter)->m_nPeerUID == peer->m_nPeerUID )
			{
				m_vClientOnThisServer.erase(iter) ;
				break; 
			}
		}
	}
	else  // server crash , close connect of this client ;
	{
		CServerNetwork::SharedNetwork()->ClosePeerConnection(m_nSelfNetGUID);
	}
}

void CGatePeer::OnAddPeerToThisServer(CGatePeer* peer )
{
	assert(m_bServer && "must be server peer" );
	if ( !m_bServer )
	{
		CLogMgr::SharedLogMgr()->PrintLog("Client Peer Can not invoke this func = OnAddPeerToThisServer , this peer address = %s ",m_nSelfNetGUID.ToString()) ;
		return ;
	}
	m_vClientOnThisServer.push_back(peer) ;
}

CGatePeer* CGatePeer::GetClientPeerOnThisByPeerUID( unsigned int nPeerUID )
{
	CGatePeerMgr::LIST_GATEPEER::iterator iter = m_vClientOnThisServer.begin();
	for ( ; iter != m_vClientOnThisServer.end(); ++iter)
	{
		if ( *iter && (*iter)->m_nPeerUID == nPeerUID )
		{
			return (*iter) ;
		}
	}
	return NULL ;
}