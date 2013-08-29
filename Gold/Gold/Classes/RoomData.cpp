//
//  RoomData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#include "RoomData.h"
#include "RoomLayer.h"
#include "ClientApp.h"
#include "PlayerBaseData.h"
#include "ResultDlg.h"
CRoomData::~CRoomData()
{
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        if ( m_vRoomPeers[i] )
        {
            delete m_vRoomPeers[i] ;
            m_vRoomPeers[i] = NULL ;
        }
    }
}

void CRoomData::Init(CRoomLayer* pLayer, int nBaseSingle )
{
    m_pRoomLayer = pLayer ;
    memset(m_vRoomPeers, 0, sizeof(m_vRoomPeers)) ;
    m_nSingleBetCoin = 0 ;
    m_nTotalBetCoin = 0 ;
    m_nRound = 0 ;
    m_nWaitSessionID = 0 ;
    m_eRoomSate = eRoomState_None ;
    m_nBaseSingle  = nBaseSingle ; 
}

bool CRoomData::OnMessage( RakNet::Packet* pRakMsg )
{
    stMsg* pMsg = (stMsg*)pRakMsg->data ;
    switch (pMsg->usMsgType)
    {
        case MSG_ROOM_PLAYER_ENTER:
        {
            stMsgRoomPlayerEnter* pRet = (stMsgRoomPlayerEnter*)pMsg ;
            if ( NULL == m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx] )
            {
                m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx] = new stGoldRoomPeerData ;
            }
            m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx]->SetData(&pRet->nEnterPlayerInfo) ;
            m_pRoomLayer->OnPlayerEnter(ConvertToClientIdx(pRet->nEnterPlayerInfo.nIdx), m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx]) ;
        }
            break;
        case MSG_ROOM_PLAYER_LEAVE:
        {
            stMsgRoomPlayerLeave* pRet = (stMsgRoomPlayerLeave*)pMsg ;
            stGoldRoomPeerData* pData = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            if (pData)
            {
                m_pRoomLayer->OnPlayerLeave(ConvertToClientIdx(pData->getIdx())) ;
                pData->setSessionID(0);
            }
            else
            {
                 
            }
        }
            break ;
        case MSG_ROOM_CURRENT_INFO:
        {
            stMsgRoomCurInfo* pRet = (stMsgRoomCurInfo*)pMsg ;
            m_nSingleBetCoin = pRet->nSingleBetCoin ;
            m_nTotalBetCoin = pRet->nTotalBetCoin ;
            m_nRound = pRet->nRound ;
            m_nRooID = pRet->nRoomID ;
            m_eRoomSate = pRet->eRoomSate ;
            char* pbuffer = (char*)pMsg ;
            pbuffer += sizeof(stMsgRoomCurInfo) ;
            stRoomPeerBrifInfo* pInfos = NULL ;
            for ( int i = 0 ; i < pRet->nPlayerCount ; ++i )
            {
                pInfos = (stRoomPeerBrifInfo*)pbuffer ;
                if ( NULL == m_vRoomPeers[pInfos->nIdx] )
                {
                    m_vRoomPeers[pInfos->nIdx] = new stGoldRoomPeerData ;
                }
                m_vRoomPeers[pInfos->nIdx]->SetData(pInfos) ;
                pbuffer += sizeof(stRoomPeerBrifInfo) ;
            }
            // create self ;
            if ( m_vRoomPeers[pRet->nSelfIdx] == NULL )
            {
                m_vRoomPeers[pRet->nSelfIdx] = new stGoldRoomSelfData ;
            }
            m_vRoomPeers[pRet->nSelfIdx]->setIdx(pRet->nSelfIdx) ;
            m_pRoomLayer->OnRefreshRoomInfo(this) ;
        }
            break ;
        case MSG_ROOM_PLAYER_READY:
        {
            stMsgRoomPlayerReady* pRet = (stMsgRoomPlayerReady*)pMsg ;
            stGoldRoomPeerData* pData = GetRoomPeerDataBySessionID(pRet->nReadyPlayerSessionID) ;
            if ( pData )
            {
                pData->setPeerState(eRoomPeer_Ready);
                m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pData->getIdx()), eRoomPeer_Ready) ;
            }
            else
            {
                CCMessageBox("MSG_ROOM_PLAYER_READY error", "tip") ;
            }
        }
            break ;
        case MSG_ROOM_DISTRIBUTE_CARD:
        {
            // change ready peer's state to unlook
            for ( int i = 0 ; i < MAX_ROOM_PEER -1 ; ++i )
            {
                if ( m_vRoomPeers[i] && m_vRoomPeers[i]->getSessionID() && m_vRoomPeers[i]->getPeerState() == eRoomPeer_Ready )
                {
                    m_vRoomPeers[i]->setPeerState(eRoomPeer_Unlook);
                }
            }
            m_eRoomSate = eRoomState_DistributeCard ;
            m_pRoomLayer->OnDistributeCard();
        }
            break ;
        case MSG_ROOM_WAIT_PLAYER_ACTION:
        {
            m_eRoomSate = eRoomState_WaitPeerAction ;
            m_pRoomLayer->OnStopTiming() ;
            stMsgRoomWaitPlayerAction* pRet = (stMsgRoomWaitPlayerAction*)pMsg ;
            m_nRound = pRet->nRound ;
            m_nWaitSessionID = pRet->nSessionID ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            if ( pdata )
            {
                m_pRoomLayer->OnWaitPlayerAction(ConvertToClientIdx(pdata->getIdx())) ;
            }
            else
            {
                CCMessageBox("MSG_ROOM_WAIT_PLAYER_ACTION error", "tip") ;
            }
        }
            break ;
        case MSG_ROOM_PLAYER_FOLLOW:
        {
            stMsgRoomPlayerFollow* pRet = (stMsgRoomPlayerFollow*)pMsg ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            
            int nBetCoin = m_nSingleBetCoin ;
            if ( pdata->getPeerState() == eRoomPeer_Look )
            {
                nBetCoin *= 2 ;
            }
            
            if ( pdata->getCoin() < nBetCoin )
            {
                nBetCoin = pdata->getCoin() ;
            }
            pdata->setBetCoin(pdata->getBetCoin() + nBetCoin ) ;
            pdata->setCoin(pdata->getCoin() - nBetCoin) ;
            m_nTotalBetCoin += nBetCoin ;
            m_pRoomLayer->OnPlayerFollow(ConvertToClientIdx(pdata->getIdx()),nBetCoin) ;
        }
            break ;
        case MSG_ROOM_PLAYER_ADD:
        {
            stMsgRoomPlayerAdd* pRet = (stMsgRoomPlayerAdd*)pMsg ;
            m_nSingleBetCoin = pRet->nNewSingle ;
            m_nTotalBetCoin += pRet->nBetCoin ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            pdata->setBetCoin(pdata->getBetCoin() + pRet->nBetCoin) ;
            pdata->setCoin(pdata->getCoin() - pRet->nBetCoin) ;
            m_pRoomLayer->OnPlayerAdd(ConvertToClientIdx(pdata->getIdx()),pRet->nBetCoin) ;
        }
            break ;
        case MSG_ROOM_PLAYER_LOOK:
        {
            stMsgRoomPlayerLook* pRet = (stMsgRoomPlayerLook*)pMsg ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            if ( CClientApp::SharedClientApp()->GetSessionID() == pRet->nSessionID )
            {
                stGoldRoomSelfData* pdataself = (stGoldRoomSelfData*)pdata ;
                memcpy(pdataself->vCard, pRet->vCard, sizeof(pRet->vCard)) ;
            }
            pdata->setPeerState(eRoomPeer_Look);
            m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pdata->getIdx()), eRoomPeer_Look) ;
        }
            break ;
        case MSG_ROOM_PLAYER_PK:
        {
            stMsgRoomPlayerPK* pRet = (stMsgRoomPlayerPK*)pMsg ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nPKInvokeSessionID) ;
            stGoldRoomPeerData* pdata2 = GetRoomPeerDataBySessionID(pRet->nPKWithSessionID) ;
            pdata->AddBetCoin(pRet->nConsumCoin);
            m_nTotalBetCoin += pRet->nConsumCoin ;
            m_pRoomLayer->OnPlayerPK(ConvertToClientIdx(pdata->getIdx()) , ConvertToClientIdx(pdata2->getIdx()), pRet->bWin ) ;
            if ( pRet->bWin )
            {
                pdata2->setPeerState(eRoomPeer_Failed);
            }
            else
            {
                pdata->setPeerState(eRoomPeer_Failed);
            }
        }
            break ;
        case MSG_ROOM_PLAYER_GIVEUP:
        {
            stMsgRoomPlayerGiveUp* pRet = (stMsgRoomPlayerGiveUp*)pMsg ;
            stGoldRoomPeerData* pdata = GetRoomPeerDataByClientIdx(ConvertToClientIdx(pRet->nIdx)) ;
            if ( pdata )
            {
                pdata->setPeerState(eRoomPeer_GiveUp) ;
            }
            m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pRet->nIdx), eRoomPeer_GiveUp) ;
        }
            break ;
        case MSG_ROOM_RESULT:
        {
            stMsgRoomResult* pRet = (stMsgRoomResult*)pMsg ;
            std::list<stResultData*> vListData ;
            char* pBuffer = (char*)pMsg;
            pBuffer += sizeof(stMsgRoomResult) ;
            for ( int i = 0 ; i < pRet->nCount ; ++i )
            {
                stResultData* pdata = (stResultData*)pBuffer ;
                vListData.push_back(pdata) ;
                m_vRoomPeers[pdata->idx]->setBetCoin(0) ;
                m_vRoomPeers[pdata->idx]->setCoin(m_vRoomPeers[pdata->idx]->getCoin() + pdata->nResultCoin) ;
                m_vRoomPeers[pdata->idx]->setPeerState(eRoomPeer_None);
                
                pBuffer += sizeof(stResultData) ;
            }
            RestState();
            // show result dlg ; vListData pass to it ;
            CResultDlg* pdlg = new CResultDlg ;
            pdlg->init() ;
            m_pRoomLayer->addChild(pdlg) ;
            pdlg->autorelease() ;
            pdlg->ShowDlg(vListData, this) ;
         }
            break ;
        default:
            break;
    }
    return false ;
}

