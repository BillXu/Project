//
//  PaijiuTable.cpp
//  Gold
//
//  Created by user on 13-8-30.
//
//

#include "PaijiuTable.h"
#include "MessageDefine.h"
#include "ClientApp.h"
#include "CommonDefine.h"
#include "RoomLayer.h"
#include "SelectRoomLayer.h"
#include "LobbyScene.h"

CCScene* PaijiuTable::CreateScene()
{
    CCScene* pscene = CCScene::create() ;
    PaijiuTable* player = new PaijiuTable ;
    player->init() ;
    pscene->addChild(player ) ;
    player->release() ;
    return pscene ;
}

bool PaijiuTable::init()
{
    CCLayer::init();
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader->readNodeGraphFromFile("ccbi/PaijiuTable.ccbi", this) ;
    addChild(pRoot) ;
    
    return true ;
}
void PaijiuTable::onEnter()
{
    CCLayer::onEnter();
    CClientApp::SharedClientApp()->AddNetMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RegisterEventHandle(eEvent_RecieveBaseData, this, ccevent_selector(PaijiuTable::OnEvent)) ;
}

void PaijiuTable::onExit()
{
    CCLayer::onExit();
    CClientApp::SharedClientApp()->RemoveMsgDelegate(this) ;
    CEventHandleQueue::SharedEventHandleQueue()->RemoveEventHandle(eEvent_RecieveBaseData, this) ;
}

bool PaijiuTable::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    return false ;
}

SEL_CCControlHandler PaijiuTable::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL ;
}

bool PaijiuTable::OnMessage( RakNet::Packet* pMsg )
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

void PaijiuTable::OnEvent(stEventArg* arg )
{
    
}

