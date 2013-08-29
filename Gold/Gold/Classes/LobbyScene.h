//
//  LobbyScene.h
//  Gold
//
//  Created by user on 13-8-27.
//
//

#ifndef __Gold__LobbyScene__
#define __Gold__LobbyScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "NetWorkManager.h"
#include "EventHandleQueue.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CLobbyScene
:public CCLayer
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CNetMessageDelegate
{
public:
    static CCScene* CreateScene();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL ;}
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void onExit();
    void onEnter();
    
    bool OnMessage( RakNet::Packet* pMsg );
    void OnEvent(stEventArg* arg );
    
    void OnButtonDown(CCObject*, CCControlEvent);
    void OnLogout(CCObject*, CCControlEvent);
    
};

#endif /* defined(__Gold__LobbyScene__) */
