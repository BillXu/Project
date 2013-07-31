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
//#include <iostream>
class CRoomPlayerInfor
:public CCBMemberVariableAssigner
,public CCNodeLoaderListener
,public CCSprite
{
public:
   static CRoomPlayerInfor* create();
    // variable
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    void update(float fTime );
    void StartTiming(){ m_bTiming = true ; m_fRunedTime = 0 ;m_pRadial->setVisible(true) ;}
    void StopTiming(){ m_bTiming = false ; m_fRunedTime = 0 ; m_pRadial->setVisible(false) ;}
    bool IsReady(){ return true ;}
protected:
    CCLabelTTF* m_pTitle,*m_pName,*m_pCoin,*m_pBetCoin ;
    CCSprite* m_pPhoto;
    CCProgressTimer* m_pRadial;
    float m_fRunedTime ;
    bool m_bTiming ;
};
#endif /* defined(__Gold__RoomPlayerInfor__) */
