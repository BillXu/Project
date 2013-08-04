#include "Room.h"
#include "Timer.h"
#include "RoomPeer.h"
#include "Player.h"
#include "LogManager.h"
#include <list>
#define TAX_START_COIN 10000
unsigned int CRoom::s_RoomID = 0 ;

CRoom::CRoom()
{
	m_eRoomType = eRoom_Gold ;
}

CRoom::~CRoom()
{

}

void CRoom::Init( unsigned int nRoomID , unsigned char nMaxSeat )
{
	CRoomBase::Init(nRoomID,nMaxSeat) ;
	m_nCurMainPeerIdx = 0 ;
	Rest();
}

void CRoom::Rest()
{
	float m_fRoomSateTick[eRoomState_Max] ;
	memset(m_fRoomSateTick,0,sizeof(m_fRoomSateTick)) ;
	m_nCurWaitPeerIdx = m_nCurMainPeerIdx ;
	m_nSingleBetCoin = 10 ;
	m_nTotalBetCoin = 0 ;
	m_nRound = 0;
}

unsigned char CRoom::GetEmptySeatCount()
{
	unsigned char n = 0 ;
	for ( int i =0  ; i < GetMaxSeat(); ++i )
	{
		if ( m_vRoomPeer[i] == NULL )
		{
			++n ;
		}
	}
	return n ;
}

bool CRoom::CanJoin(CRoomPeer* peer )
{
	if ( CRoomBase::CanJoin(peer) == false )
	{
		return false ;
	}

	// some other condition ;
	return true ;
}

void CRoom::AddPeer(CRoomPeer* peer )
{
	// find a empty place 
	for ( int i = 0 ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] == NULL )
		{
			peer->m_nPeerIdx = i ;
			break; 
		}
	}

	stMsgRoomPlayerEnter msg ;
	peer->GetBrifBaseInfo(msg.nEnterPlayerInfo) ;
	SendMsgRoomPeers(&msg,sizeof(msg)) ;
	m_vRoomPeer[peer->GetPeerIdxInRoom()] = peer ;
}

void CRoom::Update(float fTimeElpas, unsigned int nTimerID )
{
	eRoomState eState = GetRoomState() ;
	switch ( eState )
	{
	case eRoomState_WaitPeerToJoin:
		{
			// just wait 
			if ( GetEmptySeatCount() == 0 )
			{
				SwitchToRoomSate(eRoomState_WaitPeerToJoin, eRoomState_WaitPeerToGetReady ) ;
			}
		}
		break;
	case eRoomState_WaitPeerToGetReady:
		{
			m_fRoomSateTick[eState]-= fTimeElpas ;
			if ( m_fRoomSateTick[eState] <= 0 || GetReadyPeerCount() == GetMaxSeat() )
			{
				SwitchToRoomSate(eState,eRoomState_DistributeCard) ;
			}
		}
		break;
	case eRoomState_DistributeCard:
		{
			m_fRoomSateTick[eState]-= fTimeElpas ;
			if ( m_fRoomSateTick[eState] <= 0 )
			{
				SwitchToRoomSate(eState,eRoomState_WaitPeerAction) ;
			}
		}
		break;
	case eRoomState_WaitPeerAction:
		{
			m_fRoomSateTick[eState]-= fTimeElpas ;
			if ( m_fRoomSateTick[eState] <= 0 )
			{
				m_vRoomPeer[m_nCurWaitPeerIdx]->OnWaitTimeOut();
			}
		}
		break;
	case eRoomState_PKing:
		{
			m_fRoomSateTick[eState]-= fTimeElpas ;
			if ( m_fRoomSateTick[eState] <= 0 )
			{
				if ( CheckFinish() )
				{
					SwitchToRoomSate(GetRoomState(),eRoomState_ShowingResult ) ;
				}
				else
				{
					NextPlayerAction() ;
				}
			}
		}
		break;
	case eRoomState_ShowingResult:
		{
			m_fRoomSateTick[eState]-= fTimeElpas ;
			if ( m_fRoomSateTick[eState] <= 0 )
			{
				SwitchToRoomSate(eState,eRoomState_WaitPeerToJoin) ;
			}
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog("unknown room state = %d", eState) ;
		}
		break; 
	}
}

void CRoom::SendCurRoomToPeer(CRoomPeer* peer )
{
	stMsgRoomCurInfo msgRoomInfo ;
	msgRoomInfo.eRoomSate = GetRoomState() ;
	msgRoomInfo.nPlayerCount = GetMaxSeat() - GetEmptySeatCount() -1 ; // -1 self ;
	msgRoomInfo.nRound = m_nRound ;
	msgRoomInfo.nSingleBetCoin = m_nSingleBetCoin ;
	msgRoomInfo.nTotalBetCoin = m_nTotalBetCoin ;
	int nAllLen = sizeof(msgRoomInfo) + msgRoomInfo.nPlayerCount * sizeof(stRoomPeerBrifInfo) ;
	stRoomPeerBrifInfo stBrifInfo ;
	char* pBuffer = new char[nAllLen] ;
	int nOffset = 0 ;
	memcpy(pBuffer,&msgRoomInfo,sizeof(msgRoomInfo));
	nOffset += sizeof(msgRoomInfo);
	for ( int i = 0 ; i < GetMaxSeat(); ++i )
	{
		if ( m_vRoomPeer[i] == NULL || peer->GetSessionID() == m_vRoomPeer[i]->GetSessionID() )
		{
			continue;
		}
		m_vRoomPeer[i]->GetBrifBaseInfo(stBrifInfo) ;
		memcpy(pBuffer + nOffset, &stBrifInfo,sizeof(stRoomPeerBrifInfo)) ;
		nOffset += sizeof(stRoomPeerBrifInfo);
	}
	peer->SendMsgToClient(pBuffer,nAllLen) ;
}

