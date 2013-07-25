#include "ClientManager.h"
#include "GateClient.h"
#include "MessageDefine.h"
#include "LogManager.h"
#include "CommonDefine.h"
#include "ServerNetwork.h"
CGateClientMgr::CGateClientMgr()
{
	m_vNetWorkIDGateClient.clear();
	m_vSessionGateClient.clear() ;
	m_vGateClientReserver.clear();
}

CGateClientMgr::~CGateClientMgr()
{
	MAP_SESSIONID_GATE_CLIENT::iterator iterS = m_vSessionGateClient.begin();
	for ( ; iterS != m_vSessionGateClient.end(); ++iterS )
	{
		delete  iterS->second ;
	}
	m_vSessionGateClient.clear() ;

	// just clear ; object deleted in session Gate ;
	m_vNetWorkIDGateClient.clear() ;

	LIST_GATE_CLIENT::iterator iter = m_vGateClientReserver.begin() ;
	for ( ; iter != m_vGateClientReserver.end(); ++iter )
	{
		delete *iter ;
	}
	m_vGateClientReserver.clear() ;
}

bool CGateClientMgr::OnMessage( RakNet::Packet* pData )
{
	// verify identify 
	stMsg* pMsg = (stMsg*)pData->data ;
	CHECK_MSG_SIZE(stMsg,pData->length);
	if ( pMsg->cSysIdentifer == ID_MSG_VERIFY )
	{
		if ( MSG_VERIFY_GAME == pMsg->usMsgType )
		{
			m_nGameServerNetWorkID = pData->guid ;
		}
		else if ( MSG_VERIFY_LOGIN == pMsg->usMsgType )
		{
			m_nLoginServerNetWorkID = pData->guid ;
		}
		else if ( MSG_VERIFY_CLIENT == pMsg->usMsgType )
		{
			stGateClient* pGateClient = GetReserverGateClient();
			if ( !pGateClient )
			{
				pGateClient = new stGateClient ;
			}
			pGateClient->Reset(0,pData->guid) ;
		}
		else
		{
			CLogMgr::SharedLogMgr()->SystemLog("Unknown identify from ip = %s",pData->systemAddress.ToString(true) ) ;
		}
		return true;
	}

	// client reconnect ;
	if ( MSG_RECONNECT == pMsg->usMsgType )
	{
		stMsgReconnect* pRet = (stMsgReconnect*)pMsg ;
		CHECK_MSG_SIZE(stMsgReconnect,pData->length);
		stGateClient* pGateClient = GetGateClientBySessionID(pRet->nSessionID) ;
		if ( pGateClient )
		{
			stGateClient* pNew = GetGateClientByNetWorkID(pData->guid);
			if ( pNew )
			{
				RemoveClientGate(pNew) ;
			}

			// update network id ;
			m_vNetWorkIDGateClient.erase(m_vNetWorkIDGateClient.find(pGateClient->nNetWorkID)) ;
			pGateClient->SetNewWorkID(pData->guid) ;
			m_vNetWorkIDGateClient[pGateClient->nNetWorkID] = pGateClient ;
		}
		stMsgReconnectRet msgback ;
		msgback.nRet = (pGateClient == NULL ? 1 : 0 ) ;
		// send msg to client ;
		return ;
	}

	// distribute msg 

}

void CGateClientMgr::OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
{
	stMsg msg ;
	msg.cSysIdentifer = ID_MSG_VERIFY ;
	msg.usMsgType = MSG_VERIFY_GATE ;
	// send msg to peer ;
}

void CGateClientMgr::OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
{

}

void CGateClientMgr::AddClientGate(stGateClient* pGateClient )
{
	m_vNetWorkIDGateClient[pGateClient->nNetWorkID] = pGateClient ;
	m_vSessionGateClient[pGateClient->nSessionId] = pGateClient ;
}

void CGateClientMgr::RemoveClientGate(stGateClient* pGateClient )
{
	if ( pGateClient == NULL )
		return ;
	pGateClient->Reset(0,RakNet::UNASSIGNED_NETWORK_ID) ;
	m_vGateClientReserver.push_back(pGateClient) ;
	m_vNetWorkIDGateClient.erase(m_vNetWorkIDGateClient.find(pGateClient->nNetWorkID)) ;
	m_vSessionGateClient.erase(m_vSessionGateClient.find(pGateClient->nSessionId)) ;
}

stGateClient* CGateClientMgr::GetReserverGateClient()
{
	stGateClient* pGateClient = NULL ;
	if ( m_vGateClientReserver.empty() == false )
	{
		pGateClient = m_vGateClientReserver.front() ;
		m_vGateClientReserver.erase(m_vGateClientReserver.begin()) ;
	}
	return pGateClient ;
}

stGateClient* CGateClientMgr::GetGateClientBySessionID(unsigned int nSessionID)
{
	MAP_SESSIONID_GATE_CLIENT::iterator iter = m_vSessionGateClient.find(nSessionID) ;
	if ( iter == m_vSessionGateClient.end() )
		return NULL ;
	return iter->second ;
}
