//
//  SelectAddBetCoinLoader.h
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#ifndef __Gold__SelectAddBetCoinLoader__
#define __Gold__SelectAddBetCoinLoader__
#include "cocos-ext.h"
#include "SelectAddBetCoin.h"
USING_NS_CC_EXT ;
class CSelectAddBetCoinLoader
:public CCSpriteLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CSelectAddBetCoinLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CSelectAddBetCoin);
} ;

#endif /* defined(__Gold__SelectAddBetCoinLoader__) */