void CRoom::SwitchToRoomSate( eRoomState eFrom, eRoomState eToDest )
{
	switch ( eToDest )
	{
	case eRoomState_WaitPeerToJoin:
		{
			
		}
		break;
	case eRoomState_WaitPeerToGetReady:
		{
			m_fRoomSateTick[eToDest] = TIME_ROOM_WAIT_READY ;
		}
		break;
	case eRoomState_DistributeCard:
		{
			// send distribute msg , switch ready player's state to unkook ;
			m_fRoomSateTick[eToDest] = TIME_ROOM_DISTRIBUTE ;
			m_Poker.ComfirmKeepCard(GetReadyPeerCount() * 3 ) ;
			for ( int i = 0 ; i < GetMaxSeat() ; ++i )
			{
				if (m_vRoomPeer[i] && eRoomPeer_Ready == m_vRoomPeer[i]->GetState() )
				{
					m_vRoomPeer[i]->OnGetCard(m_Poker.GetCardWithCompositeNum(),m_Poker.GetCardWithCompositeNum(),m_Poker.GetCardWithCompositeNum() );
					m_vRoomPeer[i]->m_eState = eRoomPeer_Unlook ;
				}
			}
			stMsgDistributeCard msg ;
			SendMsgRoomPeers(&msg,sizeof(msg)) ;
		}
		break;
	case eRoomState_WaitPeerAction:
		{
			m_fRoomSateTick[eToDest] = TIME_ROOM_WAIT_PEER_ACTION ;
			if ( eRoomState_PKing == eFrom )
			{
				NextPlayerAction();
			}
			else if ( eRoomState_DistributeCard == eFrom )
			{
				--m_nCurMainPeerIdx ;
				NextPlayerAction() ;
			}
			else
			{
				CLogMgr::SharedLogMgr()->ErrorLog("Unknown state switch to eRoomState_WaitPeerAction, nFrom = %d",eFrom ) ;
			}
		}
		break;
	case eRoomState_PKing:
		{
			m_fRoomSateTick[eToDest] = TIME_ROOM_PK_DURATION ;
		}
		break;
	case eRoomState_ShowingResult:
		{
			m_fRoomSateTick[eToDest] = TIME_ROOM_SHOW_RESULT ;
			Rest();
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog("switch unknown room state = %d", eToDest) ;
			return ;
		}
		break; 
	}
	m_eRoomState = eToDest ;
}

void CRoom::NextPlayerAction()
{
	for ( int i = m_nCurWaitPeerIdx + 1 ; 1 ;++i )
	{
		int nIdx = i ;
		if ( nIdx >= GetMaxSeat() - 1 )
		{
			nIdx -= GetMaxSeat() ;
		}
		
		if ( nIdx == m_nCurMainPeerIdx )
		{
			++m_nRound ;
		}

		if ( m_vRoomPeer[nIdx] && m_vRoomPeer[nIdx]->IsActive() )
		{
			m_nCurWaitPeerIdx = nIdx ;
			break; 
		}
	}
	stMsgRoomWaitPlayerAction msg ;
	msg.nRound = m_nRound ;
	msg.nSessionID = m_vRoomPeer[m_nCurWaitPeerIdx]->GetSessionID() ;
	SendMsgRoomPeers(&msg,sizeof(msg)) ;
}

char CRoom::GetReadyPeerCount()
{
	char nCount = 0 ;
	for ( int i = 0 ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] && m_vRoomPeer[i]->GetState() == eRoomPeer_Ready )
		{
			++nCount ;
		}
	}
	return nCount ;
}

bool CRoom::CheckFinish()
{
	char nCount = 0 ;
	CRoomPeer* peer = NULL ;
	for ( int i = 0 ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] && m_vRoomPeer[i]->IsActive() )
		{
			++nCount ;
			peer = m_vRoomPeer[i] ;
			if ( nCount > 1 )
			{
				return false ;
			}
		}
	}

	if ( peer == NULL )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("No winner , no") ;
		return true ;
	}

	std::list<stResultData*> vRData ;
	for ( int i = 0 ; i < GetMaxSeat() ; ++i )
	{
		if ( m_vRoomPeer[i] == NULL )
		{
			continue; 
		}
		if ( m_vRoomPeer[i]->GetState() == eRoomPeer_GiveUp || eRoomPeer_Failed == m_vRoomPeer[i]->GetState() )
		{
			stResultData* pData = new stResultData ;
			pData->idx = i ;
			pData->nResultCoin = m_vRoomPeer[i]->m_nBetCoin ;
			pData->nResultCoin *= -1 ;
			m_vRoomPeer[i]->m_PeerCard.GetCompositeCardRepresent(pData->vCard) ;
			vRData.push_back(pData) ;
		}
		else  if ( m_vRoomPeer[i]->IsActive() ) // win ;
		{
			stResultData* pData = new stResultData ;
			pData->idx = i ;
			pData->nResultCoin = m_nTotalBetCoin ;
			if ( m_nTotalBetCoin > TAX_START_COIN )
			{
				float nValue = (float)pData->nResultCoin * 0.95f ;
				pData->nResultCoin = (int)nValue ;
			}
			m_vRoomPeer[i]->OnWinCoin(pData->nResultCoin) ;
			m_vRoomPeer[i]->m_PeerCard.GetCompositeCardRepresent(pData->vCard) ;
			vRData.push_back(pData) ;
		}
		else
		{
			CLogMgr::SharedLogMgr()->ErrorLog("what happen other state ") ;
		}
	}

	stMsgRoomResult msgResult ;
	msgResult.nCount = vRData.size();
	char * pbuffer = new char[sizeof(msgResult) + vRData.size()*sizeof(stResultData)] ;
	int nOffset = 0 ;
	memcpy(pbuffer,&msgResult,sizeof(msgResult));
	nOffset += sizeof(msgResult) ;

	for (std::list<stResultData*>::iterator iter = vRData.begin() ; iter != vRData.end(); ++iter  )
	{
		stResultData* pData = *iter ;
		memcpy(pbuffer+nOffset,&pData,sizeof(stResultData));
		nOffset += sizeof(stResultData) ;
		delete pData ; // delete because have copyed to buffer ;
		pData = NULL ;
	}
	SendMsgRoomPeers((stMsg*)pbuffer,nOffset ) ;
	delete[] pbuffer ;

	vRData.clear() ; // new data have been delete in uplown loop ;
	return true;
}

