//
//  LobbyScene.cpp
//  Gold
//
//  Created by user on 13-8-27.
//
//

#include "LobbyScene.h"
#include "MessageDefine.h"
#include "ClientApp.h"
#include "CommonDefine.h"
#include "RoomLayer.h"
#include "SelectGameScene.h"
#include "LoginScene.h"

CCScene* CLobbyScene::CreateScene()
{
    CCScene* pscene = CCScene::create() ;
    CLobbyScene* player = new CLobbyScene ;
    player->init() ;
    pscene->addChild(player ) ;
    player->release() ;
    return pscene ;
}

bool CLobbyScene::init()
{
    CCLayer::init();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/LobbyScene.ccbi", this) ;
    addChild(pRoot) ;
    return true ;
}
void CLobbyScene::onExit()
{
    CCLayer::onExit();
    CClientApp::SharedClientApp()->AddNetMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RegisterEventHandle(eEvent_RecieveBaseData, this, ccevent_selector(CLobbyScene::OnEvent)) ;
}

void CLobbyScene::onEnter()
{
    CCLayer::onEnter();
    CClientApp::SharedClientApp()->RemoveMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RemoveEventHandle(eEvent_RecieveBaseData, this) ;
}

bool CLobbyScene::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    return false ;
}

SEL_CCControlHandler CLobbyScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnButtonDown", CLobbyScene::OnButtonDown) ;
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnLogout", CLobbyScene::OnLogout) ;
    
    return NULL ;
}

bool CLobbyScene::OnMessage( RakNet::Packet* pMsg )
{
    stMsg* pmsg = (stMsg*)pMsg->data ;
    switch (pmsg->usMsgType) {
        case MSG_PLAYER_REGISTER:
        {
        }
            break;
        case MSG_PLAYER_CHECK_ACCOUNT:
        {
        }
            break ;
        case MSG_ROOM_CURRENT_INFO:
        {
        }
            break;
        default:
            break;
    }
    return false ;
}

void CLobbyScene::OnEvent(stEventArg* arg )
{
    
}

void CLobbyScene::OnButtonDown(CCObject* pobj , CCControlEvent event )
{
    CCScene* psecen = SelectGameScene::CreateScene(); //HelloWorld::scene() ;
    CCDirector::sharedDirector()->replaceScene(psecen) ;
    CCLOG("OK");
}

void CLobbyScene::OnLogout(CCObject* pobj , CCControlEvent event )
{
    CCScene* psecen = CLoginLayer::CreateScene(); //HelloWorld::scene() ;
    CCDirector::sharedDirector()->replaceScene(psecen) ;
}
