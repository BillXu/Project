//
//  LoginScene.cpp
//  GoldenPoker
//
//  Created by Xu BILL on 13-5-20.
//
//

#include "LoginScene.h"
CCScene* CLoginLayer::scene()
{
    CCScene* scene = CCScene::create() ;
    CLoginLayer * layer = new CLoginLayer ;
    layer->init() ;
    scene->addChild(layer) ;
    layer->release() ;
    CCSize ptWinSize = CCDirector::sharedDirector()->getWinSize() ;
    scene->setContentSize(ptWinSize) ;
    CCSize ptLogicWinSize = CCDirector::sharedDirector()->getLogicWinSize() ;
    // scale layer ;
    float fScaleToHeight = (float)ptWinSize.height / (float)ptLogicWinSize.height ;
    float fScaledWidth = ptLogicWinSize.width * fScaleToHeight ;
    if ( fScaledWidth > ptWinSize.width )
    {
        float fScaleToWidth = (float)ptWinSize.width / (float)ptLogicWinSize.width ;
        layer->setScale(fScaleToWidth) ;
    }
    else
    {
        layer->setScale(fScaleToHeight) ;
    }
    layer->setPosition(ccp((ptWinSize.width - ptLogicWinSize.width ) * 0.5, (ptWinSize.height - ptLogicWinSize.height) *0.5 ));
    return scene ;
}

bool CLoginLayer::init()
{
    CCLayer::init() ;
    m_pAccount = m_pPassWord = NULL ;
    CCNodeLoaderLibrary* pLiberay =  CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary() ;
    CCBReader* pReader =  new CCBReader(pLiberay);
    CCNode* pnode = pReader->readNodeGraphFromFile("UIScene/Login.ccbi", this) ;
    addChild(pnode) ;
    pReader->release() ;
//    CCSprite* pgirl = CCSprite::create("iphone/linkingscene_logo.png") ;
//    pgirl->setAnchorPoint(ccp(0,0));
//    pgirl->setPosition(ccp(8.0,-7));
//    addChild(pgirl) ;
    return true ;
}

void CLoginLayer::OnRegisterClicked(cocos2d::CCObject *pObj, CCControlEvent event)
{
    CCMessageBox("OnRegisterClicked", "ok");
}

void CLoginLayer::OnAccountLoginCliked(cocos2d::CCObject *pObj, CCControlEvent event)
{
    CCMessageBox("OnAccountLoginCliked", "ok");
}

void CLoginLayer::OnQuickLoginCliked(cocos2d::CCObject *pObj, CCControlEvent event)
{
    CCMessageBox("OnQuickLoginCliked", "ok");
}

SEL_CCControlHandler CLoginLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnRegister", CLoginLayer::OnRegisterClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnQuikLogin", CLoginLayer::OnQuickLoginCliked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnAccountLogin", CLoginLayer::OnAccountLoginCliked);
    return NULL ;
}

bool CLoginLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pAccount", CCNode*, m_pAccount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pPassWord", CCNode*, m_pPassWord);
    return false ;
}
