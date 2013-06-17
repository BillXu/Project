#include "LoginServerPeer.h"
#include "GatePeerManager.h"
#include "ServerMessageDefine.h"
#include "LogManager.h"
CLoginServerPeer::CLoginServerPeer()
{
	m_ePeerType = eGatePeer_LoginServer ;
	m_bActive = false ;
}

void CLoginServerPeer::OnMessage( RakNet::Packet* pData )
{
	stMsgTransferData* pRealMsg = (stMsgTransferData*)pData->data ;
	CClientPeer* pClientPeer = (CClientPeer*)m_pGatePeerMgr->GetAcitveClientPeerBySessionID(pRealMsg->nSessionID);
	if ( pClientPeer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Can not find the peerUID = %d on the server %s " ,pRealMsg->nSessionID,m_nSelfNetGUID.ToString());
		return ;
	}

	if ( pClientPeer->IsDisconnected() )
	{
		CLogMgr::SharedLogMgr()->PrintLog("client lost state , don't send this message") ;
		return ;
	}

	char * pSendData = (char*)pData->data + sizeof(stMsgTransferData) ;
	CServerNetwork::SharedNetwork()->SendMsg(pSendData,pData->length - sizeof(stMsgTransferData),pClientPeer->GetSelfNetGUID(),false) ;
}

void CLoginServerPeer::Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	CGatePeer::Reset(nPeerUID,nSelfNetGUID);
	if ( nPeerUID != 0 )
	{
		m_bActive = true ;
	}
}
