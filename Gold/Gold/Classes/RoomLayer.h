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
#include "RoomPlayerInfor.h"
#include "SelectAddBetCoin.h"
#include "SelectAddBetCoinLoader.h"
#include "CommonDefine.h"
USING_NS_CC ;
USING_NS_CC_EXT ;
struct stRoomPeerData ;
class CCard;
class CRoomData ;
class CRoomPlayerInfor ;
class CRoomLayer
:public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCBAnimationManagerDelegate
,public CRoomPlayerInforDelegate
,public CSelectAddBetCoinDelegate
{
public:
    typedef std::list<CCSprite*> LIST_SPRITE ;
public:
    static CCScene* RoomScene(int a , int b , int c , int d ,int e );
    virtual bool init(int a , int b , int c , int d ,int e );
    void SetRoomData(CRoomData* pdata ){ m_pRoomData = pdata ;}
    // delete gate ;
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){ return NULL ;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void completedAnimationSequenceNamed(const char *name);
    void OnClickRoomPlayerInfo(CRoomPlayerInfor* pPlayerInfo );
    void OnSelectedAddBetCoin(CSelectAddBetCoin* pBtn , int nCoin );

    // btn event
    void OnFollow(CCObject*, CCControlEvent);
    void OnAdd(CCObject*, CCControlEvent);
    void OnLook(CCObject*, CCControlEvent);
    void OnPK(CCObject*, CCControlEvent);
    void OnGiveUp(CCObject*, CCControlEvent);
    void OnBuy(CCObject*, CCControlEvent);
    void OnBack(CCObject*, CCControlEvent);
    void OnSay(CCObject*, CCControlEvent);
    void OnReady(CCObject*, CCControlEvent);
    void OnDistributeCardOver(float fTime );
    
    void StartGame(unsigned int nMainPlayer );
    
    void StartMyClock();
    void OnClocked(float fTime );
    void StopMyClock();
    
    // logic invoke 
    void OnPlayerLeave( char nIdx );
    void OnPlayerEnter( char nIdx , stRoomPeerData* pPlayerData );
    void OnRefreshRoomInfo(CRoomData*proomdata);
    void OnUpdatePlayerState(char nIdx , eRoomPeerState ePeerState );
    void OnDistributeCard();
    void OnWaitPlayerAction(char nIdx );
    void OnPlayerFollow(char nIdx , int nFollowedCoin );
    void OnPlayerAdd(char nIdx , int nOffsetCoin );
    void OnPlayerPK(char nIdxInvoke , char nIdxWith , bool bWin );
protected:
    void StartDistributeCard();
    void ResetRoomState();
    void RunPKIconAnimationByPlayerIdx(char nIdx );
    void StartPushCoinAnimation( char nIdx , unsigned int nCoin );
    void ClearShowingChouMa();
    void UpdateMyCoin();
    CCSprite* GetAutoSpriteByCard(CCard* pcard);
    void UpdateButton(bool bMyTurn = false );
    void StopPkIconAnimation();
protected:
    CCLabelTTF* m_pMyCoin ,*m_pMyDiamen,*m_pMyName, *m_pMyTitle,*m_pMyBetCoin, *m_pRound, *m_pTotalBet,*m_pSingleBet, *m_pTime;
    CCSprite* m_pTable,*m_pClock ;
    CCControlButton* m_pbtnFollow, *m_pbtnAdd,*m_pbtnLook,*m_pbtnPK,*m_pbtnGiveUp,*m_pbtnReady;
    CCBAnimationManager* animationManager  ;
    CRoomPlayerInfor* m_pPlayer[4];
    CCSprite* m_pDefault[5];
    CCSprite* m_pLook[4] ;
    CCSprite* m_pPKIcon[4] ;
    CCSprite* m_pGive[5];
    CCSprite* m_pFail[5] ;
    CCSprite* m_pReadyIcon[5] ;
    CCSprite* m_pCardSender ;
    CCSprite* m_pDistributeCard[15] ;
    CCPoint m_ptDistributePoint[5];
    CSelectAddBetCoin* m_pSelectAddBetCoin ;
    float m_fMyTimerCount ;  // for clock display ;
    CRoomData* m_pRoomData ;
    int m_vCoin[5] ; // chou ma ji bie ;
    LIST_SPRITE m_vShowingChouMa ;
    CCSprite* m_pLookShowCard[3] ;
    CCSprite* m_pMyGiveupIcon ;
    CCSprite* m_pMyFailedIcon ;
    bool m_bSelectingPKTarget ;
};

#endif /* defined(__Gold__RoomLayer__) */
