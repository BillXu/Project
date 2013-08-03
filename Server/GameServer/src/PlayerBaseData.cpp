#include "PlayerBaseData.h"
#include <string>
#include "MessageDefine.h"
#include "Player.h"
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
	nCoin = 19850 ;
	nDiamoned = 0 ;
	nTitle = 0 ;
	nVipLevel = 0 ;
	memset(strName,0,sizeof(strName));
	sprintf_s(strName,"%d","default") ;
	nDefaulPhotoID = 0 ;
	nUserDefinePhotoID = 0 ;

	// temp do 
	SendBaseDatToClient();
}

bool CPlayerBaseData::OnMessage(stMsg* pMsg )
{
	return false ;
}

void CPlayerBaseData::SendBaseDatToClient()
{
	stMsgPlayerBaseData msg ;
	msg.nSessionID = GetPlayer()->GetSessionID() ;
	SendMsgToClient((const char*)&msg,sizeof(msg));
}