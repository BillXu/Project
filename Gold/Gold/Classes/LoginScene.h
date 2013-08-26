//
//  LoginScene.h
//  Gold
//
//  Created by Xu BILL on 13-8-4.
//
//

#ifndef __Gold__LoginScene__
#define __Gold__LoginScene__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "NetWorkManager.h"
#include "EventHandleQueue.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CLoginLayer
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
    
    void AffterLoaderCCB();
    void OnLogin(CCObject*, CCControlEvent);
    void OnRegister(CCObject*, CCControlEvent);
    void OnQuickEnter(CCObject*, CCControlEvent);
    bool OnMessage( RakNet::Packet* pMsg );
    void OnEvent(stEventArg* arg );
protected:
    CCLayer* m_pAccount , * m_pPassword ;
    CCEditBox* m_pEAccount, *m_pEPassword ;
};
#endif /* defined(__Gold__LoginScene__) */
