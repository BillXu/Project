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
bool stRoomPeerData::IsActive()
{
    return (ePeerState == eRoomPeer_Look || ePeerState == eRoomPeer_Unlook );
}

void CRoomData::Init(CRoomLayer* pLayer, int nBaseSingle )
{
    m_pRoomLayer = pLayer ;
    memset(m_vRoomPeers, 0, sizeof(m_vRoomPeers)) ;
    m_nSingleBetCoin = 0 ;
    m_nTotalBetCoin = 0 ;
    m_nRound = 0 ;
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
            memcpy(&m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx], &pRet->nEnterPlayerInfo, sizeof(stRoomPeerBrifInfo)) ;
            m_pRoomLayer->OnPlayerEnter(ConvertToClientIdx(pRet->nEnterPlayerInfo.nIdx), &m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx]) ;
        }
            break;
        case MSG_ROOM_PLAYER_LEAVE:
        {
            stMsgRoomPlayerLeave* pRet = (stMsgRoomPlayerLeave*)pMsg ;
            stRoomPeerData* pData = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            if (pData)
            {
                m_pRoomLayer->OnPlayerLeave(ConvertToClientIdx(pData->nIdx)) ;
                pData->nSessionID = 0 ;
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
            m_eRoomSate = pRet->eRoomSate ;
            char* pbuffer = (char*)pMsg ;
            pbuffer += sizeof(stMsgRoomCurInfo) ;
            stRoomPeerBrifInfo* pInfos = NULL ;
            for ( int i = 0 ; i < pRet->nPlayerCount ; ++i )
            {
                pInfos = (stRoomPeerBrifInfo*)pbuffer ;
                memcpy(&m_vRoomPeers[pInfos->nIdx], pInfos, sizeof(stRoomPeerBrifInfo)) ;
                pbuffer += sizeof(stRoomPeerBrifInfo) ;
            }
            CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState = eRoomPeer_None ;
            m_pRoomLayer->OnRefreshRoomInfo(this) ;
        }
            break ;
        case MSG_ROOM_PLAYER_READY:
        {
            stMsgRoomPlayerReady* pRet = (stMsgRoomPlayerReady*)pMsg ;
            stRoomPeerData* pData = GetRoomPeerDataBySessionID(pRet->nReadyPlayerSessionID) ;
            if ( pData )
            {
                pData->ePeerState = eRoomPeer_Ready ;
                m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pData->nIdx), eRoomPeer_Ready) ;
            }
            else
            {
                if ( pRet->nReadyPlayerSessionID ==  CClientApp::SharedClientApp()->GetSessionID() )
                {
                    CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState = eRoomPeer_Ready ;
                    m_pRoomLayer->OnUpdatePlayerState(4, eRoomPeer_Ready) ;
                }
            }
        }
            break ;
        case MSG_ROOM_DISTRIBUTE_CARD:
        {
            // change ready peer's state to unlook
            for ( int i = 0 ; i < MAX_ROOM_PEER -1 ; ++i )
            {
                if ( m_vRoomPeers[i].nSessionID && m_vRoomPeers[i].ePeerState == eRoomPeer_Ready )
                {
                    m_vRoomPeers[i].ePeerState = eRoomPeer_Unlook;
                }
            }
            
            if ( CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState == eRoomPeer_Ready )
            {
                CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState = eRoomPeer_Unlook ;
            }
            m_eRoomSate = eRoomState_DistributeCard ;
            m_pRoomLayer->OnDistributeCard();
        }
            break ;
        case MSG_ROOM_WAIT_PLAYER_ACTION:
        {
            m_eRoomSate = eRoomState_WaitPeerAction ;
            m_pRoomLayer->StopMyClock() ;
            stMsgRoomWaitPlayerAction* pRet = (stMsgRoomWaitPlayerAction*)pMsg ;
            m_nRound = pRet->nRound ;
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            if ( pdata )
            {
                m_pRoomLayer->OnWaitPlayerAction(ConvertToClientIdx(pdata->nIdx)) ;
            }
            else
            {
                if ( pRet->nSessionID ==  CClientApp::SharedClientApp()->GetSessionID() )
                {
                    m_pRoomLayer->OnWaitPlayerAction(4) ;
                }
            }
        }
            break ;
        case MSG_ROOM_PLAYER_FOLLOW:
        {
            stMsgRoomPlayerFollow* pRet = (stMsgRoomPlayerFollow*)pMsg ;
            if ( CClientApp::SharedClientApp()->GetSessionID() == pRet->nSessionID )
            {
                int nBetCoin = m_nSingleBetCoin ;
                if ( CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState == eRoomPeer_Look )
                {
                    nBetCoin *= 2 ;
                }
                
                if ( CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nCoin < nBetCoin )
                {
                    nBetCoin = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nCoin ;
                }
                CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nBetCoin += nBetCoin ;
                CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nCoin -= nBetCoin ;
                m_nTotalBetCoin += nBetCoin ;
                m_pRoomLayer->OnPlayerFollow(4,nBetCoin) ;
                return true ;
            }
            
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            
            int nBetCoin = m_nSingleBetCoin ;
            if ( pdata->ePeerState == eRoomPeer_Look )
            {
                nBetCoin *= 2 ;
            }
            
            if ( pdata->nCoin < nBetCoin )
            {
                nBetCoin = pdata->nCoin ;
            }
            pdata->nBetCoin += nBetCoin ;
            pdata->nCoin -= nBetCoin ;
            m_nTotalBetCoin += nBetCoin ;
            
            m_pRoomLayer->OnPlayerFollow(ConvertToClientIdx(pdata->nIdx),nBetCoin) ;
        }
            break ;
        case MSG_ROOM_PLAYER_ADD:
        {
            stMsgRoomPlayerAdd* pRet = (stMsgRoomPlayerAdd*)pMsg ;
            m_nSingleBetCoin = pRet->nNewSingle ;
            m_nTotalBetCoin += pRet->nBetCoin ;
            if ( pRet->nSessionID == CClientApp::SharedClientApp()->GetSessionID() )
            {
                // self
                CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
                pbasedata->nBetCoin += pRet->nBetCoin ;
                pbasedata->nCoin -= pRet->nBetCoin ;
                m_pRoomLayer->OnPlayerAdd(4, pRet->nBetCoin) ;
            }
            else
            {
                stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
                pdata->nBetCoin += pRet->nBetCoin ;
                pdata->nCoin -= pRet->nBetCoin ;
                m_pRoomLayer->OnPlayerAdd(ConvertToClientIdx(pdata->nIdx),pRet->nBetCoin) ;
            }
            
        }
            break ;
        case MSG_ROOM_PLAYER_LOOK:
        {
            stMsgRoomPlayerLook* pRet = (stMsgRoomPlayerLook*)pMsg ;
            if ( CClientApp::SharedClientApp()->GetSessionID() == pRet->nSessionID )
            {
                CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState = eRoomPeer_Look ;
                memcpy(CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->vCard, pRet->vCard, sizeof(pRet->vCard)) ;
                m_pRoomLayer->OnUpdatePlayerState(4, eRoomPeer_Look) ;
            }
            else
            {
                stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
                pdata->ePeerState = eRoomPeer_Look ;
                m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pdata->nIdx), eRoomPeer_Look) ;
            }
        }
            break ;
        case MSG_ROOM_PLAYER_PK:
        {
            stMsgRoomPlayerPK* pRet = (stMsgRoomPlayerPK*)pMsg ;
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nPKInvokeSessionID) ;
            stRoomPeerData* pdata2 = GetRoomPeerDataBySessionID(pRet->nPKWithSessionID) ;
            pdata->nBetCoin += pRet->nConsumCoin ;
            pdata->nCoin -= pRet->nConsumCoin ;
            m_nTotalBetCoin += pRet->nConsumCoin ;
            m_pRoomLayer->OnPlayerPK(ConvertToClientIdx(pdata->nIdx) , ConvertToClientIdx(pdata2->nIdx), pRet->bWin ) ;
            if ( pRet->bWin )
            {
                pdata2->ePeerState = eRoomPeer_Failed ;
            }
            else
            {
                pdata->ePeerState = eRoomPeer_Failed ;
            }
        }
            break ;
        case MSG_ROOM_PLAYER_GIVEUP:
        {
            stMsgRoomPlayerGiveUp* pRet = (stMsgRoomPlayerGiveUp*)pMsg ;
            stRoomPeerData* pdata = GetRoomPeerDataByClientIdx(ConvertToClientIdx(pRet->nIdx)) ;
            if ( pdata )
            {
                pdata->ePeerState = eRoomPeer_GiveUp ;
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
            CPlayerBaseData* pBaseData = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData();
            for ( int i = 0 ; i < pRet->nCount ; ++i )
            {
                stResultData* pdata = (stResultData*)pBuffer ;
                vListData.push_back(pdata) ;
                if ( pBaseData->nIdx == pdata->idx )
                {
                    pBaseData->nCoin += pdata->nResultCoin ;
                    pBaseData->nBetCoin = 0 ;
                    pBaseData->ePeerState = eRoomPeer_None;
                }
                else
                {
                    m_vRoomPeers[pdata->idx].nBetCoin = 0 ;
                    m_vRoomPeers[pdata->idx].nCoin += pdata->nResultCoin ;
                    m_vRoomPeers[pdata->idx].ePeerState = eRoomPeer_None;
                }
                pBuffer += sizeof(stResultData) ;
            }
            m_nSingleBetCoin = m_nBaseSingle ;
            m_nTotalBetCoin = 0 ;
            m_nRound = 0 ;
            m_eRoomSate = eRoomState_WaitPeerToJoin ;
            m_pRoomLayer->OnRefreshRoomInfo(this);
            // show result dlg ; vListData pass to it ;
        }
            break ;
        default:
            break;
    }
    return false ;
}

