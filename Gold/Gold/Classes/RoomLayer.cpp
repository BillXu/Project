//
//  RoomLayer.cpp
//  Gold
//
//  Created by Xu BILL on 13-7-26.
//
//

#include "RoomLayer.h"
#include "RoomPlayerInforLoader.h"
#include "RoomData.h"
#include "ClientApp.h"
#include "CardPoker.h"
#include "PeerCard.h"
CCScene* CRoomLayer::RoomScene( int a , int b , int c , int d ,int e )
{
    CRoomLayer* pLayer = new CRoomLayer ;
    pLayer->init(a,b,c,d,e) ;
    CCScene* pScene = CCScene::create() ;
    pLayer->setTag(0);
    pScene->addChild(pLayer) ;
    pLayer->release() ;
    return  pScene ;
};

bool CRoomLayer::init(int a , int b , int c , int d ,int e )
{
    CCLayer::init() ;
    m_vCoin[0]=a ;
    m_vCoin[1]=b ;
    m_vCoin[2]=c ;
    m_vCoin[3]=d ;
    m_vCoin[4]=e ;
    memset(m_pLookShowCard, 0, sizeof(m_pLookShowCard)) ;
    m_vShowingChouMa.clear();
    m_pRoomData = NULL ;
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("RoomPlayer", CRoomPlayerInforLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("SelectAddBetCoin", CSelectAddBetCoinLoader::loader());
    
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
        
        m_pGive[i] = CCSprite::create("ccbResources/gold_peerc3.png") ;
        m_pGive[i]->setPosition(pt) ;
        parent->addChild(m_pGive[i]) ;
        m_pGive[i]->setVisible(false) ;
        
        m_pFail[i] = CCSprite::create("ccbResources/gold_peerc4.png") ;
        m_pFail[i]->setPosition(pt) ;
        parent->addChild(m_pFail[i]) ;
        m_pFail[i]->setVisible(false) ;
        
        if ( i >= 4 )
        {
            break ;
        }
        
        m_pLook[i] = CCSprite::create("ccbResources/gold_peerc2.png") ;
        m_pLook[i]->setPosition(pt) ;
        parent->addChild(m_pLook[i]) ;
        m_pLook[i]->setVisible(false) ;
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
    
    // set selecte add bet coin delegate ;
    m_pSelectAddBetCoin->SetDelegate(this) ;
    m_pSelectAddBetCoin->InitSelectedMoney(a, b, c, d, e) ;
    m_pSelectAddBetCoin->SetMinEnable(a, true) ;
    
    ResetRoomState();
    
    // self info
    CPlayerBaseData* pmybasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    m_pMyName->setString(pmybasedata->pName) ;
    m_pMyTitle->setString(pmybasedata->strTitle.c_str()) ;
    UpdateMyCoin();
    
    // icon fail and give up icon ;
    m_pMyGiveupIcon = CCSprite::create("ccbResources/gold_playerc1.png") ;
    m_pTable->addChild(m_pMyGiveupIcon) ;
    m_pMyFailedIcon = CCSprite::create("ccbResources/gold_playerc3.png") ;
    m_pTable->addChild(m_pMyFailedIcon) ;
    m_pMyGiveupIcon->setPosition(m_pDefault[0]->getPosition());
    m_pMyFailedIcon->setPosition(m_pDefault[0]->getPosition());
    m_pMyFailedIcon->setVisible(false) ;
    m_pMyGiveupIcon->setVisible(false) ;
    return true ;
}

void CRoomLayer::OnDistributeCardOver(float fTime )
{
    unschedule(schedule_selector(CRoomLayer::OnDistributeCardOver));
    animationManager->runAnimationsForSequenceNamed("GoBack");
    
    // clearn distribute elements ;
    for ( int i = 0 ; i < 4 ; ++i )
    {
        stRoomPeerData* pData = m_pRoomData->GetRoomPeerDataByClientIdx(i) ;
        if (pData != NULL &&  pData->ePeerState == eRoomPeer_Unlook )
        {
            m_pDefault[i]->setVisible(true) ;
        }
        else
        {
            m_pDefault[i]->setVisible(false) ;
        }
        m_pLook[i]->setVisible(false) ;
        m_pGive[i]->setVisible(false) ;
        m_pFail[i]->setVisible(false) ;
    }
    
    for ( int i = 0 ; i < 15 ; ++i )
    {
        m_pDistributeCard[i]->setPosition(ccp(m_pCardSender->getContentSize().width * 0.5,m_pCardSender->getContentSize().height * 0.6)) ;
        m_pDistributeCard[i]->stopAllActions();
    }
    
    // update my button
    bool bUnlook = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState == eRoomPeer_Unlook ;
    m_pbtnReady->setVisible(false) ;
    m_pbtnReady->setEnabled(false) ;
    if ( bUnlook )
    {
        m_pbtnLook->setEnabled(true) ;
        m_pbtnGiveUp->setEnabled(true) ;
        m_pDefault[4]->setVisible(true) ;
    }
    else
    {
    
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pSelectAddBetCoin",CSelectAddBetCoin*,m_pSelectAddBetCoin);
    
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
    stMsgRoomFollow msg ;
    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
}

void CRoomLayer::OnAdd(CCObject*, CCControlEvent)
{
    m_pSelectAddBetCoin->setVisible(true) ;
    m_pSelectAddBetCoin->SetMinEnable(m_pRoomData->m_nSingleBetCoin, true) ;
}

void CRoomLayer::OnLook(CCObject*, CCControlEvent)
{
    stMsgRoomLook msg ;
    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
}

void CRoomLayer::OnPK(CCObject*, CCControlEvent)
{
    bool bDirectPK = m_pRoomData->GetActiveCount() == 1 ;
    
    stRoomPeerData* pData = NULL ;
    for ( int i = 0 ; i < MAX_ROOM_PEER -1 ; ++i )
    {
        pData = m_pRoomData->GetRoomPeerDataByClientIdx(i) ;
        if ( !pData || pData->nSessionID == 0  )
        {
            continue ;
        }
        
        if ( pData->IsActive() == false )
        {
            continue ;
        }
        
        if ( bDirectPK )
        {
            // send pk message ;
            stMsgRoomPK pkMsg ;
            pkMsg.nPKWithSessionID = pData->nSessionID ;
            CClientApp::SharedClientApp()->SendMsg(&pkMsg, sizeof(pkMsg)) ;
            return ;
        }
        else
        {
            RunPKIconAnimationByPlayerIdx(i) ;
            m_bSelectingPKTarget = true ;
        }
    }
}

void CRoomLayer::OnGiveUp(CCObject*, CCControlEvent)
{
    stMsgRoomReady msg ;
    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
}

void CRoomLayer::OnBuy(CCObject*, CCControlEvent)
{
    
}

void CRoomLayer::OnBack(CCObject*, CCControlEvent)
{

}

void CRoomLayer::OnSay(CCObject*, CCControlEvent)
{

}

void CRoomLayer::OnReady(CCObject*, CCControlEvent)
{
    m_pbtnReady->setEnabled(false) ;
    CCFadeOut* pfade = CCFadeOut::create(0.26);
    CCEaseOut* ccer = CCEaseOut::create(pfade, 0.7);
    m_pbtnReady->runAction(ccer) ;
    
    stMsgRoomReady msg ;
    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
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
        stRoomPeerData* pData = m_pRoomData->GetRoomPeerDataByClientIdx(i) ;
        if ( (pData != NULL && pData->ePeerState != eRoomPeer_Unlook) || ( pData == NULL && i != 4 ) )
        {
            continue ;
        }
        
        if ( i == 4 && CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->ePeerState != eRoomPeer_Unlook )
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
    m_bSelectingPKTarget = false ;
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
        m_pPKIcon[i]->setVisible(false) ;
    }
    // disable buttons ;
    m_pbtnAdd->setEnabled(false) ;
    m_pbtnFollow->setEnabled(false) ;
    m_pbtnLook->setEnabled(false) ;
    m_pbtnPK->setEnabled(false) ;
    m_pbtnGiveUp->setEnabled(false) ;
    m_pbtnReady->setVisible(false) ;
    m_pbtnReady->setEnabled(false) ;
    m_pbtnReady->setOpacity(255) ;
    
    m_pSelectAddBetCoin->setVisible(false) ;
    
    m_pMyFailedIcon->setVisible(false) ;
    m_pMyGiveupIcon->setVisible(false) ;
    
    ClearShowingChouMa();
    
    // show card clearn
    for ( int i = 0 ; i < 3 ; ++i )
    {
        if ( m_pLookShowCard[i] )
        {
            m_pLookShowCard[i]->removeFromParent() ;
            m_pLookShowCard[i] = NULL ;
        }
    }
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
    m_pPKIcon[nIdx]->setPosition(ptStand) ;
    m_pPKIcon[nIdx]->runAction(rept) ;
}

void CRoomLayer::StartPushCoinAnimation( char nIdx , unsigned int nCoin )
{
    char pBuffer[120] = { 0 } ;
    while ( nCoin > 0 )
    {
        for ( int i = 4 ; i > 0 ; --i )
        {
            if ( m_vCoin[i] <= nCoin )
            {
                // use this type
                nCoin -= m_vCoin[i];
                memset(pBuffer, 0, sizeof(pBuffer)) ;
                sprintf(pBuffer, "ccbResources/gold_%d.png",m_vCoin[i]) ;
                CCSprite* vfr = CCSprite::create(pBuffer) ;
                m_pTable->addChild(vfr) ;
                m_vShowingChouMa.push_back(vfr) ;
                vfr->setPosition(m_pDefault[nIdx]->getPosition()) ;
                float fRate = CCRANDOM_MINUS1_1();
                CCPoint ptMoveTarget = CCPoint(m_pClock->getPosition()) ;
                ptMoveTarget.x += (fRate * 0.25 * m_pTable->getContentSize().width );
                fRate = CCRANDOM_MINUS1_1();
                ptMoveTarget.y += (fRate * 0.25 * m_pTable->getContentSize().height );
                CCMoveTo* pMoveto = CCMoveTo::create(0.3, ptMoveTarget) ;
                vfr->runAction(pMoveto) ;
                break ;
            }
        }
    }
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
    if (pPlayerInfo->isVisible() == false )
    {
        return  ;
    }
    m_bSelectingPKTarget = false ;
    if ( m_bSelectingPKTarget )
    {
        stMsgRoomPK pkMsg ;
        pkMsg.nPKWithSessionID = pPlayerInfo->getSessionID() ;
        CClientApp::SharedClientApp()->SendMsg(&pkMsg, sizeof(pkMsg)) ;
        
        // stop pkicon animation
        for ( int i = 0 ; i < MAX_ROOM_PEER -1 ; ++i )
        {
            m_pPKIcon[i]->stopAllActions();
            m_pPKIcon[i]->setVisible(false) ;
        }
    }
    else
    {
        // request player detail infor ;
    }
}

void CRoomLayer::OnSelectedAddBetCoin(CSelectAddBetCoin* pBtn , int nCoin )
{
    stMsgRoomAdd msg ;
    msg.nAddMoney = nCoin ;
    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
    m_pSelectAddBetCoin->setVisible(false) ;
}

// logic invoke
void CRoomLayer::OnPlayerLeave( char nIdx )
{
    if ( nIdx < 0 || nIdx >= 4 )
    {
        CCAssert(0, "invalid idx") ;
        return ;
    }
    m_pPlayer[nIdx]->setVisible(false) ;
    m_pDefault[nIdx]->setVisible(false) ;
    m_pLook[nIdx]->setVisible(false) ;
    m_pPKIcon[nIdx]->setVisible(false) ;
    m_pGive[nIdx]->setVisible(false) ;
    m_pFail[nIdx]->setVisible(false) ;
}

void CRoomLayer::OnPlayerEnter( char nIdx , stRoomPeerData* pPlayerData )
{
    m_pPlayer[nIdx]->setVisible(true) ;
    m_pPlayer[nIdx]->setSessionID(pPlayerData->nSessionID) ;
    m_pPlayer[nIdx]->SetPlayerInfo(pPlayerData->nSessionID, pPlayerData->pName, "Master", pPlayerData->nCoin,pPlayerData->nBetCoin ) ;
}

void CRoomLayer::OnRefreshRoomInfo(CRoomData*proomdata)
{
    ResetRoomState();
    char pBuffer[100] = {0} ;
    sprintf(pBuffer, "%d",proomdata->m_nSingleBetCoin ) ;
    m_pSingleBet->setString(pBuffer) ;
    
    // total ;
    memset(pBuffer, 0, sizeof(pBuffer) ) ;
    sprintf(pBuffer, "%d",proomdata->m_nTotalBetCoin ) ;
    m_pTotalBet->setString(pBuffer) ;
    
    memset(pBuffer, 0, sizeof(pBuffer) ) ;
    sprintf(pBuffer, "%d",proomdata->m_nRound ) ;
    m_pRound->setString(pBuffer) ;
    
    for ( int i = 0 ; i < MAX_ROOM_PEER - 1 ; ++i )
    {
        stRoomPeerData* pRoomPeerdata = proomdata->GetRoomPeerDataByClientIdx(i) ;
        if ( pRoomPeerdata)
        {
            OnPlayerEnter(i, pRoomPeerdata ) ;
            OnUpdatePlayerState(i, (eRoomPeerState)pRoomPeerdata->ePeerState ) ;
        }
        else
        {
            OnPlayerLeave(i) ;
        }
    }
    
    // update btn
    if ( m_pRoomData->m_eRoomSate == eRoomState_WaitPeerToGetReady || eRoomState_WaitPeerToJoin == m_pRoomData->m_eRoomSate )
    {
        m_pbtnReady->setVisible(true) ;
        m_pbtnReady->setEnabled(true) ;
        m_pbtnReady->setOpacity(255) ;
    }
    else
    {
    
    }
    UpdateButton();
}

void CRoomLayer::OnUpdatePlayerState(char nIdx , eRoomPeerState ePeerState )
{
    if ( nIdx == 4 )
    {
        switch (ePeerState )
        {
            case eRoomPeer_Look:
            {
                m_pDefault[4]->setVisible(false) ;
                char* vCard = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData()->vCard ;
                CPeerCard peerCard ;
                peerCard.SetPeerCardByNumber(vCard[0], vCard[1], vCard[2]);
                CCPoint ptDefault = m_pDefault[4]->getPosition();
                for ( int i = 0 ; i < 3 ; ++i )
                {
                    m_pLookShowCard[i] = GetAutoSpriteByCard(peerCard.GetCardByIdx(i));
                    m_pTable->addChild(m_pLookShowCard[i]) ;
                    m_pLookShowCard[i]->setPosition(ccp(ptDefault.x + ( m_pLookShowCard[i]->getContentSize().width * 1.2 ) * ( i - 1 ),ptDefault.y));
                }
            }
                break;
            case eRoomPeer_Unlook:
            {
                m_pDefault[nIdx]->setVisible(true) ;
            }
                break;
            case eRoomPeer_Failed:
            {
                if (m_pLookShowCard[0] == NULL )  // unlook failed
                {
                    m_pDefault[4]->setVisible(false) ;
                    m_pFail[4]->setVisible(true) ;
                }
                else   // look failed ;
                {
                    m_pMyFailedIcon->setVisible(true) ;
                }
            }
                break;
            case eRoomPeer_GiveUp:
            {
                if (m_pLookShowCard[0] == NULL )  // unlook gvie up 
                {
                    m_pDefault[4]->setVisible(false) ;
                    m_pGive[4]->setVisible(true) ;
                }
                else   // look give up ;
                {
                    m_pMyGiveupIcon->setVisible(true) ;
                }
            }
                break;
            default:
                break;
        }        
    }
    else
    {
        m_pDefault[nIdx]->setVisible(ePeerState == eRoomPeer_Unlook) ;
        m_pLook[nIdx]->setVisible(ePeerState == eRoomPeer_Look) ;
        m_pGive[nIdx]->setVisible(ePeerState == eRoomPeer_GiveUp) ;
        m_pFail[nIdx]->setVisible(ePeerState == eRoomPeer_Failed) ;
    }
    m_pReadyIcon[nIdx]->setVisible(ePeerState == eRoomPeer_Ready) ;
    UpdateButton();
}

void CRoomLayer::OnDistributeCard()
{
    // hide ready icon ;
    for ( int i = 0 ; i < MAX_ROOM_PEER ; ++i )
    {
        m_pReadyIcon[i]->setVisible(false) ;
    }
    animationManager->runAnimationsForSequenceNamed("ComeIn") ;
    UpdateButton();
}

void CRoomLayer::OnWaitPlayerAction(char nIdx )
{
    char pBuffer[20] = { 0 } ;
    sprintf(pBuffer, "%d",m_pRoomData->m_nRound ) ;
    m_pRound->setString(pBuffer) ;
    if ( nIdx == 4 )
    {
        StartMyClock() ;
    }
    else
    {
        m_pPlayer[nIdx]->StartTiming() ;
    }
    UpdateButton(nIdx == 4 );
}

void CRoomLayer::OnPlayerFollow(char nIdx , int nFollowedCoin )
{
    StartPushCoinAnimation(nIdx, nFollowedCoin) ;
    char nBuffer[20]={0} ;
    sprintf(nBuffer, "%d",m_pRoomData->m_nTotalBetCoin ) ;
    m_pTotalBet->setString(nBuffer) ;
    
    if ( nIdx == 4 )  // self
    {
        UpdateMyCoin();
        return ;
    }
    stRoomPeerData* pData = m_pRoomData->GetRoomPeerDataByClientIdx(nIdx);
    m_pPlayer[nIdx]->UpdateCoinInfo(pData->nBetCoin, pData->nCoin) ;
}

void CRoomLayer::OnPlayerAdd(char nIdx , int nOffsetCoin )
{
    StartPushCoinAnimation(nIdx, nOffsetCoin) ;
    char nBuffer[20]={0} ;
    sprintf(nBuffer, "%d",m_pRoomData->m_nTotalBetCoin ) ;
    m_pTotalBet->setString(nBuffer) ;

    memset(nBuffer, 0, sizeof(nBuffer)) ;
    sprintf(nBuffer, "%d",m_pRoomData->m_nSingleBetCoin ) ;
    m_pSingleBet->setString(nBuffer) ;

    if ( nIdx == 4 )
    {
        UpdateMyCoin();
    }
    else
    {
        stRoomPeerData* pData = m_pRoomData->GetRoomPeerDataByClientIdx(nIdx);
        m_pPlayer[nIdx]->UpdateCoinInfo(pData->nBetCoin, pData->nCoin) ;
    }
}

void CRoomLayer::OnPlayerPK(char nIdxInvoke , char nIdxWith , bool bWin )
{
    
}

void CRoomLayer::ClearShowingChouMa()
{
    LIST_SPRITE::iterator iter = m_vShowingChouMa.begin();
    for ( ; iter != m_vShowingChouMa.end() ; ++iter )
    {
        if ( *iter)
        {
            (*iter)->removeFromParent() ;
        }
    }
    m_vShowingChouMa.clear() ;
}

void CRoomLayer::UpdateMyCoin()
{
    CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    char nBuffer[20]={0} ;
    // bet coin
    memset(nBuffer, 0, sizeof(nBuffer) ) ;
    sprintf(nBuffer, "%d",pbasedata->nBetCoin) ;
    m_pMyBetCoin->setString(nBuffer) ;
    // ncoin
    memset(nBuffer, 0, sizeof(nBuffer) ) ;
    sprintf(nBuffer, "%d",pbasedata->nCoin) ;
    m_pMyCoin->setString(nBuffer) ;
}

CCSprite* CRoomLayer::GetAutoSpriteByCard(CCard* pcard)
{
    float fW = 99.2 ;
    float fH = 129.3 ;
    int nLineIdx = ( pcard->GetCardFaceNum() - 1 ) * 4 + pcard->GetType() ;
    int nRow = nLineIdx / 10 ;
    int nCol = nLineIdx % 10 ;
    CCRect ptRct = CCRectMake(nRow * fH, fW * nCol, fW, fH) ;
    CCSprite* sprite = CCSprite::create("ccbResources/gold_poker.png", ptRct) ;
    return sprite ;
}

void CRoomLayer::UpdateButton(bool bMyTurn )
{
    CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData();
    m_pbtnFollow->setEnabled(bMyTurn && pbasedata->IsActive() ) ;
    m_pbtnAdd->setEnabled(bMyTurn && pbasedata->IsActive() );
    m_pbtnLook->setEnabled(pbasedata->ePeerState == eRoomPeer_Unlook ) ;
    m_pbtnGiveUp->setEnabled(pbasedata->IsActive() );
    m_pbtnPK->setEnabled( bMyTurn && pbasedata->IsActive() && (m_pRoomData->m_nRound > 2 || pbasedata->nCoin < m_pRoomData->m_nSingleBetCoin ));
    bool bShowReady = pbasedata->ePeerState == eRoomPeer_None ;
    m_pbtnReady->setVisible(bShowReady) ;
    m_pbtnReady->setEnabled(bShowReady) ;
    m_pbtnReady->setOpacity(255*bShowReady) ;
}
