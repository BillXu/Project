//
//  PKDlg.h
//  Gold
//
//  Created by Xu BILL on 13-8-6.
//
//

#ifndef __Gold__PKDlg__
#define __Gold__PKDlg__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
class CPKDlg ;
class CDlgDelegate
{
public:
   virtual void OnDlgEnd(CPKDlg* pDlg) {}
};

class CPKDlg
:public CCLayer
,public CCBMemberVariableAssigner
,public CCBAnimationManagerDelegate
{
public:
    bool init();
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void ShowDlg(const char* pLeft, const char* pRight, bool bLeftWin );
    void completedAnimationSequenceNamed(const char *name);
    void SetDelegate(CDlgDelegate*pDlegate){m_pDelegate = pDlegate ;}
    void SetPkIdx(char*pidx){ memcpy( m_vPKIdx, pidx, sizeof(m_vPKIdx ));}
    char* GetPkIdx(){ return m_vPKIdx ;}
protected:
    CCSprite* m_pLeft , *m_pRight , *m_pLeftPhoto, *m_pRightPhoto;
    CCBAnimationManager* m_pAnimate ;
    bool m_bLeftWin ;
    CDlgDelegate* m_pDelegate ;
    char m_vPKIdx[2];
};

#endif /* defined(__Gold__PKDlg__) */
