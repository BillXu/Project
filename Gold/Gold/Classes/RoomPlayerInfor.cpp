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
// selector
SEL_MenuHandler CRoomPlayerInfor::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    printf("abc") ;
    return NULL ;
}

SEL_CCControlHandler CRoomPlayerInfor::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    printf("abc") ;
    return NULL ;
}

// variable
bool CRoomPlayerInfor::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    printf("abc") ;
    return false ;
}

// listener
void CRoomPlayerInfor::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    printf("abc") ;

}