void CRoomData::RestState()
{
    m_nSingleBetCoin = m_nBaseSingle ;
    m_nTotalBetCoin = 0 ;
    m_nRound = 0 ;
    m_eRoomSate = eRoomState_WaitPeerToJoin ;
    m_nWaitSessionID = 0 ;
    stGoldRoomPeerData* pdata = NULL ;
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        pdata = GetRoomPeerDataByClientIdx(i) ;
        if (pdata )
        {
            pdata->setPeerState(eRoomPeer_None);
            pdata->setBetCoin(0);
        }
    }
    m_pRoomLayer->OnRefreshRoomInfo(this);
}

stGoldRoomPeerData* CRoomData::GetRoomPeerDataBySessionID( unsigned int nSessionID )
{
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        if ( m_vRoomPeers[i] && m_vRoomPeers[i]->getSessionID() == nSessionID )
        {
            return m_vRoomPeers[i] ;
        }
    }
    return NULL ;
}

stGoldRoomPeerData* CRoomData::GetRoomPeerDataByClientIdx( char nClientIdx )
{    
    char nServerIdx = ConvertoServerIdx(nClientIdx) ;
    if ( nServerIdx < 0 || nServerIdx >= MAX_ROOM_PEER )
    {
        return NULL ;
    }
    if ( m_vRoomPeers[nServerIdx] && m_vRoomPeers[nServerIdx]->getSessionID() )
        return m_vRoomPeers[nServerIdx] ;
    return NULL ;
}

