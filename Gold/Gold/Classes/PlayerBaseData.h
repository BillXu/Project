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
class CPlayerBaseData
:public IPlayerDataBase
,public stRoomPeerBrifInfo
{
public:
    bool OnMessage( stMsg* pMessage );
protected:
    unsigned int nRoomID ;
};

#endif /* defined(__Gold__PlayerBaseData__) */
