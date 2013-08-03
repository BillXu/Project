#pragma once
#include "CommonDefine.h"
#include "IPlayerComponent.h"
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
public:
	int nCoin ;
	unsigned int nDiamoned ;
	unsigned char nTitle ;
	unsigned char nVipLevel ; // 0 is not vip ;
	char strName[MAX_LEN_CHARACTER_NAME] ;
	int nDefaulPhotoID ;
	int nUserDefinePhotoID ;
};