bool CRoomData::IsWaitMyTurn()
{
    return CClientApp::SharedClientApp()->GetSessionID() == m_nWaitSessionID ;
}

char CRoomData::ConvertoServerIdx(char nClientIdx)
{
    stGoldRoomPeerData* pmydata = GetRoomPeerDataBySessionID(CClientApp::SharedClientApp()->GetSessionID()) ;
    int nMyServerIdx = pmydata->getIdx() ;
    if ( nClientIdx == MAX_ROOM_PEER -1 )
        return nMyServerIdx ;
    char nOffset = MAX_ROOM_PEER -1  - nClientIdx ;
    char nServerIdx =  nMyServerIdx - nOffset ;
    if ( nServerIdx < 0  )
        nServerIdx += MAX_ROOM_PEER ;
    return nServerIdx ;
}

char CRoomData::ConvertToClientIdx(char nServerIdx )
{
    stGoldRoomPeerData* pmydata = GetRoomPeerDataBySessionID(CClientApp::SharedClientApp()->GetSessionID()) ;
    char nMyServerIdx = pmydata->getIdx();
    char nOffset = nMyServerIdx - nServerIdx ;
    char nClientIdx = MAX_ROOM_PEER - 1 - nOffset  ;
    if ( nClientIdx < 0 )
        nClientIdx += MAX_ROOM_PEER ;
    if ( nClientIdx >= MAX_ROOM_PEER )
        nClientIdx -= MAX_ROOM_PEER ;
    return nClientIdx ;
}

char CRoomData::GetPlayerCount()
{
    int nCount = 0 ;
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        if ( m_vRoomPeers[i] && m_vRoomPeers[i]->getSessionID() )
        {
            ++nCount ;
        }
    }
    return nCount ;
}

char CRoomData::GetActiveCount()
{
    int nCount = 0 ;
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        if ( m_vRoomPeers[i] && m_vRoomPeers[i]->getSessionID() && m_vRoomPeers[i]->IsActive() )
        {
            ++nCount ;
        }
    }
    return nCount ;
}