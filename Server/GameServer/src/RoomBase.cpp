#include "RoomBase.h"
#include <assert.h>
#include "RoomPeer.h"
#include "Player.h"
#include "MessageDefine.h"
CRoomBase::CRoomBase()
{
	m_eRoomType = eRoom_Max ;
	m_nRoomID = 0 ;
	m_vRoomPeer.clear() ;
	m_nMaxSeat = 0 ;
}

void CRoomBase::Init( unsigned int nRoomID , unsigned char nMaxSeat )
{
	assert(m_eRoomType !=eRoom_Max && "Please assign m_eRoomtype" ) ;
	m_nRoomID = nRoomID ;
	m_nMaxSeat = nMaxSeat ;
}

void CRoomBase::SendMsgRoomPeers(stMsg*pMsg ,unsigned short nLen )
{
	LIST_ROOM_PEER::iterator iter = m_vRoomPeer.begin() ;
	CRoomPeer* pPeer = NULL ;
	for ( ; iter != m_vRoomPeer.end(); ++iter )
	{
		pPeer = *iter ;
		if ( !pPeer )
		{
			continue; 
		}
		pPeer->SendMsgToClient((char*)pMsg,nLen);
	}
}

CRoomPeer* CRoomBase::GetRoomPeerBySessionID( unsigned int nSessionID )
{
	LIST_ROOM_PEER::iterator iter = m_vRoomPeer.begin() ;
	CRoomPeer* pPeer = NULL ;
	for ( ; iter != m_vRoomPeer.end(); ++iter )
	{
		pPeer = *iter ;
		if ( !pPeer )
		{
			continue; 
		}
		if ( pPeer->GetPlayer()->GetSessionID() == nSessionID )
		{
			return pPeer ;
		}
	}
	return NULL ;
}

void CRoomBase::OnPeerLeave( CRoomPeer* peer ) 
{
	LIST_ROOM_PEER::iterator iter = m_vRoomPeer.begin() ;
	CRoomPeer* pPeer = NULL ;
	for ( ; iter != m_vRoomPeer.end(); ++iter )
	{
		pPeer = *iter ;
		if ( pPeer == peer )
		{
			m_vRoomPeer.erase(iter) ;
			break; 
		}
	}
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