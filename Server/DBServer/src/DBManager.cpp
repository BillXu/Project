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
			pRequest->nSqlBufferLen = sprintf_s(pRequest->pSqlBuffer,"SELECT * FROM Account WHERE Account = '%s'",pAccountEString ) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	case MSG_PLAYER_REGISTER:
		{
			stMsgLoginRegister* pLoginRegister = (stMsgLoginRegister*)pmsg ;
			pdata->nSessionID = pLoginRegister->nSessionID ;
			pdata->nExtenArg1 = pLoginRegister->bAutoRegister ;

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
			pRequest->nSqlBufferLen = sprintf_s(pRequest->pSqlBuffer,"INSERT INTO `gamedb`.`account` (`Account`, `Password`, `CharacterName`, `Sex`) VALUES ('%s', '%s', '%s', '%d');",pAccountEString,pPasswordEString,pCharaNameEString,pLoginRegister->cSex ) ;
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