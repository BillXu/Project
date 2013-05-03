#include "Room.h"
#include "Timer.h"
#include "RoomPeer.h"
#include "Player.h"
#include "LogManager.h"
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
	switch( pMsg->usMsgType )
	{
	case MSG_ROOM_ENTER:
		{
			OnPlayerEnter(pPeer) ;
		}
		break;
	case MSG_ROOM_EXIT:
		{
			OnPlayerExit(pPeer) ;
		}
		break;
	case MSG_ROOM_ACTION:
		{
			OnProcessPlayerAction((stMsgRoomActionCmd*)pMsg,pPeer) ;
		}
		break; 
	}
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
	if ( nTimerID == m_pTimerToAction->GetTimerID() )
	{

	}
	else if ( nTimerID == m_pTimerToReady->GetTimerID() )
	{
		m_pTimerToReady->Stop() ;
		if ( GetReadyPeerCount() < 2 )
		{
			m_pTimerToReady->Reset();
			m_pTimerToReady->Start();
			return ;
		}
		DistributeCard() ;
	}
}

unsigned char CRoom::GetRoomPeerCount()
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
	// send base info ;
	int nPlayerCount = GetRoomPeerCount();
	stRoomPeerBrifData* pAllPeerCount = new stRoomPeerBrifData[nPlayerCount];
	for ( int i = 0 , nAdded = 0 ; i < MAX_ROOM_PEER ; ++i, ++nAdded )
	{
		CRoomPeer* pPeer = m_pAllPeers[i];
		if ( pPeer == NULL )
			continue ;
		pAllPeerCount[nAdded].nAllMoney = 1000;
		pAllPeerCount[nAdded].nBetMoney = pPeer->m_nBetMoney ;
		pAllPeerCount[nAdded].nIdx = i ;
		pAllPeerCount[nAdded].nPeerState = pPeer->GetState();
		pAllPeerCount[nAdded].nShowCard[0] =  0 ;
		pAllPeerCount[nAdded].nShowCard[1] = 0 ;
		pAllPeerCount[nAdded].nSwitchedCard[0] = 0 ;
		pAllPeerCount[nAdded].nSwitchedCard[1] = 0 ;
		pAllPeerCount[nAdded].nSwitchedCard[2] = 0 ;
		pAllPeerCount[nAdded].nUserUID = pPeer->GetPlayer()->GetUserUID();
		pAllPeerCount[nAdded].nTimesMoneyForPK = pPeer->m_nTimesMoneyForPK;
		pAllPeerCount[nAdded].nTitle = 0 ;
		sprintf(pAllPeerCount[nAdded].pName,"%s",pPeer->GetPlayerBaseData()->strName); 
		memcpy(pAllPeerCount[nAdded].pName,pPeer->GetPlayerBaseData()->strName, sizeof(char)* MAX_LEN_CHARACTER_NAME) ;
	}
	stMsgRoomInfo MsgInfo ;
	MsgInfo.nRound = m_nRound ;
	MsgInfo.nAllBetMoney = m_nAllBetMoney ;
	MsgInfo.nCurPlayerIdx = m_nCurActionPeerIndex ;
	MsgInfo.nMainPlayerIdx = m_nMainPeerIndex ;
	MsgInfo.nRoomID = m_nRoomID ;
	MsgInfo.nRoomState = m_eState ;
	MsgInfo.nRunWaitSecond = 20 ;
	MsgInfo.nSingleBetMoney = m_nSingleBetMoney ;
	MsgInfo.peerCount = nPlayerCount ;
	int nLen = sizeof(stMsgRoomInfo) + sizeof(stRoomPeerBrifData) * nPlayerCount;
	char* pBuffer = new char[nLen] ;
	memcpy(pBuffer,&MsgInfo,sizeof(stMsgRoomInfo)) ;
	memcpy(pBuffer + sizeof(stMsgRoomInfo),pAllPeerCount,nLen - sizeof(stMsgRoomInfo) );
	pPeerToEnter->SendMsgToClient(pBuffer,nLen);
	delete[]pBuffer ;
	delete[]pAllPeerCount;

	// add player to room 
	pPeerToEnter->m_pRoom = this ;
	for ( int i = 0 ; i < MAX_ROOM_PEER; ++i )
	{
		if ( m_pAllPeers[i] == NULL )
		{
			m_pAllPeers[i] = pPeerToEnter ;
			pPeerToEnter->m_nPeerIdx = i ;
			pPeerToEnter->OnEnterRoom(this) ;
			break; ;
		}
	}
	// send player EnterMsg ;
	stMsgOtherPlayerEnter msgOther ;
	msgOther.nAllMoney = 1000 ;
	msgOther.nPeerIdx = pPeerToEnter->m_nPeerIdx ;
	msgOther.nTitle = 0 ;
	msgOther.pPlayerUID = pPeerToEnter->GetPlayer()->GetUserUID();
	SendMsgToRoomPeers((char*)&msgOther,sizeof(msgOther),pPeerToEnter) ;
}

