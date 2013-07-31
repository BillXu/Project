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
    
    // set up showcards and ready icon 
    for ( int i = 0 ; i < 5 ; ++i )
    {
        
        CCPoint pt = m_pDefault[i]->getPosition() ;
        m_pDefault[i]->setVisible(false) ;
        CCNode* parent = m_pDefault[i]->getParent() ;
        
        m_pReadyIcon[i] = CCSprite::create("ccbResources/gold_readystatus.png") ;
        m_pReadyIcon[i]->setPosition(pt) ;
        parent->addChild(m_pReadyIcon[i]) ;
        m_pReadyIcon[i]->setVisible(false) ;
        if ( i >= 4 )
        {
            break ;
        }
        
        m_pLook[i] = CCSprite::create("ccbResources/gold_peerc2.png") ;
        m_pLook[i]->setPosition(pt) ;
        parent->addChild(m_pLook[i]) ;
        m_pLook[i]->setVisible(false) ;
        
        m_pGive[i] = CCSprite::create("ccbResources/gold_peerc3.png") ;
        m_pGive[i]->setPosition(pt) ;
        parent->addChild(m_pGive[i]) ;
        m_pGive[i]->setVisible(false) ;
        
        m_pFail[i] = CCSprite::create("ccbResources/gold_peerc4.png") ;
        m_pFail[i]->setPosition(pt) ;
        parent->addChild(m_pFail[i]) ;
        m_pFail[i]->setVisible(false) ;
    }
    
    // set up distribute cards ;
    for ( int i = 0 ; i < 15 ; ++i )
    {
        CCSprite* pCard = CCSprite::create("ccbResources/gold_poker.png", CC_RECT_PIXELS_TO_POINTS(CCRectMake(201, 646, 100, 131)));
        m_pCardSender->addChild(pCard) ;
        m_pDistributeCard[i] = pCard ;
        pCard->setScale(0.67) ;
        pCard->setPosition(ccp(m_pCardSender->getContentSize().width * 0.5,m_pCardSender->getContentSize().height * 0.6)) ;
    }
    CCSprite* pSendFront = CCSprite::create("ccbResources/gold_sendcardFront.png") ;
    m_pCardSender->addChild(pSendFront) ;
    pSendFront->setPosition(ccpMult(m_pCardSender->getContentSize(), 0.5)) ;
    m_pCardSender->setVisible(false) ;
    memset(m_ptDistributePoint, 0, sizeof(m_ptDistributePoint)) ;
    // init member var state
    m_pClock->setVisible(false) ;
    // set player info delegete
    for ( int i = 0 ; i < 4 ; ++i )
    {
        m_pPlayer[i]->setDelegate(this) ;
    }
    
    return true ;
}

void CRoomLayer::OnDistributeCardOver(float fTime )
{
    unschedule(schedule_selector(CRoomLayer::OnDistributeCardOver));
    animationManager->runAnimationsForSequenceNamed("GoBack");
    
    // clearn distribute elements ;
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
        m_pDistributeCard[i]->stopAllActions();
    }
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
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnReady",CRoomLayer::OnReady);
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

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnFollow",CCControlButton*,m_pbtnFollow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnAdd",CCControlButton*,m_pbtnAdd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnLook",CCControlButton*,m_pbtnLook);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnPK",CCControlButton*,m_pbtnPK);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnGiveUp",CCControlButton*,m_pbtnGiveUp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pbtnReady",CCControlButton*,m_pbtnReady);
    
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
        
        // pk icon
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pPKIcon[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,pBuffer,CCSprite*,m_pPKIcon[i]);
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

     }
}

void CRoomLayer::OnFollow(CCObject*, CCControlEvent)
{
    animationManager->runAnimationsForSequenceNamed("ComeIn") ;
}

void CRoomLayer::OnAdd(CCObject*, CCControlEvent)
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        RunPKIconAnimationByPlayerIdx(i) ;
    }
}

void CRoomLayer::OnLook(CCObject*, CCControlEvent)
{
    m_pPlayer[0]->StartTiming();
    StartMyClock();
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
    ResetRoomState();
}

void CRoomLayer::OnSay(CCObject*, CCControlEvent)
{
    for ( int i = 0 ; i < 5 ; ++i )
    {
        m_pDefault[i]->setVisible(!m_pDefault[i]->isVisible()) ;
    }
}

