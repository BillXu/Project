//
//  RoomData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#include "RoomData.h"
#include "RoomLayer.h"
void CRoomData::Init(CRoomLayer* pLayer )
{
    m_pRoomLayer = pLayer ;
    memset(m_vRoomPeers, 0, sizeof(m_vRoomPeers)) ;
    m_nSingleBetCoin = 0 ;
    m_nTotalBetCoin = 0 ;
    m_nRound = 0 ;
    m_eRoomSate = eRoomState_None ;
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
            m_pRoomLayer->OnPlayerEnter(pRet->nEnterPlayerInfo.nIdx, &m_vRoomPeers[pRet->nEnterPlayerInfo.nIdx]) ;
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
        }
            break ;
        case MSG_ROOM_DISTRIBUTE_CARD:
        {
            m_pRoomLayer->OnDistributeCard();
        }
            break ;
        case MSG_ROOM_WAIT_PLAYER_ACTION:
        {
            stMsgRoomWaitPlayerAction* pRet = (stMsgRoomWaitPlayerAction*)pMsg ;
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            m_nRound = pRet->nRound ;
            m_pRoomLayer->OnWaitPlayerAction(ConvertToClientIdx(pdata->nIdx)) ;
        }
            break ;
        case MSG_ROOM_PLAYER_FOLLOW:
        {
            stMsgRoomPlayerFollow* pRet = (stMsgRoomPlayerFollow*)pMsg ;
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            
            int nBetCoin = m_nSingleBetCoin ;
            if ( pdata->ePeerState == eRoomPeer_Look )
            {
                nBetCoin *= 2 ;
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
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            m_nSingleBetCoin = pRet->nNewSingle ;
            pdata->nBetCoin += pRet->nBetCoin ;
            pdata->nCoin -= pRet->nBetCoin ;
            m_nTotalBetCoin += pRet->nBetCoin ;
            m_pRoomLayer->OnPlayerFollow(ConvertToClientIdx(pdata->nIdx),pRet->nBetCoin) ;
        }
            break ;
        case MSG_ROOM_PLAYER_LOOK:
        {
            stMsgRoomPlayerLook* pRet = (stMsgRoomPlayerLook*)pMsg ;
            stRoomPeerData* pdata = GetRoomPeerDataBySessionID(pRet->nSessionID) ;
            pdata->ePeerState = eRoomPeer_Look ;
            m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pdata->nIdx), eRoomPeer_Look) ;
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
            m_pRoomLayer->OnUpdatePlayerState(ConvertToClientIdx(pdata->nIdx), eRoomPeer_GiveUp) ;
        }
            break ;
        default:
            break;
    }
    return false ;
}

stRoomPeerData* CRoomData::GetRoomPeerDataBySessionID( unsigned int nSessionID )
{
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
    int nMyServerIdx = 3 ;
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
    char nMyServerIdx = 0 ;;
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