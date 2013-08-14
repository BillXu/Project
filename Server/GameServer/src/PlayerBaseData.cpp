#include "PlayerBaseData.h"
#include <string>
#include "MessageDefine.h"
#include "ServerMessageDefine.h"
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
	switch( pMsg->usMsgType )
	{
	case MSG_PLAYER_BASE_DATA:   // from db server ;
		{
			stMsgGameServerGetBaseDataRet* pBaseData = (stMsgGameServerGetBaseDataRet*)pMsg ;
			nCoin = pBaseData->nCoin;
			nDefaulPhotoID = pBaseData->nDefaulPhotoID;
			nDiamoned = pBaseData->nDiamoned;
			nLoseTimes = pBaseData->nLoseTimes ;
			nQQNumber = pBaseData->nQQNumber ;
			nSex = pBaseData->nSex ;
			nSingleWinMost = pBaseData->nSingleWinMost ;
			nTitle = pBaseData->nTitle ;
			nUserDefinePhotoID = pBaseData->nUserDefinePhotoID;
			nVipLevel = pBaseData->nVipLevel ;
			nWinTimes = pBaseData->nWinTimes ; 
			nYeastodayWinCoin = pBaseData->nYeastodayWinCoin ;
			char *pBuffer = (char*)pMsg ;
			pBuffer += sizeof(stMsgGameServerGetBaseDataRet);
			memset(strName,0,sizeof(strName));
			memcpy(strName,pBuffer,pBaseData->nNameLen );
			pBuffer += pBaseData->nNameLen ;

			strSigure = std::string(pBuffer,pBaseData->nSigureLen);

			return true ;
		}
		break;
	default:
		{
			return false ;
		}
		break;
	}
	return false ;
}

void CPlayerBaseData::SendBaseDatToClient()
{
	stMsgPlayerBaseData msg ;
	msg.nSessionID = GetPlayer()->GetSessionID() ;
	msg.nCoin = nCoin ;
	msg.nDefaulPhotoID = nDefaulPhotoID ;
	msg.nDiamoned = nDiamoned ;
	msg.nLoseTimes = nLoseTimes ;
	msg.nNameLen = strlen(strName);
	msg.nQQNumber = nQQNumber ;
	msg.nSex = nSex ;
	msg.nSigureLen = strSigure.size();
	msg.nSingleWinMost = nSingleWinMost;
	msg.nTitle = nTitle;
	msg.nUserDefinePhotoID = nUserDefinePhotoID ;
	msg.nVipLevel = nVipLevel;
	msg.nWinTimes = nWinTimes ;
	msg.nYeastodayWinCoin = nYeastodayWinCoin ;
	
	int nlen = sizeof(msg) + msg.nSigureLen + msg.nNameLen ;
	int nOffset = 0 ;
	char* pbuffer = new char [nlen] ;
	memcpy(pbuffer,(char*)&msg,sizeof(msg));
	nOffset += sizeof(msg);

	memcpy(pbuffer + nOffset , strName, msg.nNameLen ) ;
	nOffset += msg.nNameLen ;

	memcpy(pbuffer + nOffset , strSigure.c_str(), msg.nSigureLen );
	nOffset+=msg.nSigureLen ;
	SendMsgToClient(pbuffer	,nOffset );
	delete[] pbuffer ;
}