void CRoomLayer::OnReady(CCObject*, CCControlEvent)
{
    m_pbtnReady->setEnabled(false) ;
    CCFadeOut* pfade = CCFadeOut::create(0.26);
    CCEaseOut* ccer = CCEaseOut::create(pfade, 0.7);
    m_pbtnReady->runAction(ccer) ;
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
    float fDelay = 0.2 ;
    float fMoveTime = 0.4 ;
    int iCount = 3 ;
    int nCardIdx = 0 ;
    CCPoint ptOffset = CCPointZero ;
    while (iCount--)
    {
        // set offset ;
        ptOffset.x = m_pDistributeCard[0]->getContentSize().width * 0.08 ;
        if ( iCount == 2 )
        {
            ptOffset.x *= -1 ;
        }
        else if ( iCount == 1 )
        {
            ptOffset.x = 0 ;
        }
        
        for ( int i = 0 ; i < vDistPoint.size(); ++i )
        {
            CCDelayTime* pDlay = CCDelayTime::create(fDelay) ;
            CCMoveTo* pMoveTo = CCMoveTo::create(fMoveTime, ccpAdd(vDistPoint[i],ptOffset)) ;
            CCSequence* pSeq = CCSequence::createWithTwoActions(pDlay, pMoveTo) ;
            m_pDistributeCard[nCardIdx++]->runAction(pSeq);
            fDelay += fMoveTime ;
        }
    }
    fDelay += 0.3 ;
    // register over function ;
    schedule(schedule_selector(CRoomLayer::OnDistributeCardOver), fDelay , 1, 0) ;
}

void CRoomLayer::ResetRoomState()
{
    unscheduleAllSelectors() ;
    m_pClock->setVisible(false) ;
    for ( int i = 0 ; i < 5 ; ++i )
    {
        m_pDefault[i]->setVisible(false) ;
        m_pReadyIcon[i]->setVisible(false) ;
        if ( i >= 4 )
        {
            break ;
        }
        m_pLook[i]->setVisible(false) ;
        m_pGive[i]->setVisible(false) ;
        m_pFail[i]->setVisible(false) ;
    }
    // disable buttons ;
    m_pbtnAdd->setEnabled(false) ;
    m_pbtnFollow->setEnabled(false) ;
    m_pbtnLook->setEnabled(false) ;
    m_pbtnPK->setEnabled(false) ;
    m_pbtnGiveUp->setEnabled(false) ;
    m_pbtnReady->setVisible(true) ;
    m_pbtnReady->setEnabled(true) ;
    m_pbtnReady->setOpacity(255) ;
}

void CRoomLayer::RunPKIconAnimationByPlayerIdx(char nIdx )
{
    if ( nIdx < 0 || nIdx > 3 )
    {
        CCAssert(0, "unknown idx ") ;
        return ;
    }
    CCPoint ptStand = m_pDefault[nIdx]->getPosition() ;
    CCSequence* pseq = NULL ;
    CCRepeat* rept = NULL ;
    CCMoveBy* pMove = NULL ;
    CCMoveBy* pMove2 = NULL ;
    CCPoint ptOffset = ccp(12.0,5.0) ;
    ptOffset = ccpMult(ptOffset, CCBReader::getResolutionScale()) ;
    switch (nIdx) {
        case 0:
        {
            pMove = CCMoveBy::create(0.5, ccpMult(ptOffset, 1));
            pMove2 = CCMoveBy::create(0.5, ccpMult(ptOffset, -1));
        }
            break;
        case 1:
        {
            ptOffset.x *= -1 ;
            pMove = CCMoveBy::create(0.5, ccpMult(ptOffset, -1));
            pMove2 = CCMoveBy::create(0.5, ccpMult(ptOffset, 1));
        }
            break ;
        case 2:
        {
            pMove = CCMoveBy::create(0.5, ccpMult(ptOffset, -1));
            pMove2 = CCMoveBy::create(0.5, ccpMult(ptOffset, 1));
        }
            break;
        case 3:
        {
            ptOffset.x *= -1 ;
            pMove = CCMoveBy::create(0.5, ccpMult(ptOffset, 1));
            pMove2 = CCMoveBy::create(0.5, ccpMult(ptOffset, -1));
        }
            break ;
        default:
        {
            CCAssert(0, "unknow idx ") ;
            return ;
        }
            break;
    }
    
    pseq = CCSequence::createWithTwoActions(pMove, pMove2) ;
    rept = CCRepeat::create(pseq, 99999999) ;
    m_pPKIcon[nIdx]->setVisible(true) ;
    m_pPKIcon[nIdx]->runAction(rept) ;
}

void CRoomLayer::StartGame(unsigned int nMainPlayer )
{
    animationManager->runAnimationsForSequenceNamed("ComeIn");
}

void CRoomLayer::StartMyClock()
{
    schedule(schedule_selector(CRoomLayer::OnClocked), 1 , 30, 0) ;
    m_pClock->setVisible(true) ;
    m_fMyTimerCount = 30 ;
}

void CRoomLayer::OnClocked(float fTime )
{
    m_fMyTimerCount -= fTime ;
    int nTimer = m_fMyTimerCount + 0.5;
    if ( nTimer < 0 )
    {
        nTimer = 0 ;
    }
    char pBuffer[10] = { 0 } ;
    sprintf(pBuffer, "%d",nTimer) ;
    m_pTime->setString(pBuffer) ;
    if ( nTimer <= 0 )
    {
        unschedule(schedule_selector(CRoomLayer::OnClocked)) ;
    }
}

void CRoomLayer::StopMyClock()
{
    unschedule(schedule_selector(CRoomLayer::OnClocked)) ;
    m_pClock->setVisible(false) ;
}

void CRoomLayer::OnClickRoomPlayerInfo(CRoomPlayerInfor* pPlayerInfo )
{
    CCMessageBox("clicked player", "Tip") ;
}