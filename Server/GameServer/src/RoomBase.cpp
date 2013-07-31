#include "RoomBase.h"
#include <assert.h>
#include "RoomPeer.h"
#include "Player.h"
#include "MessageDefine.h"
CRoomBase::CRoomBase()
{
	m_eRoomType = eRoom_Max ;
	m_nRoomID = 0 ;
	m_vRoomPeer = NULL ;
	m_nMaxSeat = 0 ;
}

CRoomBase::~CRoomBase()
{
	delete[] m_vRoomPeer ;
}

void CRoomBase::Init( unsigned int nRoomID , unsigned char nMaxSeat )
{
	assert(m_eRoomType !=eRoom_Max && "Please assign m_eRoomtype" ) ;
	m_nRoomID = nRoomID ;
	m_nMaxSeat = nMaxSeat ;
	m_eRoomState = eRoomState_PeerToJoin ;
	m_vRoomPeer = new CRoomPeer*[nMaxSeat];
	for ( int i = 0 ; i < nMaxSeat ; ++i )
	{
		m_vRoomPeer[i] = NULL ;
	}
}

void CRoomBase::SendMsgRoomPeers(stMsg*pMsg ,unsigned short nLen )
{
	CRoomPeer* pPeer = NULL ;
	for ( int i = 0; i < GetMaxSeat(); ++i )
	{
		pPeer = m_vRoomPeer[i];
		if ( !pPeer )
		{
			continue; 
		}
		pPeer->SendMsgToClient((char*)pMsg,nLen);
	}
}

CRoomPeer* CRoomBase::GetRoomPeerBySessionID( unsigned int nSessionID )
{
	for ( int i = 0 ; i < GetMaxSeat(); ++i )
	{
		if ( m_vRoomPeer[i] && m_vRoomPeer[i]->GetPlayer()->GetSessionID() == nSessionID )
		{
			return m_vRoomPeer[i] ;
		}
	}
	return NULL ;
}

int CRoomBase::GetRoomPeerCount()
{
	int ncount = 0 ;
	for ( int i = 0  ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] == NULL )
		{
			continue; 
		}
		++ncount ;
	}
	return ncount ;
}

void CRoomBase::OnPeerLeave( CRoomPeer* peer ) 
{
	// send Leave msg ;
	stMsgRoomPlayerLeave msg ;
	msg.nSessionID = peer->GetPlayer()->GetSessionID() ;
	SendMsgRoomPeers(&msg,sizeof(msg)) ;
}

bool CRoomBase::CanJoin( CRoomPeer* peer )
{
	if ( GetEmptySeatCount() <= 0 )
		return false ;
	return true ;
}

void CRoomBase::Update(float fTimeElpas, unsigned int nTimerID )
{
	
}

bool CRoomBase::OnPeerMsg(CRoomPeer* pPeer, stMsg* pmsg )
{
	// process common msg that don't assocaite with specail card game ;
	// for example : speak work , look other player info ,and make firend , and so on ;
	// when processed , return true , other case return false ;
	return false ;
}

char CRoomBase::GetRoomPeerIdx(CRoomPeer* pPeer )
{
	for ( int i = 0  ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] == pPeer )
		{
			return  i ;
		}
	}
	return -1 ;
}