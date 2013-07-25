//
//  LoginScene.h
//  GoldenPoker
//
//  Created by Xu BILL on 13-5-20.
//
//

#ifndef __GoldenPoker__LoginScene__
#define __GoldenPoker__LoginScene__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT ;
USING_NS_CC ;
class CLoginLayer
:public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static CCScene* scene();
    bool init();
    void OnRegisterClicked(CCObject* pObj ,CCControlEvent event  );
    void OnQuickLoginCliked( CCObject* pObj , CCControlEvent event );
    void OnAccountLoginCliked( CCObject* pObj , CCControlEvent event );
    
    // resolve selector 
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; }
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    // member
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
protected:
    CCNode* m_pPassWord ;
    CCNode* m_pAccount ;
};

#endif /* defined(__GoldenPoker__LoginScene__) */
