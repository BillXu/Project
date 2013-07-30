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
    animationManager->setDelegate(this) ;
    addChild(pRoot) ;
    
    for ( int i = 0 ; i < 15 ; ++i )
    {
        CCSprite* pCard = CCSprite::create("ccbResources/gold_poker.png", CCRectMake(201, 646, 100, 131));
        m_pCardSender->addChild(pCard) ;
        m_pDistributeCard[i] = pCard ;
        pCard->setPosition(ccp(m_pCardSender->getContentSize().width * 0.5,m_pCardSender->getContentSize().height * 0.6)) ;
    }
    CCSprite* pSendFront = CCSprite::create("ccbResources/gold_sendcardFront.png") ;
    m_pCardSender->addChild(pSendFront) ;
    pSendFront->setPosition(ccpMult(m_pCardSender->getContentSize(), 0.5)) ;
    m_pCardSender->setVisible(false) ;
    memset(m_ptDistributePoint, 0, sizeof(m_ptDistributePoint)) ;
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
    
    char pBuffer[50] = { 0 } ;
    for ( int i = 0 ; i < 4 ; ++i )
    {
        // player info 
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pPlayer[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CRoomPlayerInfor*,m_pPlayer[i]);
    
        // default card
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pDefault[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CCSprite*,m_pDefault[i]);
        
        //look card
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pLook[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CCSprite*,m_pLook[i]);
        
        // giveup card
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pGive[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CCSprite*,m_pGive[i]);
        
        // failed card
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pFail[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CCSprite*,m_pFail[i]);
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pDefault[4]",CCSprite*,m_pDefault[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pCardSender",CCSprite*,m_pCardSender);
    
    return NULL ;
}

void CRoomLayer::completedAnimationSequenceNamed(const char *name)
{
     if ( strcmp(name, "ComeIn") == 0 )
     {
         StartDistributeCard();
     }
     else if ( strcmp(name, "GoBack") == 0 )
     {
        for ( int i = 0 ; i < 4 ; ++i )
        {
            m_pDefault[i]->setVisible(true) ;
            m_pLook[i]->setVisible(false) ;
            m_pGive[i]->setVisible(false) ;
            m_pFail[i]->setVisible(false) ;
        }
        m_pDefault[4]->setVisible(true) ;
         
        for ( int i = 0 ; i < 15 ; ++i )
        {
            m_pDistributeCard[i]->setPosition(ccp(m_pCardSender->getContentSize().width * 0.5,m_pCardSender->getContentSize().height * 0.6)) ;
        }
     }
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
    for ( int i = 0 ; i < 4 ; ++i )
    {
        m_pFail[i]->setVisible(!m_pFail[i]->isVisible()) ;
    }
}

void CRoomLayer::OnBuy(CCObject*, CCControlEvent)
{
    m_pPlayer[3]->StartTiming();
    for ( int i = 0 ; i < 4 ; ++i )
    {
        m_pGive[i]->setVisible(!m_pGive[i]->isVisible()) ;
    }
}

void CRoomLayer::OnBack(CCObject*, CCControlEvent)
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        m_pLook[i]->setVisible(!m_pLook[i]->isVisible()) ;
    }
}

void CRoomLayer::OnSay(CCObject*, CCControlEvent)
{
    for ( int i = 0 ; i < 5 ; ++i )
    {
        m_pDefault[i]->setVisible(!m_pDefault[i]->isVisible()) ;
    }
}

void CRoomLayer::StartDistributeCard()
{
    if ( m_ptDistributePoint[0].x == 0 ) // prepare dist pos
    {
        for ( int i = 0 ; i < 5 ; ++i )
        {
            m_ptDistributePoint[i] = m_pTable->convertToWorldSpace(m_pDefault[i]->getPosition()) ;
            m_ptDistributePoint[i] = m_pCardSender->convertToNodeSpace(m_ptDistributePoint[i]) ;
        }
    }
    
    // propare destPoint ; ;
    std::vector<CCPoint> vDistPoint ;
    int iFistIdex  = 0 ;
    int idx = 0 ;
    for ( int i = iFistIdex ; i < iFistIdex + 5 ; ++i )
    {
        idx = i ;
        if ( idx >= 5 )
        {
            idx -= 5 ;
        }
        
        if ( m_pPlayer[idx]->IsReady() == false )
        {
            continue ;
        }
        vDistPoint.push_back(m_ptDistributePoint[idx]) ;
    }
    
    // set up card animation ;
    float fDelay = 0 ;
    float fMoveTime = 0.3 ;
    int iCount = 3 ;
    int nCardIdx = 0 ;
    CCPoint ptOffset = CCPointZero ;
    while (iCount--)
    {
        // set offset ;
        ptOffset.x = m_pDistributeCard[0]->getContentSize().width * 0.2 * iCount ;
        for ( int i = 0 ; i < vDistPoint.size(); ++i )
        {
            CCDelayTime* pDlay = CCDelayTime::create(fDelay) ;
            CCMoveTo* pMoveTo = CCMoveTo::create(fMoveTime, ccpAdd(vDistPoint[i],ptOffset)) ;
            CCSequence* pSeq = CCSequence::createWithTwoActions(pDlay, pMoveTo) ;
            m_pDistributeCard[nCardIdx++]->runAction(pSeq);
            fDelay += fMoveTime ;
        }
    }
}
