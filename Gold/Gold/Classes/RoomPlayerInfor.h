//
//  RoomPlayerInfor.h
//  Gold
//
//  Created by Xu BILL on 13-7-27.
//
//

#ifndef __Gold__RoomPlayerInfor__
#define __Gold__RoomPlayerInfor__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
//#include <iostream>
class CRoomPlayerInfor
:public CCSprite
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCNodeLoaderListener
{
public:
   static CRoomPlayerInfor* create();
    // selector
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) ;
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    // variable
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    // listener
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
};
#endif /* defined(__Gold__RoomPlayerInfor__) */
