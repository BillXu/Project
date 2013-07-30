//
//  RoomPlayerInfor.cpp
//  Gold
//
//  Created by Xu BILL on 13-7-27.
//
//

#include "RoomPlayerInfor.h"

CRoomPlayerInfor* CRoomPlayerInfor::create()
{
    CRoomPlayerInfor* pRoom = new  CRoomPlayerInfor ;
    pRoom->init() ;
    pRoom->autorelease() ;
    return pRoom ;
}

// variable
bool CRoomPlayerInfor::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pTitle", CCLabelTTF*, m_pTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pName", CCLabelTTF*, m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pCoin", CCLabelTTF*, m_pCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pBetCoin", CCLabelTTF*, m_pBetCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pbtnPhoto", CCControlButton*, m_pbtnPhoto);
    return false ;
}

SEL_CCControlHandler CRoomPlayerInfor::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"OnClickPhoto",CRoomPlayerInfor::OnClickPhoto);
    return NULL ;
}

void CRoomPlayerInfor::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    CCSprite* pS = CCSprite::create("ccbResources/gold_roomTime.png") ;
    m_pRadial = CCProgressTimer::create(pS);
    m_pRadial->setType(kCCProgressTimerTypeRadial);
    addChild(m_pRadial) ;
    m_pRadial->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5)) ;
    m_pRadial->setReverseDirection(true) ;
    setDelegate(NULL) ;
    StopTiming();
    scheduleUpdate();
}

void CRoomPlayerInfor::update(float fTime )
{
    if ( m_bTiming == false )
        return ;
    m_fRunedTime += fTime ;
    if ( m_fRunedTime <= 30 )
    {
        float fpercent = 1 - m_fRunedTime / 30.0 ;
        m_pRadial->setPercentage(100* fpercent ) ;
    }
    else
    {
        StopTiming() ;
    }
}

void CRoomPlayerInfor::OnClickPhoto(CCObject*, CCControlEvent)
{
    if ( getDelegate() )
    {
        getDelegate()->OnClickRoomPlayerInfo(this) ;
    }
}

void CRoomPlayerInfor::SetPlayerInfo( unsigned int nSessionID ,const char* pname , const char* ptitle, int nCoin )
{
    setSessionID(nSessionID) ;
    m_pName->setString(pname) ;
    m_pTitle->setString(ptitle) ;
    UpdateCoinInfo(0, nCoin);
}

void CRoomPlayerInfor::UpdateCoinInfo(int nBetedCoin , int nOwnCoin)
{
    char pBuffer[50] = { 0 };
    sprintf(pBuffer, "%d",nBetedCoin) ;
    m_pBetCoin->setString(pBuffer) ;
    
    memset(pBuffer, 0, sizeof(pBuffer)) ;
    sprintf(pBuffer, "%d",nOwnCoin) ;
    m_pCoin->setString(pBuffer) ;
}

