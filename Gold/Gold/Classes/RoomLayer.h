//
//  RoomLayer.h
//  Gold
//
//  Created by Xu BILL on 13-7-26.
//
//

#ifndef __Gold__RoomLayer__
#define __Gold__RoomLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CRoomLayer
:public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static CCScene* RoomScene();
    virtual bool init();
    // selector
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){ return NULL ;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    // variable
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
};

#endif /* defined(__Gold__RoomLayer__) */
