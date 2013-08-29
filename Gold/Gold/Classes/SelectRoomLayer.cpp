//
//  SelectRoomLayer.cpp
//  Gold
//
//  Created by user on 13-8-28.
//
//

#include "SelectRoomLayer.h"
#include "MessageDefine.h"
#include "ClientApp.h"
#include "CommonDefine.h"
#include "RoomLayer.h"

CCLayer* SelectRoomLayer::CreateLayer()
{
    CCLayer* pLayer = CCLayer::create() ;
    SelectRoomLayer* player = new SelectRoomLayer ;
    player->init() ;
    pLayer->addChild(player ) ;
    player->release() ;
    return pLayer ;
}

bool SelectRoomLayer::init()
{
    CCLayer::init();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/SelectRoomLayer.ccbi", this) ;
    addChild(pRoot) ;
    
    return true ;
}
void SelectRoomLayer::onExit()
{
    CCLayer::onExit();
    CClientApp::SharedClientApp()->AddNetMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RegisterEventHandle(eEvent_RecieveBaseData, this, ccevent_selector(SelectRoomLayer::OnEvent)) ;
}

void SelectRoomLayer::onEnter()
{
    CCLayer::onEnter();
    CClientApp::SharedClientApp()->RemoveMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RemoveEventHandle(eEvent_RecieveBaseData, this) ;
}

bool SelectRoomLayer::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    return false ;
}

SEL_CCControlHandler SelectRoomLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnClose", SelectRoomLayer::OnClose) ;
    return NULL ;
}

bool SelectRoomLayer::OnMessage( RakNet::Packet* pMsg )
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

void SelectRoomLayer::OnEvent(stEventArg* arg )
{
    
}

void SelectRoomLayer::OnClose(CCObject* p , CCControlEvent )
{
    setVisible(false) ;
    removeFromParent() ;
}

