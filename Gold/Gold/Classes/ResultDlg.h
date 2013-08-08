//
//  ResultDlg.h
//  Gold
//
//  Created by Xu BILL on 13-8-8.
//
//

#ifndef __Gold__ResultDlg__
#define __Gold__ResultDlg__

#include <iostream>
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
struct stResultData ;
class CRoomData ;
class CResultDlg
:public CCLayer
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    struct stResultInfo
    {
        CCLabelTTF* pName , *m_pResult, * m_pNumber ;
        void SetInfo(const char* pInfo , bool bWin , unsigned int nCoin );
        void Hide();
    };
public:
    bool init();
    void ShowDlg(std::list<stResultData*>& pst , CRoomData* pdata );
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){ return NULL ;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void OnClose(CCObject* p , CCControlEvent );
protected:
    stResultInfo m_vInfos[5];
};

#endif /* defined(__Gold__ResultDlg__) */