void CRoom::DebugRoomInfo()
{
	CLogMgr::SharedLogMgr()->PrintLog("Room id = %d , totalBetCoin = %d , singleBetCoin = %d , roomState = %d , Round = %d , curIdx = %d , MainIdx = %d",m_nRoomID,m_nTotalBetCoin,m_nSingleBetCoin,GetRoomState(),m_nRound,m_nCurWaitPeerIdx,m_nCurMainPeerIdx )  ;
}

bool CRoom::OnPeerMsg(CRoomPeer* pPeer, stMsg* pmsg )
{
	if ( CRoomBase::OnPeerMsg(pPeer,pmsg) )
	{
		return true ;
	}

	switch ( pmsg->usMsgType )
	{
	case MSG_ROOM_READY:
		{
			if ( eRoomState_WaitPeerAction == GetRoomState() || eRoomState_DistributeCard == GetRoomState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false ;
			}

			pPeer->m_eState = eRoomPeer_Ready ;
			stMsgRoomPlayerReady msg ;
			msg.nReadyPlayerSessionID = pPeer->GetSessionID() ;
			SendMsgRoomPeers(&msg,sizeof(msg)) ;
			if ( GetReadyPeerCount() == GetMaxSeat() )
			{
				SwitchToRoomSate(GetRoomState(),eRoomState_DistributeCard) ;
			}
			else if ( GetReadyPeerCount() >= 2 && GetRoomState() != eRoomState_ShowingResult )
			{
				SwitchToRoomSate(GetRoomState(),eRoomState_WaitPeerToGetReady ) ;
			}
			DebugRoomInfo();
		}
		return true; 
	case MSG_ROOM_FOLLOW:
		{
			if ( eRoomState_WaitPeerAction != GetRoomState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}
			
			int factor = pPeer->GetState() == eRoomPeer_Look ? 2 : 1 ;
			unsigned int naddBet = pPeer->AddBetCoin(m_nSingleBetCoin*factor) ; 
			m_nTotalBetCoin += naddBet ;

			stMsgRoomPlayerFollow msgFollow ;
			msgFollow.nSessionID = pPeer->GetSessionID() ;
			SendMsgRoomPeers(&msgFollow,sizeof(msgFollow)) ;
			DebugRoomInfo();
			NextPlayerAction();
		}
		return true; 
	case MSG_ROOM_ADD:
		{
			if ( eRoomState_WaitPeerAction != GetRoomState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}

			// update single 
			stMsgRoomAdd* msgAdd = (stMsgRoomAdd*)pmsg ;
			if ( msgAdd->nAddMoney == 0 ) // double ;
			{
				m_nSingleBetCoin *= 2 ;
			}
			else if ( (unsigned int )msgAdd->nAddMoney < m_nSingleBetCoin )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 2 ; // add money should greate than crrent ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}
			else
			{
				m_nSingleBetCoin = msgAdd->nAddMoney ;
			}

			// update player self ;
			int factor = pPeer->GetState() == eRoomPeer_Look ? 2 : 1 ;
			unsigned int naddBet = pPeer->AddBetCoin(m_nSingleBetCoin*factor) ; 
			m_nTotalBetCoin += naddBet ;

			stMsgRoomPlayerAdd msgPlayerAdd ;
			msgPlayerAdd.nBetCoin = naddBet ;
			msgPlayerAdd.nNewSingle = m_nSingleBetCoin ;
			msgPlayerAdd.nSessionID = pPeer->GetSessionID() ;
			SendMsgRoomPeers(&msgPlayerAdd,sizeof(msgPlayerAdd)) ;
			NextPlayerAction();
			DebugRoomInfo() ;
		}
		return true;
	case MSG_ROOM_PK:
		{
			if ( eRoomState_WaitPeerAction != GetRoomState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}
			stMsgRoomPK* pkMsg = (stMsgRoomPK*)pmsg ;
			CRoomPeer* PKpeer = GetRoomPeerBySessionID(pkMsg->nPKWithSessionID );
			if ( !PKpeer || PKpeer->IsActive() == false )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 3 ; // unlegal target ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}

			bool bWin = pPeer->m_PeerCard.PKPeerCard(&PKpeer->m_PeerCard) ;
			if ( bWin )
			{
				PKpeer->m_eState = eRoomPeer_Failed ;
			}
			else
			{
				pPeer->m_eState = eRoomPeer_Failed ;
			}

			int factor = pPeer->GetState() == eRoomPeer_Look ? 2 : 1 ;
			unsigned int nConsum = m_nSingleBetCoin * factor * 2 ;
			unsigned int naddBet = pPeer->AddBetCoin(nConsum) ; 
			m_nTotalBetCoin += naddBet ;

			stMsgRoomPlayerPK msgplayerpk  ;
			msgplayerpk.bWin = bWin ;
			msgplayerpk.nConsumCoin = naddBet ;
			msgplayerpk.nPKInvokeSessionID = pPeer->GetSessionID() ;
			msgplayerpk.nPKWithSessionID = PKpeer->GetSessionID() ;
			SendMsgRoomPeers(&msgplayerpk,sizeof(msgplayerpk)) ;
			DebugRoomInfo() ;
		}
		return true ;
	case MSG_ROOM_LOOK:
		{
			if ( eRoomState_WaitPeerAction != GetRoomState() && eRoomPeer_Unlook == pPeer->GetState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}

			pPeer->m_eState = eRoomPeer_Look ; 
			stMsgRoomPlayerLook msgLook ;
			msgLook.nSessionID = pPeer->GetSessionID() ;
			pPeer->m_PeerCard.GetCompositeCardRepresent(msgLook.vCard);
			SendMsgRoomPeers(&msgLook,sizeof(msgLook)) ;
		}
		return true;
	case MSG_ROOM_GIVEUP:
		{
			if ( eRoomState_WaitPeerAction != GetRoomState() )
			{
				stMsgRoomRet msgRet ;
				msgRet.nRet = 1 ; // room state not fitable ;
				pPeer->SendMsgToClient((char*)&msgRet, sizeof(msgRet)) ; 
				return false;
			}
			pPeer->m_eState = eRoomPeer_GiveUp ;

			stMsgRoomPlayerGiveUp msgGiveUp ;
			msgGiveUp.nIdx = pPeer->m_nPeerIdx ;
			SendMsgRoomPeers(&msgGiveUp,sizeof(msgGiveUp)) ;	
			if ( CheckFinish() )
			{
				SwitchToRoomSate(GetRoomState(),eRoomState_ShowingResult) ;
			}
		}
		return true; 
	}
	return false ;
}

