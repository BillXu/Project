//
//  SelectGameScene.cpp
//  Gold
//
//  Created by user on 13-8-27.
//
//

#include "SelectGameScene.h"
#include "MessageDefine.h"
#include "ClientApp.h"
#include "CommonDefine.h"
#include "RoomLayer.h"
#include "SelectRoomLayer.h"
#include "LobbyScene.h"

CCScene* SelectGameScene::CreateScene()
{
    CCScene* pscene = CCScene::create() ;
    SelectGameScene* player = new SelectGameScene ;
    player->init() ;
    pscene->addChild(player ) ;
    player->release() ;
    return pscene ;
}

bool SelectGameScene::init()
{
    CCLayer::init();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/SelectGameScene.ccbi", this) ;
    addChild(pRoot) ;
    
    return true ;
}
void SelectGameScene::onExit()
{
    CCLayer::onExit();
    CClientApp::SharedClientApp()->AddNetMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RegisterEventHandle(eEvent_RecieveBaseData, this, ccevent_selector(SelectGameScene::OnEvent)) ;
}

void SelectGameScene::onEnter()
{
    CCLayer::onEnter();
    CClientApp::SharedClientApp()->RemoveMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RemoveEventHandle(eEvent_RecieveBaseData, this) ;
}

bool SelectGameScene::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    return false ;
}

SEL_CCControlHandler SelectGameScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnPaijiuDown", SelectGameScene::OnPaijiuDown) ;
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnBack", SelectGameScene::OnBack) ;
    return NULL ;
}

bool SelectGameScene::OnMessage( RakNet::Packet* pMsg )
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

void SelectGameScene::OnEvent(stEventArg* arg )
{
    
}


void SelectGameScene::OnPaijiuDown(CCObject* pobj , CCControlEvent event )
{
    CCLayer* pLayer = SelectRoomLayer::CreateLayer(); //HelloWorld::scene() ;
    addChild(pLayer);
}
void SelectGameScene::OnBack(CCObject* pobj , CCControlEvent event )
{
    CCScene* psecen = CLobbyScene::CreateScene(); //HelloWorld::scene() ;
    CCDirector::sharedDirector()->replaceScene(psecen) ;
}

