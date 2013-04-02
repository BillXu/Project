#include "ServerNetwork.h"
void CServerNetworkDelegate::SetPriority(unsigned int nPriority )
{
	if ( nPriority == m_nPriority )
		return ;
	CServerNetwork::SharedNetwork()->RemoveDelegate(this);
	CServerNetwork::SharedNetwork()->AddDelegate(this,nPriority);
}

CServerNetwork* CServerNetwork::SharedNetwork()
{
	static CServerNetwork g_sNetwork ;
	return &g_sNetwork ;
}

CServerNetwork::CServerNetwork()
{
	m_pNetPeer = NULL;
	m_vAllDelegates.clear() ;
}

CServerNetwork::~CServerNetwork()
{
	ShutDown();
}

bool CServerNetwork::StartupNetwork( unsigned short nPort , int nMaxInComming )
{

}

void CServerNetwork::ShutDown()
{
	if ( m_pNetPeer == NULL)
		return ;
	m_vAllDelegates.clear() ;
	m_pNetPeer->Shutdown(2);
	RakNet::RakPeerInterface::DestroyInstance(m_pNetPeer);
	m_pNetPeer = NULL ;
}

void CServerNetwork::RecieveMsg()
{

}

void CServerNetwork::SendMsg(const char* pData , int nLength , RakNet::RakNetGUID& nSendToOrExcpet ,bool bBroadcast )
{

}

void CServerNetwork::ClosePeerConnection(RakNet::RakNetGUID& nPeerToClose)
{
	if ( !m_pNetPeer )
		return ;
	m_pNetPeer->CloseConnection(nPeerToClose,true);
	// tell delegate ;
}

void CServerNetwork::AddDelegate(CServerNetworkDelegate* pDelegate , unsigned int nPriority /* = 0 */ )
{
	
}

void CServerNetwork::RemoveDelegate(CServerNetworkDelegate* pDelegate )
{

}
