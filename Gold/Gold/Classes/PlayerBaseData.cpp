//
//  PlayerBaseData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#include "PlayerBaseData.h"
#include "EventHandleQueue.h"
bool CPlayerBaseData::OnMessage(stMsg *pMessage)
{
    if ( MSG_PLAYER_BASE_DATA == pMessage->usMsgType )
    {
        stMsgPlayerBaseData* pbase = (stMsgPlayerBaseData*)pMessage ;
        setSessionID(pbase->nSessionID) ;
        setCoin(pbase->nCoin) ;
        setDiamoned(pbase->nDiamoned) ;
        setTitle(pbase->nTitle) ;
        setVipLevel(pbase->nVipLevel) ;
        setSex(pbase->nSex) ;
        setDefaulPhotoID(pbase->nDefaulPhotoID) ;
        setYeastodayWinCoin(pbase->nYeastodayWinCoin) ;
        setSingleWinMost(pbase->nSingleWinMost) ;
        setWinTimes(pbase->nWinTimes) ;
        setLoseTimes(pbase->nLoseTimes) ;
        setQQNumber(pbase->nQQNumber) ;
        
        // paser name
        char* pBufferMsg = (char*)pMessage ;
        pBufferMsg += sizeof(stMsgPlayerBaseData) ;
        char pbuffer[512] = { 0 } ;
        memcpy(pbuffer, pBufferMsg, pbase->nNameLen) ;
        pBufferMsg += pbase->nNameLen ;
        setName(pbuffer) ;
        
        memset(pbuffer, 0, sizeof(pbuffer)) ;
        memcpy(pbuffer, pBufferMsg, pbase->nSigureLen) ;
        setSigure(pbuffer) ;
        
        // invoke event ;
        stEventArg arg ;
        arg.pUserData = this ;
        CEventHandleQueue::SharedEventHandleQueue()->SendEvent(eEvent_RecieveBaseData, &arg) ;
    }
    return false ;
}