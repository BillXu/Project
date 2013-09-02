//
//  SelectRoomLayer.h
//  Gold
//
//  Created by user on 13-8-28.
//
//

#ifndef __Gold__SelectRoomLayer__
#define __Gold__SelectRoomLayer__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "NetWorkManager.h"
#include "EventHandleQueue.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class SelectRoomLayer
:public CCLayer
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CNetMessageDelegate
{
public:
    static CCLayer* CreateLayer();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){ return NULL ;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void OnClose(CCObject* p , CCControlEvent );
    
    void onExit();
    void onEnter();
    
    bool OnMessage( RakNet::Packet* pMsg );
    void OnEvent(stEventArg* arg );
    
    void OnRoom1Down(CCObject*, CCControlEvent);
    
};



#endif /* defined(__Gold__SelectRoomLayer__) */
