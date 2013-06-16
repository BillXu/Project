#include "GatePeer.h"
#include "ServerNetwork.h"
#include "LogManager.h"
#include "ServerMessageDefine.h"
#include "GatePeerManager.h"
#define MAX_MSG_BUFFER 1024*4
char* CGatePeer::s_pBuffer = NULL ;
CGatePeer::CGatePeer()
{
	if ( s_pBuffer == NULL )
	{
		s_pBuffer = new char[MAX_MSG_BUFFER];
		memset(s_pBuffer,0,sizeof(char)*(MAX_MSG_BUFFER)) ;
	}
	m_ePeerType = eGatePeer_None ;
}

CGatePeer::~CGatePeer()
{

}

void CGatePeer::Init(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	assert(m_ePeerType != eGatePeer_None && "Please Set Peer Type " );
	Reset(nPeerUID,nSelfNetGUID);
}

void CGatePeer::Reset(unsigned int nPeerUID , RakNet::RakNetGUID& nSelfNetGUID)
{
	m_nSessionID = nPeerUID ;
	m_nSelfNetGUID = nSelfNetGUID ;
}

void CGatePeer::OnDisconnected()
{
	m_bRemove = true ;
	// put to remove queuer ;
	CGatePeerMgr::SharedGatePeerMgr()->RemovePeer(this) ;
}