void CRoom::OnPlayerExit( CRoomPeer* pPeerToEnter )
{
	if ( pPeerToEnter->m_nPeerIdx >= MAX_ROOM_PEER )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("Player Room Idx is too big idx = %d",pPeerToEnter->m_nPeerIdx);
		return ;
	}
	stMsgOtherPlayerExit msg ;
	msg.nRoomIdx = pPeerToEnter->m_nPeerIdx ;
	SendMsgToRoomPeers((char*)&msg,sizeof(msg),pPeerToEnter) ;
	m_pAllPeers[pPeerToEnter->m_nPeerIdx] = NULL ;
	pPeerToEnter->OnExitRoom(this) ;
}

void CRoom::OnProcessPlayerAction(stMsgRoomActionCmd* pActMsg, CRoomPeer* pPeer )
{
	switch( pActMsg->nActionType )
	{
	case eRoomPeerAction_Ready:
		{
			if ( m_eState != eRoom_Wait )
			{
				stMsgRoomActionRet RetMsg ;
				RetMsg.nErr = 1 ; 
				pPeer->SendMsgToClient((char*)&RetMsg,sizeof(RetMsg)) ;
				return ;
			}
			//m_pTimerToReady->Reset();
			//m_pTimerToReady->Start() ;
			pPeer->m_eState = CRoomPeer::eState_Ready ;

			stMsgOtherPlayerCmd msg ;
			msg.nActionType = pActMsg->nActionType ;
			msg.nPeerIdx = pPeer->m_nPeerIdx ;
			SendMsgToRoomPeers((char*)&msg,sizeof(msg),pPeer) ;
			// check if all player ready ?
			if ( CheckAllPlayerReady() )
			{
				DistributeCard();
			}
		}
		break;
	}
}

void CRoom::DistributeCard()
{
	int nReadyCount = GetReadyPeerCount();
	if (  nReadyCount < 2 )
		return ;
	m_pTimerToReady->Stop() ;
	m_Poker.ComfirmKeepCard(nReadyCount * PEER_CARD_COUNT * 2 ) ;

	stGetCardPeer* pVGetCardPeer = new stGetCardPeer[nReadyCount] ;

	for ( int i = 0, nIdx = 0 ; i < MAX_ROOM_PEER ; ++i, ++nIdx )
	{
		if ( NULL == m_pAllPeers[i])
			continue ;
		
		if ( CRoomPeer::eState_Ready == m_pAllPeers[i]->m_eState )
		{
			pVGetCardPeer[nIdx].nPeerIdx = m_pAllPeers[i]->m_nPeerIdx ;
			for ( int n = 0 ; n < PEER_CARD_COUNT ; ++ n )
			{
				pVGetCardPeer[nIdx].vCard[n] = m_Poker.GetCardWithCompositeNum();
			}
			
			m_pAllPeers[i]->OnGetCard(pVGetCardPeer[nIdx].vCard[0],pVGetCardPeer[nIdx].vCard[1],pVGetCardPeer[nIdx].vCard[2]) ;
			m_pAllPeers[i]->m_eState =  CRoomPeer::eState_Playing ;
		}
	}

	// send msg ;
	stMsgRoomDistributeCard msg ;
	msg.nCount = nReadyCount ;
	int nLen = sizeof(msg) + nReadyCount * sizeof(stGetCardPeer) ;
	char* pBuffer = new char[nLen] ;
	memcpy(pBuffer,&msg,sizeof(msg));
	memcpy(pBuffer + sizeof(msg),pVGetCardPeer,nLen - sizeof(msg));
	SendMsgToRoomPeers(pBuffer,nLen,NULL) ;
	delete[] pBuffer ;
	delete[] pVGetCardPeer ;
	m_eState = eRoom_Playing ;
}

bool CRoom::CheckAllPlayerReady()
{
	for ( int i = 0 ; i <MAX_ROOM_PEER ; ++i )
	{
		if ( m_pAllPeers[i] == NULL )
			continue ;
		if ( CRoomPeer::eState_Ready != m_pAllPeers[i]->m_eState )
			return false ;
	}
	return true ;
}

unsigned char CRoom::GetReadyPeerCount()
{
	unsigned char nCount = 0 ;
	for ( int i = 0 ; i <MAX_ROOM_PEER ; ++i )
	{
		if ( m_pAllPeers[i] == NULL )
			continue ;
		if ( CRoomPeer::eState_Ready == m_pAllPeers[i]->m_eState )
		{
			++nCount ;
		}
	}
	return nCount ;
}