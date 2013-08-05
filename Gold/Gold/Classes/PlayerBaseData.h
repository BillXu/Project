//
//  PlayerBaseData.h
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#ifndef __Gold__PlayerBaseData__
#define __Gold__PlayerBaseData__

#include <iostream>
#include "IPlayerData.h"
#include "MessageDefine.h"
#include "RoomData.h"
class CPlayerBaseData
:public IPlayerDataBase
,public stRoomPeerData
{
public:
    bool OnMessage( stMsg* pMessage );
public:
    unsigned int nRoomID ;
    char vCard[3] ;
};

#endif /* defined(__Gold__PlayerBaseData__) */
