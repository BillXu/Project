#pragma once
#include "CommonDefine.h"
#include "IPlayerComponent.h"
#include <string>
struct stMsg ;
class CPlayer ;
class CPlayerBaseData 
	:public IPlayerComponent
{
public:
	CPlayerBaseData(CPlayer*);
	void Init();
	bool OnMessage(stMsg* pMsg );
	void SendBaseDatToClient();
	void CPlayerBaseData::Reset();
public:
	int nCoin ;
	unsigned int nDiamoned ;
	unsigned char nTitle ;
	unsigned char nVipLevel ; // 0 is not vip ;
	unsigned char nSex ;   // eSex ;
	unsigned short nDefaulPhotoID ;
	int nUserDefinePhotoID ;
	int nYeastodayWinCoin ;
	int nSingleWinMost;
	unsigned short nWinTimes;
	unsigned short nLoseTimes ;
	int nQQNumber ;
	std::string strSigure;
	char strName[MAX_LEN_CHARACTER_NAME] ;
};