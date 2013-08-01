//
//  SelectAddBetCoin.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#include "SelectAddBetCoin.h"
CSelectAddBetCoin* CSelectAddBetCoin::create()
{
    CSelectAddBetCoin* p = new CSelectAddBetCoin ;
    p->init() ;
    p->autorelease() ;
    return  p ;
}

// variable
bool CSelectAddBetCoin::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    char pBuffer[100] = { 0 } ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pDouble", CCControlButton*, m_pDouble) ;
    for ( int i = 0; i < 5 ; ++i )
    {
        memset(pBuffer, 0 , sizeof(pBuffer)) ;
        sprintf(pBuffer, "m_pCoin[%d]",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, pBuffer, CCControlButton*, m_pCoin[i]) ;
    }
    return false ;
}

SEL_CCControlHandler CSelectAddBetCoin::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnSelected", CSelectAddBetCoin::OnSelected) ;
    return NULL ;
}

void CSelectAddBetCoin::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    SetDelegate(NULL) ;
    memset(m_vCoin, 0, sizeof(m_vCoin)) ;
}

void CSelectAddBetCoin::OnSelected(CCObject*pobj, CCControlEvent)
{
    if ( isVisible() == false )
        return ;
    CCNode* pNode = (CCNode*)pobj ;
    if ( pNode == NULL )
        return ;
    int nTag = pNode->getTag() ;
    if ( nTag < 0 || nTag > 5 )
        return  ;
    int nSelectCoin = 0 ;
    if ( nTag != 5 )
    {
        nSelectCoin = m_vCoin[nTag] ;
    }
    if ( m_pDelegate )
    {
        m_pDelegate->OnSelectedAddBetCoin(this, nSelectCoin) ;
    }
    setVisible(false) ;
}

void CSelectAddBetCoin::InitSelectedMoney(int a , int b , int c , int d , int e )
{
    m_vCoin[0] = a ;
    m_vCoin[1] = b ;
    m_vCoin[2] = c ;
    m_vCoin[3] = d ;
    m_vCoin[4] = e ;
    for ( int i = 0 ; i < 5; ++i  )
    {
        setNumber(m_vCoin[i], i) ;
    }
}

void CSelectAddBetCoin::SetMinEnable( int nCoin , bool bEnableDouble )
{
    bool bEnable = 0 ;
    for ( int i = 0 ; i < 5; ++i  )
    {
        bEnable = m_vCoin[i]>= nCoin ;
        m_pCoin[i]->setEnabled(bEnable) ;
    }
    m_pDouble->setEnabled(bEnableDouble) ; 
}

void CSelectAddBetCoin::setNumber(int nCoin, int nIndex )
{
    if ( nIndex >= 5 || nIndex < 0 )
    {
        CCAssert(false, "index Error") ;
        return ;
    }
    char pBuffer[120] = { 0 } ;
    sprintf(pBuffer, "ccbResources/gold_%d.png",nCoin) ;
    CCScale9Sprite* pNormal = CCScale9Sprite::create(pBuffer) ;
    CCScale9Sprite* pHight = CCScale9Sprite::create(pBuffer) ;
    
    memset(pBuffer, 0, sizeof(pBuffer)) ;
    sprintf(pBuffer, "ccbResources/gold_%d_dis.png",nCoin) ;
    CCScale9Sprite* pDisable = CCScale9Sprite::create(pBuffer) ;
    if ( !pNormal || !pHight || !pDisable )
    {
        CCMessageBox("coin resource don't exist nCoin", "error") ;
        return ;
    }
    m_pCoin[nIndex]->setBackgroundSpriteForState(pNormal, CCControlStateNormal) ;
    m_pCoin[nIndex]->setBackgroundSpriteForState(pHight, CCControlStateHighlighted) ;
    m_pCoin[nIndex]->setBackgroundSpriteForState(pDisable, CCControlStateDisabled) ;
}
