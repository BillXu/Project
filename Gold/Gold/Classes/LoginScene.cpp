//
//  LoginScene.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-4.
//
//

#include "LoginScene.h"
#include "MessageDefine.h"
#include "ClientApp.h"
#include "CommonDefine.h"
#include "RoomLayer.h"
#include "LobbyScene.h"

CCScene* CLoginLayer::CreateScene()
{
    CCScene* pscene = CCScene::create() ;
    CLoginLayer* player = new CLoginLayer ;
    player->init() ;
    pscene->addChild(player ) ;
    player->release() ;
    return pscene ;
}

bool CLoginLayer::init()
{
    CCLayer::init();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/Login.ccbi", this) ;
    addChild(pRoot) ;
    AffterLoaderCCB();
    return true ;
}
void CLoginLayer::onExit()
{
    CCLayer::onExit();
    CClientApp::SharedClientApp()->RemoveMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RemoveEventHandle(eEvent_RecieveBaseData, this) ;
}

void CLoginLayer::onEnter()
{
    CCLayer::onEnter();
    CClientApp::SharedClientApp()->AddNetMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RegisterEventHandle(eEvent_RecieveBaseData, this, ccevent_selector(CLoginLayer::OnEvent)) ;
}

bool CLoginLayer::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pAccount", CCScale9Sprite*, m_pAccount) ;
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pPassword", CCScale9Sprite*, m_pPassword) ;
    return false ;
}

SEL_CCControlHandler CLoginLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnLogin", CLoginLayer::OnLogin) ;
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnRegister", CLoginLayer::OnRegister) ;
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnQuickEnter", CLoginLayer::OnQuickEnter) ;
    return NULL ;
}

void CLoginLayer::OnLogin(CCObject* pobj , CCControlEvent event )
{
    const char* pAccount = m_pEAccount->getText() ;
    const char* pPassword = m_pEPassword->getText() ;
    stMsgCheckAccount msg ;
    msg.nAccountLen = strlen(pAccount) ;
    msg.nPasswordlen = strlen(pPassword) ;
    
    if ( !msg.nAccountLen || !msg.nPasswordlen )
    {
        CCMessageBox("Can not be null", "tip") ;
        return ;
    }
    
    char* pBuffer = new char [sizeof(msg) + msg.nPasswordlen + msg.nAccountLen ] ;
    int nOffset = 0 ;
    
    memcpy(pBuffer, &msg,sizeof(msg)) ;
    nOffset += sizeof(msg) ;
    
    memcpy(pBuffer + nOffset,pAccount, msg.nAccountLen) ;
    nOffset += msg.nAccountLen ;
    
    memcpy(pBuffer + nOffset,pPassword, msg.nPasswordlen) ;
    nOffset += msg.nPasswordlen ;
    
    CClientApp::SharedClientApp()->SendMsg((stMsg*)pBuffer, nOffset) ;
    delete [] pBuffer;
}

void CLoginLayer::OnRegister(CCObject* pobj , CCControlEvent event )
{
    const char* pAccount = m_pEAccount->getText() ;
    const char* pPassword = m_pEPassword->getText() ;
    stMsgRegister msg ;
    msg.nAccLen = strlen(pAccount) ;
    msg.nPasswordLen = strlen(pPassword) ;
    if ( !msg.nAccLen || !msg.nPasswordLen )
    {
        CCMessageBox("Can not be null", "tip") ;
        return ;
    }
    const char*pdefaultName = "default";
    msg.bSex = eSex_Female ;
    msg.bAutoQuickEnter = false ;
    msg.ncharNameLen = strlen(pdefaultName);
    
    
    char* pBuffer = new char[sizeof(msg)+msg.nPasswordLen + msg.nAccLen + msg.ncharNameLen];
    
    int nOffset = 0 ;
    memcpy(pBuffer, &msg, sizeof(msg)) ;
    nOffset += sizeof(msg) ;
    
    memcpy(pBuffer + nOffset, pAccount, msg.nAccLen) ;
    nOffset += msg.nAccLen ;
    
    memcpy(pBuffer + nOffset, pPassword, msg.nPasswordLen) ;
    nOffset += msg.nPasswordLen ;
    
    memcpy(pBuffer + nOffset , pdefaultName,msg.ncharNameLen) ;
    nOffset += msg.ncharNameLen ;
    
    CClientApp::SharedClientApp()->SendMsg((stMsg*)pBuffer, nOffset) ;
    delete [] pBuffer ;
}

