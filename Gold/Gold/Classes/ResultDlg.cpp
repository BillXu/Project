//
//  ResultDlg.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-8.
//
//

#include "ResultDlg.h"
#include "RoomData.h"
void CResultDlg::stResultInfo::SetInfo(const char* pInfo , bool bWin , unsigned int nCoin )
{
    pName->setString(pInfo) ;
    pName->setVisible(true) ;
    m_pNumber->setVisible(true) ;
    m_pResult->setVisible(true) ;
    if ( bWin )
    {
        m_pResult->setString("胜利");
        m_pResult->setColor(ccc3(0, 128, 128)) ;
    }
    else
    {
        m_pResult->setString("失败");
        m_pResult->setColor(ccWHITE) ;
    }
    
    char pBuffer[30] = {0} ;
    sprintf(pBuffer, "%d",nCoin) ;
    m_pNumber->setString(pBuffer) ;
}

void CResultDlg::stResultInfo::Hide()
{
    pName->setVisible(false) ;
    m_pNumber->setVisible(false) ;
    m_pResult->setVisible(false) ;
}

bool CResultDlg::init()
{
    CCLayer::init() ;
    CCNodeLoaderLibrary* plib = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary() ;
    CCBReader* pread = new CCBReader(plib);
    CCNode* pnode = pread->readNodeGraphFromFile("ResultDlg.ccbi", this) ;
    addChild(pnode) ;
    return true ;
}

bool CResultDlg::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    char pBuffer[100] = { 0 } ;
    for ( int i = 0 ; i < 5 ; ++i )
    {
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        // name
        sprintf(pBuffer, "name%d",i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, pBuffer,CCLabelTTF*, m_vInfos[i].pName) ;
        // result ;
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "result%d", i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, pBuffer, CCLabelTTF*, m_vInfos[i].m_pResult) ;
        // number
        memset(pBuffer, 0, sizeof(pBuffer)) ;
        sprintf(pBuffer, "coin%d", i) ;
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, pBuffer, CCLabelTTF*, m_vInfos[i].m_pNumber) ;
    }
    return false ;
}

SEL_CCControlHandler CResultDlg::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnClose", CResultDlg::OnClose) ;
    return NULL ;
}

void CResultDlg::OnClose(CCObject* p , CCControlEvent )
{
    setVisible(false) ;
    removeFromParent() ;
}

void CResultDlg::ShowDlg(std::list<stResultData*>& pst , CRoomData* pdata)
{
    for ( int i = 0 ; i < 5 ; ++i )
    {
        m_vInfos[i].Hide() ;
    }
    // show 
}