//
//  SelectAddBetCoin.h
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#ifndef __Gold__SelectAddBetCoin__
#define __Gold__SelectAddBetCoin__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CSelectAddBetCoin ;
class CSelectAddBetCoinDelegate
{
public:
    virtual void OnSelectedAddBetCoin(CSelectAddBetCoin* pBtn , int nCoin ){};
};

class CSelectAddBetCoin
:public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CCNodeLoaderListener
,public CCSprite
{
public:
    static CSelectAddBetCoin* create();
    // variable
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);

    // event ;
    void OnSelected(CCObject*, CCControlEvent);
    void InitSelectedMoney(int a , int b , int c , int d , int e );
    void SetMinEnable( int nCoin , bool bEnableDouble );
    void SetDelegate(CSelectAddBetCoinDelegate* pDelegate ){ m_pDelegate = pDelegate ;}
protected:
    void setNumber(int nCoin, int nIndex );
protected:
    CCControlButton* m_pCoin[5] , * m_pDouble ;
    int m_vCoin[5] ;
    CSelectAddBetCoinDelegate* m_pDelegate ;
};

#endif /* defined(__Gold__SelectAddBetCoin__) */