void CLoginLayer::OnQuickEnter(CCObject* pobj , CCControlEvent event )
{
    CCScene *pScene = CLobbyScene::CreateScene();
    CCDirector::sharedDirector()->replaceScene(pScene);
//    stMsgRegister msg ;
//    msg.nAccLen = 0 ;
//    msg.nPasswordLen =0;
//    msg.ncharNameLen = 0;
//    msg.bSex = eSex_Female ;
//    msg.bAutoQuickEnter = true ;
//    CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
}

void CLoginLayer::AffterLoaderCCB()
{
    CCSize ptSize = m_pAccount->getContentSize() ;
    CCEditBox* paccount = CCEditBox::create(ptSize, CCScale9Sprite::create("ccbResources/gold_login_editbg.png")) ;
    paccount->ignoreAnchorPointForPosition(true) ;
    paccount->setFontColor(ccc3(0, 0, 0));
    m_pAccount->addChild(paccount) ;
    paccount->setPlaceHolder("输入账号");
    m_pEAccount = paccount ;
    
    ptSize = m_pPassword->getContentSize() ;
    CCEditBox* ppassword = CCEditBox::create(ptSize, CCScale9Sprite::create("ccbResources/gold_login_editbg.png")) ;
    ppassword->ignoreAnchorPointForPosition(true) ;
    ppassword->setFontColor(ccc3(0, 0, 0));
    ppassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pPassword->addChild(ppassword) ;
    ppassword->setPlaceHolder("输入密码");
    m_pEPassword = ppassword ;
}

bool CLoginLayer::OnMessage( RakNet::Packet* pMsg )
{
    stMsg* pmsg = (stMsg*)pMsg->data ;
    switch (pmsg->usMsgType) {
        case MSG_PLAYER_REGISTER:
        {
            stMsgRegisterRet* pRet = (stMsgRegisterRet*)pmsg ;
             // 0 success ;  1 . account have exsit ;
            if ( pRet->nRet == 0  )
            {
                stMsgPlayerEnterGame msg ;
                msg.nUserUID = pRet->nUserID ;
                CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
            }
            else if ( 1 == pRet->nRet )
            {
                CCMessageBox("account have exsit", "tip") ;
            }
            return true ;
        }
            break;
        case MSG_PLAYER_CHECK_ACCOUNT:
        {
            stMsgCheckAccountRet* pRet = (stMsgCheckAccountRet*)pmsg ;
            // 0 ; success ; 1 account error , 2 password error ;
            if ( pRet->nRet == 0  )
            {
                stMsgPlayerEnterGame msg ;
                msg.nUserUID = pRet->nUserID ;
                CClientApp::SharedClientApp()->SendMsg(&msg, sizeof(msg)) ;
            }
            else if ( 1 == pRet->nRet )
            {
                CCMessageBox("account error", "tip") ;
            }
            else if ( 2 == pRet->nRet )
            {
                CCMessageBox("password error", "tip") ;
            }
            return true ;
        }
            break ;
        case MSG_ROOM_CURRENT_INFO:
        {
            // enter Room scene ;
            CCScene *pScene = CRoomLayer::RoomScene(10,20,50,200,500);
            // run
            CCDirector::sharedDirector()->replaceScene(pScene);
            CRoomLayer* pRL = (CRoomLayer*)pScene->getChildByTag(0) ;
            CRoomData* pRd = new CRoomData ;
            pRd->Init(pRL,10) ;
            pRL->SetRoomData(pRd) ;
            pRd->OnMessage(pMsg) ;
            CClientApp::SharedClientApp()->AddNetMsgDelegate(pRd) ;
        }
            break;
        default:
            break;
    }
    return false ;
}

void CLoginLayer::OnEvent(stEventArg* arg )
{
    if ( eEvent_RecieveBaseData == arg->eEventInvoked )
    {
        // enter to main scene ;
        // temp direct enter room ;
//        stMsgRoomEnter msgTengr ;
//        msgTengr.nRoomLevel = 0 ;
//        msgTengr.nRoomType = 0 ;
//        CClientApp::SharedClientApp()->SendMsg(&msgTengr, sizeof(msgTengr)) ;
        CCScene *pScene = CLobbyScene::CreateScene();
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
}
