#include "DBManager.h"
#include "LogManager.h"
#include "DBRequest.h"
#include "ServerMessageDefine.h"
#include "DBApp.h"
#include "DataBaseThread.h"
CDBManager::CDBManager(CDBServerApp* theApp )
{
	m_vReserverArgData.clear();
	m_pTheApp = theApp ;
	nCurUserUID = 0 ;
}

CDBManager::~CDBManager()
{
	LIST_ARG_DATA::iterator iter = m_vReserverArgData.begin() ;
	for ( ; iter != m_vReserverArgData.end() ; ++iter )
	{
		if ( *iter )
		{
			delete *iter ;
			*iter = NULL ;
		}
	}
	m_vReserverArgData.clear() ;
}

void CDBManager::Init()
{
	// register funcs here ;
}

void CDBManager::OnMessage(RakNet::Packet* packet)
{
	// construct sql
	stArgData* pdata = GetReserverArgData() ;
	if ( pdata == NULL )
	{
		pdata = new stArgData ;
	}

	pdata->m_nReqrestFromAdd = packet->guid ;
	stMsg* pmsg = (stMsg*)packet->data ;
	switch( pmsg->usMsgType )
	{
	case MSG_PLAYER_CHECK_ACCOUNT:
		{
			stMsgLoginAccountCheck* pLoginCheck = (stMsgLoginAccountCheck*)pmsg ;
			pdata->nSessionID = pLoginCheck->nSessionID ;
			
			char* paccount = NULL ;
			char * ppassword = NULL ;
			char* pbuffer = (char*)pLoginCheck ;
			pbuffer += sizeof(stMsgLoginAccountCheck) ;
			paccount = pbuffer ;

			pbuffer += pLoginCheck->nAccountLen ;
			ppassword = new char[pLoginCheck->nPasswordLen + 1 ] ;
			memset(ppassword,0,	pLoginCheck->nPasswordLen + 1  );
			memcpy(ppassword,pbuffer,pLoginCheck->nPasswordLen);
			pdata->pUserData = ppassword ;    // must delete when processed the result ;
			
			stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
			pRequest->cOrder = 1 ;
			pRequest->eType = eRequestType_Select ;
			pRequest->nRequestUID = pmsg->usMsgType ;
			pRequest->pUserData = pdata ;
			// format sql String ;
			char pAccountEString[MAX_LEN_ACCOUNT * 2 + 1 ] = {0} ;
			CDataBaseThread::SharedDBThread()->EscapeString(pAccountEString,paccount,pLoginCheck->nAccountLen ) ;
			pRequest->nSqlBufferLen = sprintf_s(pRequest->pSqlBuffer,"SELECT Password,UserUID FROM Account WHERE Account = '%s'",pAccountEString ) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	case MSG_PLAYER_REGISTER:
		{
			stMsgLoginRegister* pLoginRegister = (stMsgLoginRegister*)pmsg ;
			pdata->nSessionID = pLoginRegister->nSessionID ;
			pdata->nExtenArg1 = pLoginRegister->bAutoRegister ;
			pdata->nExtenArg2 = GenerateUserUID();

			char* paccount = NULL , *ppassword = NULL , *pcharaName = NULL;
			char* pBuffer = (char*)pmsg ;
			pBuffer += sizeof(stMsgLoginRegister);
			paccount = pBuffer ;
			pBuffer += pLoginRegister->ppPasswordLen ;
			ppassword = pBuffer ;
			pBuffer += pLoginRegister->ppPasswordLen ;
			pcharaName = pBuffer ;

			stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
			pRequest->cOrder = 1 ;
			pRequest->eType = eRequestType_Add ;
			pRequest->nRequestUID = pmsg->usMsgType ;
			pRequest->pUserData = pdata ;
			// format sql String ;
			char pAccountEString[MAX_LEN_ACCOUNT * 2 + 1 ] = {0} ;
			char pPasswordEString[MAX_LEN_PASSWORD * 2 + 1 ] = {0} ;
			char pCharaNameEString[MAX_LEN_PASSWORD * 2 + 1 ] = {0} ;
			CDataBaseThread::SharedDBThread()->EscapeString(pAccountEString,paccount,pLoginRegister->pAcoundLen ) ;
			CDataBaseThread::SharedDBThread()->EscapeString(pPasswordEString,ppassword,pLoginRegister->ppPasswordLen ) ;
			CDataBaseThread::SharedDBThread()->EscapeString(pCharaNameEString,pcharaName,pLoginRegister->ncharNameLen ) ;
			pRequest->nSqlBufferLen = sprintf_s(pRequest->pSqlBuffer,"INSERT INTO `gamedb`.`account` (`Account`, `Password`, `CharacterName`, `Sex`, `UserUID`) VALUES ('%s', '%s', '%s', '%d');",pAccountEString,pPasswordEString,pCharaNameEString,pLoginRegister->cSex ,pdata->nExtenArg2) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	case MSG_PLAYER_BASE_DATA:
		{
			stMsgGameServerGetBaseData* pbasedata = (stMsgGameServerGetBaseData*)pmsg ;
			pdata->nSessionID = pbasedata->nSessionID ;
			pdata->nExtenArg1 = pbasedata->nUserUID ;

			stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
			pRequest->cOrder = 1 ;
			pRequest->eType = eRequestType_Select ;
			pRequest->nRequestUID = pmsg->usMsgType ;
			pRequest->pUserData = pdata;
			pRequest->nSqlBufferLen = sprintf_s(pRequest->pSqlBuffer,"SELECT * FROM Account WHERE UserUID = '%d'",pbasedata->nUserUID) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	default:
		{
			m_vReserverArgData.push_back(pdata) ;
			CLogMgr::SharedLogMgr()->ErrorLog("unknown msg type = %d",pmsg->usMsgType ) ;
		}
	}
}

void CDBManager::OnDBResult(stDBResult* pResult)
{
	// process result 
	stArgData*pdata = (stArgData*)pResult->pUserData ;
	switch ( pResult->nRequestUID )
	{
	case  MSG_PLAYER_REGISTER:
		{
			stMsgLoginRegisterRet msgRet ;
			msgRet.bAuto = (bool)pdata->nExtenArg1;
			msgRet.nSessionID = pdata->nSessionID;
			if ( pResult->nAffectRow > 0 )
			{
				msgRet.nRet = 0 ;
				msgRet.nUserID = pdata->nExtenArg2 ;
			}
			else
			{
				msgRet.nRet = 1 ;
				msgRet.nUserID = 0 ;
			}
			m_pTheApp->SendMsg((char*)&msgRet,sizeof(msgRet),pdata->m_nReqrestFromAdd) ;
		}
		break;
	case MSG_PLAYER_CHECK_ACCOUNT:
		{
			stMsgLoginAccountCheckRet msgRet ;
			msgRet.nSessionID = pdata->nSessionID ;
			if ( pResult->nAffectRow > 0 )
			{
				CMysqlRow* pRow = pResult->vResultRows.front() ;
				// check password 
				if ( strcmp((char*)pdata->pUserData,pRow->GetFiledByName("Password")->BufferData()) == 0 )
				{
					msgRet.nRet = 0 ;
					msgRet.nUserID = pRow->GetFiledByName("UserUID")->IntValue() ;
					CLogMgr::SharedLogMgr()->PrintLog("check account success") ;
				}
				else
				{
					msgRet.nRet = 2 ;  // password error ;
					msgRet.nUserID = 0 ;
					CLogMgr::SharedLogMgr()->PrintLog("check account password error") ;
				}
			}
			else
			{
				msgRet.nRet = 1 ;  // account error ;   
				msgRet.nUserID = 0 ;
				CLogMgr::SharedLogMgr()->PrintLog("check account  account error") ;
			}
			m_pTheApp->SendMsg((char*)&msgRet,sizeof(msgRet),pdata->m_nReqrestFromAdd) ;
			delete [] pdata->pUserData ;
			pdata->pUserData = NULL ;
		}
		break;
	case MSG_PLAYER_BASE_DATA:
		{
			stArgData* pdata = (stArgData*)pResult->pUserData ;
			stMsgGameServerGetBaseDataRet msg ;
			msg.nSessionID = pdata->nSessionID ;
			if ( pResult->nAffectRow < 0 )
			{
				CLogMgr::SharedLogMgr()->ErrorLog("can not find base data with userUID = %d " , pdata->nExtenArg1 ) ;
				msg.nCoin = 0 ;
				msg.nDefaulPhotoID = 1 ;
				msg.nDiamoned = 0 ;
				msg.nLoseTimes = 0 ;
				msg.nNameLen = 0 ;
				msg.nQQNumber = 0 ;
				msg.nSex = 0 ;
				msg.nSigureLen = 0 ;
				msg.nSingleWinMost = 0 ;
				msg.nTitle = 0 ;
				msg.nUserDefinePhotoID = 0 ;
				msg.nVipLevel = 0 ;
				msg.nWinTimes = 0 ;
				msg.nYeastodayWinCoin = 0 ;
				m_pTheApp->SendMsg((char*)&msg,sizeof(msg),pdata->m_nReqrestFromAdd) ;
			}
			else
			{
				CMysqlRow& pRow = *pResult->vResultRows.front(); 
				msg.nCoin = pRow["nCoin"]->IntValue();
				msg.nDefaulPhotoID = pRow["nDefaulPhotoID"]->IntValue();
				msg.nDiamoned = pRow["nDiamoned"]->IntValue();
				msg.nLoseTimes = pRow["nLoseTimes"]->IntValue();
				msg.nNameLen = strlen(pRow["CharacterName"]->CStringValue());
				msg.nQQNumber = pRow["nQQNumber"]->IntValue();
				msg.nSex = pRow["nSex"]->IntValue();;
				msg.nSigureLen = strlen(pRow["strSigure"]->CStringValue());
				msg.nSingleWinMost = pRow["nSingleWinMost"]->IntValue();
				msg.nTitle = pRow["nTitle"]->IntValue();
				msg.nUserDefinePhotoID = pRow["nUserDefinePhotoID"]->IntValue();
				msg.nVipLevel = pRow["nVipLevel"]->IntValue();
				msg.nWinTimes = pRow["nWinTimes"]->IntValue();
				msg.nYeastodayWinCoin = pRow["nYeastodayWinCoin"]->IntValue();
				
				int nAllLen = sizeof(msg) + msg.nNameLen + msg.nSigureLen ;
				char* pBuffer = new char[nAllLen] ;
				int nOffset = 0 ;
				memcpy(pBuffer,(char*)&msg,sizeof(msg));
				nOffset += sizeof(msg);

				memcpy(pBuffer+nOffset,pRow["CharacterName"]->CStringValue(),msg.nNameLen);
				nOffset += msg.nNameLen ;

				memcpy(pBuffer+nOffset,pRow["strSigure"]->CStringValue(),msg.nSigureLen);
				nOffset += msg.nSigureLen ;
				m_pTheApp->SendMsg(pBuffer,nOffset,pdata->m_nReqrestFromAdd) ;
			}
		}
		break;
	default:
		{
			CLogMgr::SharedLogMgr()->ErrorLog("unprocessed db result msg id = %d ", pResult->nRequestUID );
		}
	}
	m_vReserverArgData.push_back(pdata) ;
}

CDBManager::stArgData* CDBManager::GetReserverArgData()
{
	LIST_ARG_DATA::iterator iter = m_vReserverArgData.begin() ;
	if ( iter != m_vReserverArgData.end() )
	{
		stArgData* p = *iter ;
		m_vReserverArgData.erase(iter) ;
		p->Reset();
		return p ;
	}
	return NULL ;
}