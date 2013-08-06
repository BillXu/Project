//
//  PlayerBaseData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#include "PlayerBaseData.h"
bool CPlayerBaseData::OnMessage(stMsg *pMessage)
{
    if ( MSG_PLAYER_BASE_DATA == pMessage->usMsgType )
    {
        stMsgPlayerBaseData* pbase = (stMsgPlayerBaseData*)pMessage ;
        nSessionID = pbase->nSessionID ;
        nCoin = 193980;
        sprintf(pName, "MyName") ;
    }
    else if ( MSG_ROOM_ENTER == pMessage->usMsgType )
    {
        stMsgRoomEnterRet* pret = (stMsgRoomEnterRet*)pMessage ;
        nIdx = pret->nIdx ;
        nRoomID = pret->nRoomID ;
    }
    return false ;
}