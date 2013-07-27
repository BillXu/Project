//
//  RoomLayer.cpp
//  Gold
//
//  Created by Xu BILL on 13-7-26.
//
//

#include "RoomLayer.h"
#include "RoomPlayerInforLoader.h"
CCScene* CRoomLayer::RoomScene()
{
    CRoomLayer* pLayer = new CRoomLayer ;
    pLayer->init() ;
    CCScene* pScene = CCScene::create() ;
    pScene->addChild(pLayer) ;
    pLayer->release() ;
    return  pScene ;
};

bool CRoomLayer::init()
{
    CCLayer::init() ;
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("RoomPlayer", CRoomPlayerInforLoader::loader());
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/Room.ccbi", this) ;

    animationManager = pReader->getAnimationManager();
    addChild(pRoot) ;
    return true ;
}

SEL_CCControlHandler CRoomLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnFollow",CRoomLayer::OnFollow);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnAdd",CRoomLayer::OnAdd);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnLook",CRoomLayer::OnLook);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnPK",CRoomLayer::OnPK);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnGiveUp",CRoomLayer::OnGiveUp);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnBuy",CRoomLayer::OnBuy);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnBack",CRoomLayer::OnBack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnSay",CRoomLayer::OnSay);
    return NULL ;
}

bool CRoomLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pMyCoin",CCLabelTTF*,m_pMyCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pMyDiamen",CCLabelTTF*,m_pMyDiamen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pMyName",CCLabelTTF*,m_pMyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pMyTitle",CCLabelTTF*,m_pMyTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pMyBetCoin",CCLabelTTF*,m_pMyBetCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pRound",CCLabelTTF*,m_pRound);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pTotalBet",CCLabelTTF*,m_pTotalBet);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pSingleBet",CCLabelTTF*,m_pSingleBet);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pTime",CCLabelTTF*,m_pTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pTable",CCSprite*,m_pTable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pClock",CCSprite*,m_pClock);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pPlayer[0]",CRoomPlayerInfor*,m_pPlayer[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pPlayer[1]",CRoomPlayerInfor*,m_pPlayer[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pPlayer[2]",CRoomPlayerInfor*,m_pPlayer[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pPlayer[3]",CRoomPlayerInfor*,m_pPlayer[3]);
    return NULL ;
}

void CRoomLayer::OnFollow(CCObject*, CCControlEvent)
{
    animationManager->runAnimationsForSequenceNamed("ComeIn") ;
}

void CRoomLayer::OnAdd(CCObject*, CCControlEvent)
{
    animationManager->runAnimationsForSequenceNamed("GoBack") ;
}

void CRoomLayer::OnLook(CCObject*, CCControlEvent)
{
    m_pPlayer[0]->StartTiming();
}

void CRoomLayer::OnPK(CCObject*, CCControlEvent)
{
    m_pPlayer[1]->StartTiming();
}

void CRoomLayer::OnGiveUp(CCObject*, CCControlEvent)
{
    m_pPlayer[2]->StartTiming();
}

void CRoomLayer::OnBuy(CCObject*, CCControlEvent)
{
    m_pPlayer[3]->StartTiming();
}

void CRoomLayer::OnBack(CCObject*, CCControlEvent)
{

}

void CRoomLayer::OnSay(CCObject*, CCControlEvent)
{

}
