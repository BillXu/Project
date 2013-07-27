//
//  RoomLayer.h
//  Gold
//
//  Created by Xu BILL on 13-7-26.
//
//

#ifndef __Gold__RoomLayer__
#define __Gold__RoomLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CRoomPlayerInfor ;
class CRoomLayer
:public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static CCScene* RoomScene();
    virtual bool init();
    // delete gate ;
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){ return NULL ;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // btn event
   void OnFollow(CCObject*, CCControlEvent);
   void OnAdd(CCObject*, CCControlEvent);
   void OnLook(CCObject*, CCControlEvent);
   void OnPK(CCObject*, CCControlEvent);
   void OnGiveUp(CCObject*, CCControlEvent);
   void OnBuy(CCObject*, CCControlEvent);
   void OnBack(CCObject*, CCControlEvent);
   void OnSay(CCObject*, CCControlEvent);
protected:
   CCLabelTTF* m_pMyCoin ,*m_pMyDiamen,*m_pMyName, *m_pMyTitle,*m_pMyBetCoin, *m_pRound, *m_pTotalBet,*m_pSingleBet, *m_pTime;
   CCSprite* m_pTable,*m_pClock ;
   CCBAnimationManager* animationManager  ;
   CRoomPlayerInfor* m_pPlayer[4];
};

#endif /* defined(__Gold__RoomLayer__) */
