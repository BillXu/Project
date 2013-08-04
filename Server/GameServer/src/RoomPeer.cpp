#include "RoomPeer.h"
#include "Player.h"
#include "Room.h"
#include "LogManager.h"
#include "GameServerApp.h"
CRoomPeer::CRoomPeer(CPlayer* pPlayer )
	:IPlayerComponent(pPlayer)
{
	m_eType = ePlayerComponent_RoomPeer ;
	m_pRoom = NULL ;
	m_nPeerIdx = 0 ;
	Reset();
}

CRoomPeer::~CRoomPeer()
{

}

bool CRoomPeer::OnMessage(stMsg* pMsg )
{
	if ( MSG_ROOM_ENTER == pMsg->usMsgType )
	{
		stMsgRoomEnter msgEnter ;
		CRoom* pRoom = CGameServerApp::SharedGameServerApp()->GetRoomMgr()->GetRoom(msgEnter.nRoomType,msgEnter.nRoomLevel) ;
		if ( pRoom->CanJoin(this))
		{
			stMsgRoomEnterRet msgRet ;
			msgRet.nRet = 0 ;
			msgRet.nRoomID = pRoom->GetRoomID();
			SendMsgToClient((char*)&msgRet,sizeof(msgRet)) ;
			// sent cur room info to client ;
			pRoom->SendCurRoomToPeer(this);
			m_pRoom = pRoom ;
			pRoom->AddPeer(this) ;
		}
		// find a room to enter ;
		return true ;
	}
	if ( m_pRoom )
	{
		return m_pRoom->OnPeerMsg(this,pMsg) ;
	}
	return false ;
}


void CRoomPeer::OnPlayerDisconnect()
{
	LeaveRoom();
}

void CRoomPeer::Reset()
{
	m_nBetCoin = 0 ;
	m_eState = eRoomPeer_None ;
	m_PeerCard.Reset();
}

CPlayerBaseData* CRoomPeer::GetPlayerBaseData()
{
	return GetPlayer()->GetBaseData() ;
}

void CRoomPeer::OnGetCard( unsigned char nCardA, unsigned char nCardB , unsigned char nCardC )
{
	m_PeerCard.SetPeerCardByNumber(nCardA,nCardB,nCardC) ;
}

void CRoomPeer::LeaveRoom()
{
	Reset();
	if ( m_pRoom )
	{
		m_pRoom->OnPeerLeave(this);
		m_pRoom = NULL ;
	}
}

void CRoomPeer::OnWaitTimeOut()// please make a message fake ;
{
	if ( m_pRoom )
	{
		stMsgRoomGiveUp msg ;
		m_pRoom->OnPeerMsg(this,&msg) ;
	}
}

bool CRoomPeer::IsActive() // not fail ,not give, not null 
{
	return (GetState() == eRoomPeer_Unlook || GetState() == eRoomPeer_Look );
}

unsigned int CRoomPeer::GetSessionID()
{
	return GetPlayer()->GetSessionID() ;
}

unsigned int CRoomPeer::GetCoin()
{
	return GetPlayerBaseData()->nCoin ;
}

unsigned int  CRoomPeer::AddBetCoin( unsigned int naddBetCoin ) // return indeeed added betCoin ;
{
	if ( naddBetCoin > GetCoin() )
	{
		naddBetCoin  = GetCoin() ;
	}
	m_nBetCoin += naddBetCoin ;
	GetPlayerBaseData()->nCoin -= naddBetCoin ;
	return naddBetCoin ;
}

void CRoomPeer::GetBrifBaseInfo(stRoomPeerBrifInfo& vInfoOut )
{
	vInfoOut.ePeerState = GetState() ;
	vInfoOut.nBetCoin = m_nBetCoin ;
	vInfoOut.nCoin = GetCoin() ;
	vInfoOut.nDefaulPhotoID = GetPlayerBaseData()->nDefaulPhotoID ;
	vInfoOut.nIdx = m_nPeerIdx ;
	vInfoOut.nSessionID = GetSessionID() ;
	vInfoOut.nTitle = GetPlayerBaseData()->nTitle ;
	vInfoOut.nUserDefinePhotoID = GetPlayerBaseData()->nUserDefinePhotoID ;
	memset(vInfoOut.pName,0	,sizeof(vInfoOut.pName));
	sprintf_s(vInfoOut.pName,"%s",GetPlayerBaseData()->strName);
}

void CRoomPeer::OnWinCoin(unsigned int nWinCoin )
{
	GetPlayerBaseData()->nCoin += nWinCoin ;
}
