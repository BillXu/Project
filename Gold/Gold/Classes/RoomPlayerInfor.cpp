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
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitle", CCLabelTTF*, m_pTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoin", CCLabelTTF*, m_pCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBetCoin", CCLabelTTF*, m_pBetCoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPhoto", CCSprite*, m_pPhoto);
    return false ;
}

void CRoomPlayerInfor::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    CCSprite* pS = CCSprite::create("ccbResources/gold_roomTime.png") ;
    m_pRadial = CCProgressTimer::create(pS);
    m_pRadial->setType(kCCProgressTimerTypeRadial);
    addChild(m_pRadial) ;
    m_pRadial->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5)) ;
    m_pRadial->setReverseDirection(true) ;
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