//CRoom::CRoom( int nBaseMoney)
//	:m_nBaseBetMoney(nBaseMoney),m_strRoomName(""),m_nflag(0)
//{
//	m_nRoomID = ++s_RoomID ;
//
//	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
//	{
//		m_pAllPeers[i] = NULL ;
//	}
//
//	m_pTimerToReady = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
//	m_pTimerToReady->SetInterval(TIMER_WAIT_TO_READY);
//
//	m_pTimerToAction = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
//	m_pTimerToAction->SetInterval(TIMER_WAIT_ACTION);
//
//	m_pTimerWaitDistribute = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
//	m_pTimerWaitDistribute->SetInterval(TIMER_WAIT_DISTRIBUTE);
//
//	m_pTimerWaitPK = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
//	m_pTimerWaitPK->SetInterval(TIMER_WAIT_PK);
//
//	m_pTimerWaitFinish = CTimerManager::SharedTimerManager()->AddTimer(this,(CTimerDelegate::lpTimerSelector)&CRoom::TimerFunc);
//	m_pTimerWaitFinish->SetInterval(TIMER_WAIT_FINISH);
//
//	m_nCurActionPeerIndex = 0 ;
//	m_nMainPeerIndex = -1; // 本局庄家的索引.
//
//	m_nAllBetMoney = 0 ;
//	m_nRound = 0  ;
//	m_nSingleBetMoney = 0 ; 
//	OnRestarMatch();
//}
//
//CRoom::~CRoom()
//{
//	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerToReady);
//	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerToAction);
//	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerWaitDistribute);
//	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerWaitPK);
//	CTimerManager::SharedTimerManager()->RemoveTimer(m_pTimerWaitFinish);
//}
//
//void CRoom::Init()
//{
//	m_pTimerToReady->Stop();
//	m_pTimerToAction->Stop() ;
//	m_pTimerWaitDistribute->Stop();
//	m_pTimerWaitPK->Stop() ;
//	m_pTimerWaitFinish->Stop() ;
//}
//
//bool CRoom::HasFlag(eRoomFlag eFlag )
//{
//	return m_nflag &= 1 << eFlag ;
//}
//
//void CRoom::SetFlag(eRoomFlag eFlag )
//{
//	m_nflag = 1 << eFlag ;
//}
//
//void CRoom::OnRoomMsg(stMsg* pMsg , CRoomPeer* pPeer )
//{
//	switch( pMsg->usMsgType )
//	{
//	case MSG_ENTER:
//		{
//			OnPlayerEnter(pPeer) ;
//		}
//		break;
//	case MSG_EXIT:
//		{
//			OnPlayerExit(pPeer) ;
//		}
//		break;
//	case MSG_ACTION:
//		{
//			OnProcessPlayerAction((stMsgActionCmd*)pMsg,pPeer) ;
//		}
//		break; 
//	}
//}
//
//void CRoom::SendMsgToRoomPeers( char* pbuffer ,unsigned short nLen ,CRoomPeer* pExcept )
//{
//	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
//	{
//		if ( NULL == m_pAllPeers[i] || m_pAllPeers[i] == pExcept )
//			continue ;
//		m_pAllPeers[i]->SendMsgToClient(pbuffer,nLen);
//	}
//}
//
//void CRoom::TimerFunc(float fTimeElaps,unsigned int nTimerID )
//{
//	if ( nTimerID == m_pTimerToAction->GetTimerID() )
//	{
//		m_pAllPeers[m_nCurActionPeerIndex]->m_eState = CRoomPeer::eState_GiveUp ;
//		stMsgRoomPlayerCmd msg ;
//		msg.nPeerIdx = m_nCurActionPeerIndex ;
//		msg.nActionType = eRoomPeerAction_GiveUp ;
//		SendMsgToRoomPeers((char*)&msg, sizeof(msg),NULL) ;
//		NextTurn();
//	}
//	else if ( nTimerID == m_pTimerToReady->GetTimerID() )
//	{
//		m_pTimerToReady->Stop() ;
//		if ( GetReadyPeerCount() < 2 )
//		{
//			m_pTimerToReady->Reset();
//			m_pTimerToReady->Start();
//			return ;
//		}
//		DistributeCard() ;
//	}
//	else if ( nTimerID == m_pTimerWaitDistribute->GetTimerID() )
//	{
//		m_pTimerWaitDistribute->Stop() ;
//		stMsgRoomPlayerActionTurn msg ;
//		msg.nPlayerIdx = m_nMainPeerIndex ;
//		SendMsgToRoomPeers((char*)&msg,sizeof(msg),NULL) ;
//		m_pTimerToAction->Reset();
//		m_pTimerToAction->Start();
//	}
//	else if ( nTimerID == m_pTimerWaitPK->GetTimerID() )
//	{
//		m_pTimerWaitPK->Stop() ;
//		NextTurn();
//	}
//	else if ( nTimerID == m_pTimerWaitFinish->GetTimerID() )
//	{
//		m_pTimerWaitFinish->Stop() ;
//		OnRestarMatch();
//	}
//}
//
//unsigned char CRoom::GetRoomPeerCount()
//{
//	unsigned char count = 0 ;
//	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
//	{
//		if ( NULL == m_pAllPeers[i] )
//			continue ;
//		++count ;
//	}
//	return count ;
//}
//
//unsigned int CRoom::GetRoomID()
//{
//	return m_nRoomID ;
//}
//
//void CRoom::OnPlayerEnter( CRoomPeer* pPeerToEnter )
//{
//	// send base info ;
//	int nPlayerCount = GetRoomPeerCount();
//	stRoomPeerBrifData* pAllPeerCount = new stRoomPeerBrifData[nPlayerCount];
//	for ( int i = 0 , nAdded = 0 ; i < MAX_ROOM_PEER ; ++i, ++nAdded )
//	{
//		CRoomPeer* pPeer = m_pAllPeers[i];
//		if ( pPeer == NULL )
//			continue ;
//		pAllPeerCount[nAdded].nAllMoney = 1000;
//		pAllPeerCount[nAdded].nBetMoney = pPeer->m_nBetMoney ;
//		pAllPeerCount[nAdded].nIdx = i ;
//		pAllPeerCount[nAdded].nPeerState = pPeer->GetState();
//		pAllPeerCount[nAdded].nShowCard[0] =  0 ;
//		pAllPeerCount[nAdded].nShowCard[1] = 0 ;
//		pAllPeerCount[nAdded].nSwitchedCard[0] = 0 ;
//		pAllPeerCount[nAdded].nSwitchedCard[1] = 0 ;
//		pAllPeerCount[nAdded].nSwitchedCard[2] = 0 ;
//		pAllPeerCount[nAdded].nUserUID = pPeer->GetPlayer()->GetUserUID();
//		pAllPeerCount[nAdded].nTimesMoneyForPK = pPeer->m_nTimesMoneyForPK;
//		pAllPeerCount[nAdded].nTitle = 0 ;
//		sprintf(pAllPeerCount[nAdded].pName,"%s",pPeer->GetPlayerBaseData()->strName); 
//		memcpy(pAllPeerCount[nAdded].pName,pPeer->GetPlayerBaseData()->strName, sizeof(char)* MAX_LEN_CHARACTER_NAME) ;
//	}
//	stMsgRoomInfo MsgInfo ;
//	MsgInfo.nRound = m_nRound ;
//	MsgInfo.nAllBetMoney = m_nAllBetMoney ;
//	MsgInfo.nCurPlayerIdx = m_nCurActionPeerIndex ;
//	MsgInfo.nMainPlayerIdx = m_nMainPeerIndex ;
//	MsgInfo.nRoomID = m_nRoomID ;
//	MsgInfo.nRoomState = m_eState ;
//	MsgInfo.nRunWaitSecond = 20 ;
//	MsgInfo.nSingleBetMoney = m_nSingleBetMoney ;
//	MsgInfo.peerCount = nPlayerCount ;
//	int nLen = sizeof(stMsgRoomInfo) + sizeof(stRoomPeerBrifData) * nPlayerCount;
//	char* pBuffer = new char[nLen] ;
//	memcpy(pBuffer,&MsgInfo,sizeof(stMsgRoomInfo)) ;
//	memcpy(pBuffer + sizeof(stMsgRoomInfo),pAllPeerCount,nLen - sizeof(stMsgRoomInfo) );
//	pPeerToEnter->SendMsgToClient(pBuffer,nLen);
//	delete[]pBuffer ;
//	delete[]pAllPeerCount;
//
//	// add player to room 
//	pPeerToEnter->m_pRoom = this ;
//	for ( int i = 0 ; i < MAX_ROOM_PEER; ++i )
//	{
//		if ( m_pAllPeers[i] == NULL )
//		{
//			m_pAllPeers[i] = pPeerToEnter ;
//			pPeerToEnter->m_nPeerIdx = i ;
//			pPeerToEnter->OnEnterRoom(this) ;
//			break; ;
//		}
//	}
//	// send player EnterMsg ;
//	stMsgRoomPlayerEnter msgOther ;
//	msgOther.nAllMoney = 1000 ;
//	msgOther.nPeerIdx = pPeerToEnter->m_nPeerIdx ;
//	msgOther.nTitle = 0 ;
//	msgOther.pPlayerUID = pPeerToEnter->GetPlayer()->GetUserUID();
//	SendMsgToRoomPeers((char*)&msgOther,sizeof(msgOther),pPeerToEnter) ;
//}
//
//void CRoom::OnPlayerExit( CRoomPeer* pPeerToEnter )
//{
//	if ( pPeerToEnter->m_nPeerIdx >= MAX_ROOM_PEER )
//	{
//		CLogMgr::SharedLogMgr()->ErrorLog("Player Room Idx is too big idx = %d",pPeerToEnter->m_nPeerIdx);
//		return ;
//	}
//
//	stMsgRoomPlayerExit msg ;
//	msg.nRoomIdx = pPeerToEnter->m_nPeerIdx ;
//	SendMsgToRoomPeers((char*)&msg,sizeof(msg),pPeerToEnter) ;
//	m_pAllPeers[pPeerToEnter->m_nPeerIdx] = NULL ;
//	pPeerToEnter->OnExitRoom(this) ;
//	if ( pPeerToEnter->m_nPeerIdx == m_nCurActionPeerIndex )
//	{
//		NextTurn();
//	}
//}
//
//void CRoom::OnProcessPlayerAction(stMsgActionCmd* pActMsg, CRoomPeer* pPeer )
//{
//	if ( CanPlayerDoThisAction((eRoomPeerAction)pActMsg->nActionType,pPeer) == false )
//		return ;
//	switch( pActMsg->nActionType )
//	{
//	case eRoomPeerAction_Ready:
//		{
//			stMsgRoomPlayerCmd cmdmsg ;
//			cmdmsg.nPeerIdx = pPeer->m_nPeerIdx ;
//			cmdmsg.nActionType = pActMsg->nActionType ;
//			//m_pTimerToReady->Reset();
//			//m_pTimerToReady->Start() ;
//			pPeer->m_eState = CRoomPeer::eState_Ready ;
//			SendMsgToRoomPeers((char*)&cmdmsg,sizeof(cmdmsg),NULL) ;
//			// check if all player ready ?
//			if ( CheckAllPlayerReady() )
//			{
//				DistributeCard();
//			}
//		}
//	case eRoomPeerAction_Follow:
//		{
//			int nBet = m_nSingleBetMoney * (pPeer->m_bViewdCard ? 2 : 1 );
//			if ( !pPeer->AddBet(nBet) )
//			{
//				stMsgRoomPlayerCmd cmdmsg ;
//				cmdmsg.nPeerIdx = pPeer->m_nPeerIdx ;
//				cmdmsg.nActionType = eRoomPeerAction_GiveUp ;
//				pPeer->m_eState = CRoomPeer::eState_GiveUp ;
//				SendMsgToRoomPeers((char*)&cmdmsg,sizeof(cmdmsg),NULL) ;
//			}
//			else
//			{
//				m_nAllBetMoney += nBet ;
//
//				stMsgRoomPlayerFollow msgFollow ;
//				msgFollow.nPeerIdx = pPeer->m_nPeerIdx ;
//				msgFollow.nActionType = pActMsg->nActionType ;
//				msgFollow.nPlayerAllBetMoney = pPeer->m_nBetMoney ;
//				msgFollow.nPlayerLeftMoney = pPeer->GetPlayerBaseData()->nCoin ;
//				msgFollow.nRoomAllBetMoney = m_nAllBetMoney ;
//				SendMsgToRoomPeers((char*)&msgFollow,sizeof(msgFollow),NULL) ;
//			}
//			// go to next turn ;
//			NextTurn();
//		}
//		break;
//	case eRoomPeerAction_Add:
//		{ 
//			int nBet = (m_nSingleBetMoney + pActMsg->nArgument) * (pPeer->m_bViewdCard ? 2 : 1 );
//			if ( !pPeer->AddBet(nBet) )
//			{
//				stMsgRoomPlayerCmd cmdmsg ;
//				cmdmsg.nPeerIdx = pPeer->m_nPeerIdx ;
//				cmdmsg.nActionType = eRoomPeerAction_GiveUp ;
//				pPeer->m_eState = CRoomPeer::eState_GiveUp ;
//				SendMsgToRoomPeers((char*)&cmdmsg,sizeof(cmdmsg),NULL) ;
//			}
//			else
//			{
//				m_nSingleBetMoney += pActMsg->nArgument ;
//				m_nAllBetMoney += nBet ;
//
//				stMsgRoomPlayerAdd msgAdd ;
//				msgAdd.nPeerIdx = pPeer->m_nPeerIdx ;
//				msgAdd.nActionType = pActMsg->nActionType ;
//				msgAdd.nPlayerAllBetMoney = pPeer->m_nBetMoney ;
//				msgAdd.nPlayerLeftMoney = pPeer->GetPlayerBaseData()->nCoin ;
//				msgAdd.nRoomAllBetMoney = m_nAllBetMoney ;
//				msgAdd.nRoomSingleBetMoney = m_nSingleBetMoney ;
//				SendMsgToRoomPeers((char*)&msgAdd,sizeof(msgAdd),NULL) ;
//			}
//			// go to next turn ;
//			NextTurn();
//		}
//		break;
//	case eRoomPeerAction_PK:
//		{
//			if ( (pActMsg->nArgument < 0 || pActMsg->nArgument >= MAX_ROOM_PEER) || m_pAllPeers[pActMsg->nArgument] == NULL )
//			{
//				CLogMgr::SharedLogMgr()->ErrorLog("Error target idx = %d",pActMsg->nArgument);
//				return ;
//			}
//
//			stMsgRoomPlayerPK msgPK ;
//			msgPK.nActionType = eRoomPeerAction_PK ;
//			msgPK.nPeerIdx = pPeer->m_nPeerIdx ;
//			
//			CRoomPeer* pPKTarget = m_pAllPeers[pActMsg->nArgument] ;
//			int nBet = m_nSingleBetMoney * (pPKTarget->m_nTimesMoneyForPK + pPeer->m_bViewdCard ? 1 : 0 );
//			pPeer->AddBet(nBet) ;
//			m_nAllBetMoney += nBet ;
//			bool bwin = pPeer->m_PeerCard.PKPeerCard(&pPKTarget->m_PeerCard) ;
//			if ( bwin )
//			{
//				pPKTarget->m_eState = CRoomPeer::eState_Failed ;
//			}
//			else
//			{
//				pPeer->m_eState = CRoomPeer::eState_Failed ;
//			}
//
//			msgPK.nPlayerAllBetMoney = pPeer->m_nBetMoney ;
//			msgPK.nPlayerLeftMoney = pPeer->GetPlayerBaseData()->nCoin ;
//			msgPK.nRoomAllBetMoney = m_nAllBetMoney ;
//			msgPK.bWin = bwin ;
//			msgPK.uTargetIdx = pPKTarget->m_nPeerIdx ;
//			SendMsgToRoomPeers((char*)&msgPK,sizeof(msgPK),NULL) ;
//			m_pTimerWaitPK->Reset();
//			m_pTimerWaitPK->Start() ;
//		}
//		break;
//	case eRoomPeerAction_GiveUp:
//		{
//			pPeer->m_eState = CRoomPeer::eState_GiveUp ;
//			stMsgRoomPlayerCmd cmdmsg ;
//			cmdmsg.nActionType = pActMsg->nActionType ;
//			cmdmsg.nPeerIdx = pPeer->m_nPeerIdx ;
//			SendMsgToRoomPeers((char*)&cmdmsg,sizeof(cmdmsg),NULL) ;
//			// go to next turn ;
//			if ( m_nCurActionPeerIndex == pPeer->m_nPeerIdx )
//			{
//				NextTurn();
//			}
//		}
//		break;
//	case eRoomPeerAction_ShowCard:
//		{
//			stMsgShowCard* msgShow = (stMsgShowCard*)pActMsg ;
//
//			if ( (msgShow->vShowCard[0] < 0 || msgShow->vShowCard[0] > 52) && msgShow->vShowCard[1] < 0 || msgShow->vShowCard[1] > 52)
//			{
//				CLogMgr::SharedLogMgr()->ErrorLog("Can not Show card ,Number error") ;
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 3 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet));
//				return ;
//			}
//			for ( int i = 0 ; i < PEER_CARD_COUNT -1 ; ++i )
//			{
//				if (msgShow->vShowCard[i] < 0 || msgShow->vShowCard[i] > 52)
//					continue  ;
//				pPeer->m_PeerCard.ShowCardByNumber(msgShow->vShowCard[i]) ;
//			}
//			
//			stMsgRoomPlayerShowCard msgRoomShowCard ;
//			msgRoomShowCard.nPeerIdx = pPeer->m_nPeerIdx ;
//			for ( int i = 0 ; i < PEER_CARD_COUNT -1 ; ++i )
//			{
//				msgRoomShowCard.vShowCard[i] = msgShow->vShowCard[i] ;
//			}
//			msgRoomShowCard.nActionType = pActMsg->nActionType ;
//			SendMsgToRoomPeers((char*)&msgRoomShowCard,sizeof(msgRoomShowCard),NULL) ;
//		}
//		break;
//	case eRoomPeerAction_TimesMoneyPk:
//		{
//			pPeer->m_nTimesMoneyForPK = pActMsg->nArgument ;
//
//			stMsgRoomPlayerTimesPk msgTimesPK ;
//			msgTimesPK.nActionType = pActMsg->nActionType ;
//			msgTimesPK.nPeerIdx = pPeer->m_nPeerIdx ;
//			msgTimesPK.nTimesPK = pPeer->m_nTimesMoneyForPK ;
//			SendMsgToRoomPeers((char*)&msgTimesPK,sizeof(msgTimesPK),NULL) ;
//		}
//		break;
//	case eRoomPeerAction_ViewCard:
//		{
//			stMsgRoomPlayerCmd cmdmsg ;
//			cmdmsg.nActionType = pActMsg->nActionType ;
//			cmdmsg.nPeerIdx = pPeer->m_nPeerIdx ;
//			SendMsgToRoomPeers((char*)&cmdmsg,sizeof(cmdmsg),NULL) ;
//			pPeer->m_bViewdCard = true ;
//		}
//		break;
//	case eRoomPeerAction_Speak_Text:
//		{
//			stMsgActionSpeak* pSpeakMsg = (stMsgActionSpeak*)pActMsg ;
//			stMsgRoomPlayerSpeakText pSendMsg ;
//			pSendMsg.nActionType = pActMsg->nActionType ;
//			pSendMsg.nTextLen = pSpeakMsg->nArgument ;
//			pSendMsg.nPeerIdx = pPeer->m_nPeerIdx ;
//			pSendMsg.pContent = NULL ;
//				
//			char * pBuffer = new char[ sizeof(pSendMsg) + sizeof(char) * pSendMsg.nTextLen ] ;
//			memcpy(pBuffer,&pSendMsg,sizeof(pSendMsg));
//			memcpy(pBuffer + sizeof(pSendMsg),((char*)&pActMsg) + sizeof(stMsgActionSpeak),  pSendMsg.nTextLen );
//			SendMsgToRoomPeers(pBuffer,sizeof(pSendMsg) + sizeof(char) *  pSendMsg.nTextLen,NULL) ;
//			delete[]pBuffer ;
//		}
//		break; 
//	case eRoomPeerAction_Speak_Default:
//		{
//			stMsgActionSpeak* pSpeakMsg = (stMsgActionSpeak*)pActMsg ;
//			stMsgRoomPlayerSpeakDefault pSendMsg ;
//			pSendMsg.nActionType = pActMsg->nActionType ;
//			pSendMsg.nPeerIdx = pPeer->m_nPeerIdx ;
//			pSendMsg.bText = pSpeakMsg->bText ;
//			pSendMsg.nDefaultID = pSpeakMsg->nArgument ;
//			SendMsgToRoomPeers((char*)&pSendMsg,sizeof(pSendMsg),NULL) ;
//		}
//		break;
//	default:
//		{
//			CLogMgr::SharedLogMgr()->ErrorLog("Unknown player action type , playerName = %s",pPeer->GetPlayerBaseData()->strName);
//		} 
//	}
//}
//
//void CRoom::DistributeCard()
//{
//	int nReadyCount = GetReadyPeerCount();
//	if (  nReadyCount < 2 )
//		return ;
//	m_pTimerToReady->Stop() ;
//	m_Poker.ComfirmKeepCard(nReadyCount * PEER_CARD_COUNT * 2 ) ;
//
//	stGetCardPeer* pVGetCardPeer = new stGetCardPeer[nReadyCount] ;
//
//	for ( int i = 0, nIdx = 0 ; i < MAX_ROOM_PEER ; ++i, ++nIdx )
//	{
//		if ( NULL == m_pAllPeers[i])
//			continue ;
//		
//		if ( CRoomPeer::eState_Ready == m_pAllPeers[i]->m_eState )
//		{
//			pVGetCardPeer[nIdx].nPeerIdx = m_pAllPeers[i]->m_nPeerIdx ;
//			for ( int n = 0 ; n < PEER_CARD_COUNT ; ++ n )
//			{
//				pVGetCardPeer[nIdx].vCard[n] = m_Poker.GetCardWithCompositeNum();
//			}
//			
//			m_pAllPeers[i]->OnGetCard(pVGetCardPeer[nIdx].vCard[0],pVGetCardPeer[nIdx].vCard[1],pVGetCardPeer[nIdx].vCard[2]) ;
//			m_pAllPeers[i]->m_eState =  CRoomPeer::eState_Playing ;
//		}
//	}
//
//	UpdateMainPlayerIndex();
//	m_nCurActionPeerIndex = m_nMainPeerIndex ;
//	// send msg ;
//	stMsgRoomDistributeCard msg ;
//	msg.nMainPlayerIndex = m_nMainPeerIndex ;
//	msg.nCount = nReadyCount ;
//	int nLen = sizeof(msg) + nReadyCount * sizeof(stGetCardPeer) ;
//	char* pBuffer = new char[nLen] ;
//	memcpy(pBuffer,&msg,sizeof(msg));
//	memcpy(pBuffer + sizeof(msg),pVGetCardPeer,nLen - sizeof(msg));
//	SendMsgToRoomPeers(pBuffer,nLen,NULL) ;
//	delete[] pBuffer ;
//	delete[] pVGetCardPeer ;
//	m_eState = eRoomState_Playing ;
//}
//
//bool CRoom::CheckAllPlayerReady()
//{
//	for ( int i = 0 ; i <MAX_ROOM_PEER ; ++i )
//	{
//		if ( m_pAllPeers[i] == NULL )
//			continue ;
//		if ( CRoomPeer::eState_Ready != m_pAllPeers[i]->m_eState )
//			return false ;
//	}
//	return true ;
//}
//
//unsigned char CRoom::GetReadyPeerCount()
//{
//	unsigned char nCount = 0 ;
//	for ( int i = 0 ; i <MAX_ROOM_PEER ; ++i )
//	{
//		if ( m_pAllPeers[i] == NULL )
//			continue ;
//		if ( CRoomPeer::eState_Ready == m_pAllPeers[i]->m_eState )
//		{
//			++nCount ;
//		}
//	}
//	return nCount ;
//}
//
//bool CRoom::NextActionPeerIdx()
//{
//	// check if just left one ;
//	int iLeftNum = 0 ;
//	unsigned char leftIdx = -1 ;
//	for ( int ndx = 0 ; ndx < MAX_ROOM_PEER; ++ndx )
//	{
//		if ( m_pAllPeers[ndx] && m_pAllPeers[ndx]->m_eState == CRoomPeer::eState_Playing )
//		{
//			++iLeftNum ;
//			leftIdx = ndx ;
//		}
//	}
//	if ( iLeftNum == 1 )
//	{
//		FinishThisMatch(m_pAllPeers[leftIdx]) ;
//		return false ;
//	}
//	// next ;
//	unsigned char nNextId = -1 ;
//	for ( int ndx = m_nCurActionPeerIndex +1 ; ndx < MAX_ROOM_PEER; ++ndx )
//	{
//		if ( m_pAllPeers[ndx] && m_pAllPeers[ndx]->m_eState == CRoomPeer::eState_Playing )
//		{
//			nNextId = ndx ;
//			break;
//		}
//	}
//	if ( nNextId >= 0 )
//	{
//		m_nCurActionPeerIndex = nNextId ;
//		return true;
//	}
//
//	for ( int ndx = MAX_ROOM_PEER - 1 ; ndx >= 0; --ndx )
//	{
//		if ( m_pAllPeers[ndx] && m_pAllPeers[ndx]->m_eState == CRoomPeer::eState_Playing )
//		{
//			nNextId = ndx ;
//			break;
//		}
//	}
//
//	if ( nNextId >= 0 )
//	{
//		m_nCurActionPeerIndex = nNextId ;
//		return true;
//	}
//
//	CLogMgr::SharedLogMgr()->ErrorLog("impossible error inCRoom::NextActionPeerIdx() ");
//	return true ;
//}
//
//bool CRoom::CanPlayerDoThisAction(eRoomPeerAction eAction , CRoomPeer* pPeer )
//{
//	switch ( eAction )
//	{
//	case eRoomPeerAction_Add:
//	case eRoomPeerAction_PK:
//	case eRoomPeerAction_GiveUp:
//		{
//			if ( pPeer->m_eState != CRoomPeer::eState_Playing )
//			{
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 1 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet));
//				return false;
//			}
//
//			if ( pPeer->m_nPeerIdx != m_nCurActionPeerIndex )
//			{
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 2 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet)) ;
//				return false;
//			}
//		}
//		break;
//	case eRoomPeerAction_ShowCard:
//		{
//			if ( HasFlag(eRoomFlag_ShowCard) == false )
//			{
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 4 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet));
//				return false;
//			}
//		}
//	case eRoomPeerAction_TimesMoneyPk:
//		{
//			if ( HasFlag(eRoomFlag_TimesPK) == false )
//			{
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 4 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet));
//				return false;
//			}
//		}
//	case eRoomPeerAction_ViewCard:
//		{
//			if ( pPeer->m_eState != CRoomPeer::eState_Playing )
//			{
//				stMsgActionRet msgRet ;
//				msgRet.nErr = 1 ;
//				pPeer->SendMsgToClient((char*)&msgRet,sizeof(msgRet));
//				return false;
//			}
//		}
//		break;
//	}
//	return true ;
//}
//
//void CRoom::NextTurn()
//{
//	if ( NextActionPeerIdx() )
//	{
//		m_pTimerToAction->Reset();
//		m_pTimerToAction->Start();
//		stMsgRoomPlayerActionTurn actMsg ;
//		actMsg.nPlayerIdx = m_nCurActionPeerIndex ;
//		SendMsgToRoomPeers((char*)&actMsg,sizeof(actMsg),NULL) ;
//	}
//}
//
//void CRoom::FinishThisMatch(CRoomPeer* peerWin )
//{
//	stMsgRoomResult msg ;
//	msg.nWinPlayerIdx = peerWin->m_nPeerIdx ;
//	msg.nWinMoney = m_nAllBetMoney ;
//	SendMsgToRoomPeers((char*)&msg,sizeof(msg),NULL) ;
//	m_pTimerWaitFinish->Reset() ;
//	m_pTimerWaitFinish->Start() ;
//	m_eState = eRoomState_End ;
//}
//
//void CRoom::OnRestarMatch()
//{
//	for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
//	{
//		if ( m_pAllPeers[i] == NULL )
//			continue ;
//		m_pAllPeers[i]->Reset();
//	}
//
//	m_pTimerToAction->Stop();
//	m_pTimerWaitDistribute->Stop() ;
//	m_pTimerWaitPK->Stop();
//	m_pTimerWaitFinish->Stop() ;
//	m_pTimerToReady->Reset();
//	m_pTimerToReady->Start() ;
//
//	m_nMainPeerIndex = 0 ;
//	m_nCurActionPeerIndex = m_nMainPeerIndex ;
//	m_eState = eRoomState_Wait ;
//
//	m_nAllBetMoney = 0 ;
//	m_nRound = 1 ;
//	m_nSingleBetMoney = m_nBaseBetMoney ;
//}
//
//void CRoom::UpdateMainPlayerIndex()
//{
//	unsigned char uMainIdx = m_nMainPeerIndex + 1 ;
//	for ( int uMainIdx = 0 ;  uMainIdx < MAX_ROOM_PEER ; ++uMainIdx )
//	{
//		if ( m_pAllPeers[uMainIdx] && m_pAllPeers[uMainIdx]->m_eState == CRoomPeer::eState_Ready )
//		{
//			m_nMainPeerIndex = uMainIdx ;
//			return ;
//		}
//	}
//
//	for ( int uMainIdx = MAX_ROOM_PEER - 1 ;  uMainIdx >=0 ; --uMainIdx )
//	{
//		if ( m_pAllPeers[uMainIdx] && m_pAllPeers[uMainIdx]->m_eState == CRoomPeer::eState_Ready && uMainIdx != m_nMainPeerIndex )
//		{
//			m_nMainPeerIndex = uMainIdx ;
//			return ;
//		}
//	}
//}