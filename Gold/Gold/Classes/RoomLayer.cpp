//
//  RoomLayer.cpp
//  Gold
//
//  Created by Xu BILL on 13-7-26.
//
//

#include "RoomLayer.h"
#include "RoomPlayerInforLoader.h"
CCScene* CRoomLayer::RoomScene()
{
    CRoomLayer* pLayer = new CRoomLayer ;
    pLayer->init() ;
    CCScene* pScene = CCScene::create() ;
    pScene->addChild(pLayer) ;
    pLayer->release() ;
    return  pScene ;
};

bool CRoomLayer::init()
{
    CCLayer::init() ;
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("RoomPlayer", CRoomPlayerInforLoader::loader());
    
    CCBReader* pReader = new CCBReader(ccNodeLoaderLibrary,this,this);
    float fHfactor = 320.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 480.0 / CCDirector::sharedDirector()->getWinSize().width ;
    float fCCBResoulution = 1.0 / (fHfactor > fWfactor ? fHfactor :fWfactor) ;
    pReader->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot2 = pReader->readNodeGraphFromFile("ccbi/RoomPlayer.ccbi", this) ;
    
    
    CCBReader* pReader2 = new CCBReader(ccNodeLoaderLibrary,this,this);
    pReader2->setResolutionScale(fCCBResoulution ) ;
    CCNode* pRoot = pReader2->readNodeGraphFromFile("ccbi/Room.ccbi", this) ;
    addChild(pRoot) ;
    addChild(pRoot2) ;
    return true ;
}

SEL_CCControlHandler CRoomLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    printf("abc") ;
    return NULL ;
}

bool CRoomLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    printf("abc") ;
    return NULL ;
}