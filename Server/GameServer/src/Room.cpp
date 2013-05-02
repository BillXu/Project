#include "Room.h"
#include "Timer.h"
#include "RoomPeer.h"
CRoom::CRoom( unsigned int nRoomID )
	:m_nRoomID(nRoomID),m_strRoomName(""),m_nflag(0)
{
	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
	{
		m_pAllPeers[i] = NULL ;
	}

	m_pTimerToReady = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
	m_pTimerToReady->SetInterval(TIMER_WAIT_TO_READY);

	m_pTimerToAction = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
	m_pTimerToAction->SetInterval(TIMER_WAIT_ACTION);

	m_nCurActionPeerIndex = 0 ;
	m_nMainPeerIndex = 0; // 本局庄家的索引.

	m_nAllBetMoney = 0 ;
	m_nRound = 0  ;
	m_nSingleBetMoney = 0 ; 
}

CRoom::~CRoom()
{
	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerToReady);
	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerToAction);
}

void CRoom::Init()
{
	m_pTimerToReady->Stop();
	m_pTimerToAction->Stop() ;
}

bool CRoom::HasFlag(eRoomFlag eFlag )
{
	return m_nflag &= 1 << eFlag ;
}

void CRoom::SetFlag(eRoomFlag eFlag )
{
	m_nflag = 1 << eFlag ;
}

void CRoom::OnRoomMsg(stMsg* pMsg , CRoomPeer* pPeer )
{

}

void CRoom::SendMsgToRoomPeers( char* pbuffer ,unsigned short nLen ,CRoomPeer* pExcept )
{
	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
	{
		if ( NULL == m_pAllPeers[i] || m_pAllPeers[i] == pExcept )
			continue ;
		m_pAllPeers[i]->SendMsgToClient(pbuffer,nLen);
	}
}

void CRoom::TimerFunc(float fTimeElaps,unsigned int nTimerID )
{

}

unsigned char CRoom::RoomPeerCount()
{
	unsigned char count = 0 ;
	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
	{
		if ( NULL == m_pAllPeers[i] )
			continue ;
		++count ;
	}
	return count ;
}

unsigned int CRoom::GetRoomID()
{
	return m_nRoomID ;
}

void CRoom::SetRoomID(unsigned int nRoomId )
{
	m_nRoomID = nRoomId ;
}

void CRoom::OnPlayerEnter( CRoomPeer* pPeerToEnter )
{

}

void CRoom::OnPlayerExit( CRoomPeer* pPeerToEnter )
{

}