stRoomPeerData* CRoomData::GetRoomPeerDataBySessionID( unsigned int nSessionID )
{
    if ( CClientApp::SharedClientApp()->GetSessionID() == nSessionID )
    {
        return  CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    }
    
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        if ( m_vRoomPeers[i].nSessionID == nSessionID )
        {
            return &m_vRoomPeers[i] ;
        }
    }
    return NULL ;
}

stRoomPeerData* CRoomData::GetRoomPeerDataByClientIdx( char nClientIdx )
{
    if ( nClientIdx == 4 ) // self
    {
        return  CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    }
    
    char nServerIdx = ConvertoServerIdx(nClientIdx) ;
    if ( nServerIdx < 0 || nServerIdx >= MAX_ROOM_PEER )
    {
        return NULL ;
    }
    if ( m_vRoomPeers[nServerIdx].nSessionID )
        return &m_vRoomPeers[nServerIdx] ;
    return NULL ;
}

char CRoomData::ConvertoServerIdx(char nClientIdx)
{
    int nMyServerIdx = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nIdx ;
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
    char nMyServerIdx = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->nIdx ;
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
        if ( m_vRoomPeers[i].nSessionID )
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
        if ( m_vRoomPeers[i].nSessionID && m_vRoomPeers[i].IsActive() )
        {
            ++nCount ;
        }
    }
    return nCount ;
}