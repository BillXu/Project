//
//  PKDlg.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-6.
//
//

#include "PKDlg.h"
bool CPKDlg::init()
{
    CCLayer::init();
    m_pLeftPhoto = m_pRightPhoto = NULL ;
    m_pDelegate = NULL ;
    
    CCNodeLoaderLibrary* plib = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader* pReader = new CCBReader(plib);
    CCNode* pnode =  pReader->readNodeGraphFromFile("PkDlg.ccbi",this);
    pnode->setPosition(ccpMult(ccpFromSize(getContentSize()), 0.5)) ;
    addChild(pnode) ;
    m_pAnimate = pReader->getAnimationManager() ;
    pReader->autorelease() ;
    return true ;
}

bool CPKDlg::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLeft", CCSprite*, m_pLeft) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pRight", CCSprite*, m_pRight) ;
    return false ;
}

void CPKDlg::ShowDlg(const char* pLeft, const char* pRight, bool bLeftWin )
{
    if ( m_pRightPhoto )
    {
        m_pRightPhoto->removeFromParent();
        m_pRightPhoto = NULL ;
    }
    
    if ( m_pLeftPhoto )
    {
        m_pLeftPhoto->removeFromParent() ;
        m_pLeftPhoto = NULL ;
    }
    
    m_bLeftWin = bLeftWin ;
    m_pLeftPhoto = CCSprite::create(pLeft) ;
    m_pLeft->addChild(m_pLeftPhoto) ;
    m_pLeftPhoto->setPosition(ccpMult(ccpFromSize(m_pLeft->getContentSize()), 0.5)) ;
    
    m_pRightPhoto = CCSprite::create(pLeft) ;
    m_pRight->addChild(m_pRightPhoto) ;
    m_pRight->setPosition(ccpMult(ccpFromSize(m_pRight->getContentSize()), 0.5)) ;
    
    m_pAnimate->runAnimationsForSequenceNamed("Default") ;
}

void CPKDlg::completedAnimationSequenceNamed(const char *name)
{
    if ( strcmp(name, "Default") == 0 )
    {
        if ( m_bLeftWin )
        {
            m_pAnimate->runAnimationsForSequenceNamed("LeftWin") ;
        }
        else
        {
            m_pAnimate->runAnimationsForSequenceNamed("RightWin") ;
        }
    }
    else
    {
        if ( m_pDelegate )
        {
            m_pDelegate->OnDlgEnd(this) ;
        }
    }
}