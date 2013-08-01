//
//  RoomPlayerInfor.h
//  Gold
//
//  Created by Xu BILL on 13-7-27.
//
//

#ifndef __Gold__RoomPlayerInfor__
#define __Gold__RoomPlayerInfor__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CRoomPlayerInfor ;
class CRoomPlayerInforDelegate
{
public:
    virtual ~CRoomPlayerInforDelegate(){}
    virtual void OnClickRoomPlayerInfo(CRoomPlayerInfor* pPlayerInfo ) = 0 ;
};
//#include <iostream>
class CRoomPlayerInfor
:public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CCNodeLoaderListener
,public CCSprite
{
public:
   static CRoomPlayerInfor* create();
    // variable
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    void update(float fTime );
    void StartTiming(){ m_bTiming = true ; m_fRunedTime = 0 ;m_pRadial->setVisible(true) ;}
    void StopTiming(){ m_bTiming = false ; m_fRunedTime = 0 ; m_pRadial->setVisible(false) ;}
    bool IsReady(){ return true ;}
    void OnClickPhoto(CCObject*, CCControlEvent);
    void SetPlayerInfo( unsigned int nSessionID ,const char* pname , const char* ptitle, int nCoin , int nBetCoin);
    void UpdateCoinInfo(int nBetedCoin , int nOwnCoin);
protected:
    CCLabelTTF* m_pTitle,*m_pName,*m_pCoin,*m_pBetCoin ;
    CCControlButton* m_pbtnPhoto;
    
    CCProgressTimer* m_pRadial;
    float m_fRunedTime ;
    bool m_bTiming ;
    
    CC_SYNTHESIZE(unsigned int, m_nSessionID, SessionID) ;
    CC_SYNTHESIZE(CRoomPlayerInforDelegate*, m_pDelegete, Delegate) ;
};
#endif /* defined(__Gold__RoomPlayerInfor__) */
