//
//  GoldRoomPeerData.h
//  Gold
//
//  Created by Xu BILL on 13-8-26.
//
//

#ifndef __Gold__GoldRoomPeerData__
#define __Gold__GoldRoomPeerData__
#include "cocos2d.h"
#include "MessageDefine.h"
#include "CommonDefine.h"
struct stGoldRoomPeerData
{
    void SetData(stRoomPeerBrifInfo*pdata);
    bool IsActive();
    virtual bool IsSelf(){ return false ; }
    void AddBetCoin(int nCoin );
protected:
    CC_PROPERTY(unsigned int,nCoin, Coin) ;
    CC_SYNTHESIZE(unsigned int,nUserDefinePhotoID, UserDefinePhotoID) ;
    CC_SYNTHESIZE(unsigned int, nBetCoin,BetCoin) ;
    CC_SYNTHESIZE(unsigned char,nIdx, Idx) ;
    CC_SYNTHESIZE(eRoomPeerState,ePeerState, PeerState) ;
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::string, strTitle, Title) ;
    CC_SYNTHESIZE_PASS_BY_REF(std::string,stName, Name) ;
    CC_SYNTHESIZE(unsigned char,nDefaulPhotoID, DefaulPhotoID) ;
    CC_SYNTHESIZE(unsigned int, nSessionID,SessionID) ;
    CC_SYNTHESIZE(bool, bIsRecievedUserDefinePhoto,IsRecievedUserDefinePhoto) ;
};

// self data in room ;
struct stGoldRoomSelfData
:public stGoldRoomPeerData
{
    stGoldRoomSelfData();
    unsigned int getCoin();
    void setCoin(unsigned int var);
    virtual bool IsSelf(){ return true ; }
public:
    char vCard[3] ;
};

#endif /* defined(__Gold__GoldRoomPeerData__) */
