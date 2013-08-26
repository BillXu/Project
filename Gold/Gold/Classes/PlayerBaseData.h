//
//  PlayerBaseData.h
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#ifndef __Gold__PlayerBaseData__
#define __Gold__PlayerBaseData__

#include <iostream>
#include "IPlayerData.h"
#include "MessageDefine.h"
#include "RoomData.h"
#include "cocos2d.h"
USING_NS_CC ;
class CPlayerBaseData
:public IPlayerDataBase
{
public:
    bool OnMessage( stMsg* pMessage );
public:
    CC_SYNTHESIZE(unsigned int, m_nSessionID, SessionID) ;
    CC_SYNTHESIZE(int, m_nCoin, Coin) ;
    CC_SYNTHESIZE(int, m_nDiamoned, Diamoned) ;
    CC_SYNTHESIZE(unsigned char, m_nTitle, Title) ;
    CC_SYNTHESIZE(unsigned char, m_nVipLevel, VipLevel) ;
    CC_SYNTHESIZE(unsigned char, m_nSex, Sex) ;
    CC_SYNTHESIZE(unsigned short, m_nDefaulPhotoID, DefaulPhotoID) ;
    CC_SYNTHESIZE(int, m_nUserDefinePhotoID, UserDefinePhotoID) ;
    CC_SYNTHESIZE(unsigned short, m_nYeastodayWinCoin, YeastodayWinCoin) ;
    CC_SYNTHESIZE(int, m_nSingleWinMost, SingleWinMost) ;
    CC_SYNTHESIZE(unsigned short, m_nWinTimes, WinTimes) ;
    CC_SYNTHESIZE(unsigned short, m_nLoseTimes, LoseTimes) ;
    CC_SYNTHESIZE(int, m_nQQNumber, QQNumber) ;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_strName, Name) ;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_strSigure, Sigure) ;
};

#endif /* defined(__Gold__PlayerBaseData__) */
