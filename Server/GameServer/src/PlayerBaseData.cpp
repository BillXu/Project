#include "PlayerBaseData.h"
#include <string>
CPlayerBaseData::CPlayerBaseData(CPlayer* player )
	:IPlayerComponent(player)
{
	m_eType = ePlayerComponent_BaseData ;
	nCoin = 0 ;
	nDiamoned = 0 ;
	nTitle = 0 ;
	nVipLevel = 0 ;
	memset(strName,0,sizeof(strName));
	sprintf_s(strName,"%d","default") ;
	nDefaulPhotoID = 0 ;
	nUserDefinePhotoID = 0 ;
}

void CPlayerBaseData::Init()
{
	nCoin = 0 ;
	nDiamoned = 0 ;
	nTitle = 0 ;
	nVipLevel = 0 ;
	memset(strName,0,sizeof(strName));
	sprintf_s(strName,"%d","default") ;
	nDefaulPhotoID = 0 ;
	nUserDefinePhotoID = 0 ;
}

bool CPlayerBaseData::OnMessage(stMsg* pMsg )
{
	return false ;
}