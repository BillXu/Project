//
//  RoomPlayerInforLoader.h
//  Gold
//
//  Created by Xu BILL on 13-7-27.
//
//

#ifndef __Gold__RoomPlayerInforLoader__
#define __Gold__RoomPlayerInforLoader__

//#include <iostream>
#include "cocos-ext.h"
#include "RoomPlayerInfor.h"
USING_NS_CC_EXT ;
class CRoomPlayerInforLoader
:public CCSpriteLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CRoomPlayerInforLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CRoomPlayerInfor);
} ;
#endif /* defined(__Gold__RoomPlayerInforLoader__) */
