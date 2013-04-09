#include "DBPlayerManager.h"
#include "DBPlayer.h"
#include "DBRequest.h"
#include "ServerMessageDefine.h"
#include "CommonDefine.h"
#include "DBRequestFlags.h"
#include "DataBaseThread.h"
#include "LogManager.h"
#include "MessageDefine.h"
char* CDBPlayerManager::s_gBuffer = NULL ;
CDBPlayerManager::CDBPlayerManager()
{
	ClearAllPlayers();
	ClearAccountCheck();
	if ( s_gBuffer == NULL )
	{
		s_gBuffer = new char[MAX_MSG_BUFFER_LEN] ;
	}
}

CDBPlayerManager::~CDBPlayerManager()
{
	ClearAllPlayers();
	ClearAccountCheck();
}

bool CDBPlayerManager::OnMessage( RakNet::Packet* pData )
{
     CDBPlayer* pTargetPlayer = NULL ;
	 stMsg* pMsg = (stMsg*)pData->data ;
	 if ( pMsg->usMsgType == MSG_LOGIN_CHECK )
	 {
		 stMsgGM2DBLoginCheck* pMsgCheck = (stMsgGM2DBLoginCheck*)pMsg ;
		 stAccountCheckAndRegister* pAccountCheck = new stAccountCheckAndRegister ;
		 m_vAccountChecks.push_back(pAccountCheck) ;
		 pAccountCheck->bCheck = true ;
		 pAccountCheck->nTempUsrUID = pMsgCheck->nTargetUserUID;
		 pAccountCheck->nFromServerID = pData->guid ;
		
		 // parse account 
		 char pAccount[MAX_LEN_ACCOUNT] = { 0 } ;
		 char* pBuffer = (char*)((char*)pData->data + sizeof(stMsgGM2DBLoginCheck));
		 
		 memcpy(pAccount,pBuffer,pMsgCheck->nAccountLen) ;
		 pBuffer += pMsgCheck->nAccountLen ;
		 
		 // password
		 char pPassword [MAX_LEN_PASSWORD] = { 0 } ;
		 memcpy(pPassword,pBuffer,pMsgCheck->nPasswordLen) ;

		 pAccountCheck->strAccount = pAccount;
		 pAccountCheck->strPassword = pPassword ;

		 // send a DBRequest ;
		 stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
		 pRequest->eType = eRequestType_Select ;
		 pRequest->nRequestUID = pAccountCheck->nTempUsrUID;
		 pRequest->nRequestFlag = eDBRequest_AccountCheck;

		 // format sql String ;
		 char pAccountEString[MAX_LEN_ACCOUNT * 2 + 1 ] = {0} ;
		 CDataBaseThread::SharedDBThread()->EscapeString(pAccountEString,pAccount,pMsgCheck->nAccountLen + 1 ) ;
		 pRequest->nSqlBufferLen = sprintf(pRequest->pSqlBuffer,"SELECT * FROM Account WHERE Account = '%s'",pAccountEString ) ;
		 CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
	 }
	 else if ( MSG_LOGIN_DIRECT == pMsg->usMsgType )
	 {
		stMsg2DBDirectLogin* pRealMsg = (stMsg2DBDirectLogin*)pMsg;
		pTargetPlayer = GetPlayer(pRealMsg->UserUID) ;
		if ( pTargetPlayer )
		{
			pTargetPlayer->SetFromServerGUID(pData->guid) ;
		}
		else
		{
			pTargetPlayer = new CDBPlayer(pData->guid);
			m_vPlayers.push_back(pTargetPlayer) ;
		}
	 }
	 else if ( MSG_TRANSER_DATA == pMsg->usMsgType )
	 {
		 stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMsg ;
		 stMsg* pTargetMessage = (stMsg*)(pData->data + sizeof(stMsgTransferData));
		 ProcessTransferedMsg(pTargetMessage,pMsgTransfer->nTargetPeerUID,pData->guid) ;
	 }
	
	 if ( pTargetPlayer )
		pTargetPlayer->OnMessage(pMsg) ;
	 return false ;
}

void CDBPlayerManager::OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
{
	CLogMgr::SharedLogMgr()->PrintLog("A GameServer Connected : %s ",pData->systemAddress.ToString(true));
}

void CDBPlayerManager::OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
{

	CLogMgr::SharedLogMgr()->PrintLog("A GameServer Lost : %s ",pData->systemAddress.ToString(true));

	LIST_DBPLAYER::iterator iter = m_vPlayers.begin();
	for ( ; iter != m_vPlayers.end(); ++iter )
	{
		CDBPlayer* pPlayer = *iter ;
		if ( pPlayer && pPlayer->GetState() == CDBPlayer::ePlayerState_Active )
		{
			pPlayer->OnDisconnected();
		}
	}
}

void CDBPlayerManager::ProcessDBResults()
{
	CDBRequestQueue::VEC_DBRESULT vResultOut ;
	CDBRequestQueue::SharedDBRequestQueue()->GetAllResult(vResultOut) ;
	CDBRequestQueue::VEC_DBRESULT::iterator iter = vResultOut.begin() ;
	for ( ; iter != vResultOut.end(); ++iter )
	{
		stDBResult* pRet = *iter ;
		OnProcessDBResult(pRet);
		delete pRet ;
	}
	vResultOut.clear();
}

CDBPlayer* CDBPlayerManager::GetPlayer( unsigned int nUID )
{
	//if ( eType < ePlayerType_None || eType >= ePlayerType_Max )
	//	return NULL ;
	LIST_DBPLAYER& vlist = m_vPlayers;
	LIST_DBPLAYER::iterator iter = vlist.begin();
	CDBPlayer* pPlayer = NULL ;
	for ( ; iter != vlist.end(); ++iter )
	{
		pPlayer = *iter ;
		if ( pPlayer && pPlayer->GetUserUID() == nUID )
			return pPlayer ;
	}
	return NULL ;
}

void CDBPlayerManager::OnProcessDBResult(stDBResult* pResult )
{
	if ( pResult->nRequestFlag == eDBRequest_AccountCheck )
	{
		OnProcessAccountCheckResult(pResult);
	}
	else if ( eDBRequest_Register == pResult->nRequestFlag )
	{
		OnProcessRegisterResult(pResult);
	}
	else
	{
		CDBPlayer* pPlayer = GetPlayer(pResult->nRequestUID) ;
		if ( pPlayer )
		{
			pPlayer->OnDBResult(pResult);
		}
		else
		{
			CLogMgr::SharedLogMgr()->ErrorLog( "can not find player id = %d",pResult->nRequestUID );
		}
	}
}

void CDBPlayerManager::ProcessTransferedMsg( stMsg* pMsg ,unsigned int nTargetUserUID , RakNet::RakNetGUID& nFromNetUID)
{
	switch ( pMsg->usMsgType )
	{
	case MSG_REGISTE:
		{
			stMsgRegister* pRealMsg = (stMsgRegister*)pMsg ;
			stAccountCheckAndRegister* pAccountCheck = new stAccountCheckAndRegister ;
			m_vAccountChecks[nTargetUserUID] = pAccountCheck  ;
			pAccountCheck->bCheck = false ;
			pAccountCheck->nTempUsrUID = nTargetUserUID;
			pAccountCheck->nFromServerID = nFromNetUID ;

			// parse account 
			char pAccount[MAX_LEN_ACCOUNT] = { 0 } ;
			char* pBuffer = (char*)((char*)pMsg + sizeof(stMsgRegister));

			memcpy(pAccount,pBuffer,pRealMsg->nAccountLen) ;
			pBuffer += pRealMsg->nAccountLen ;

			// password
			char pPassword [MAX_LEN_PASSWORD] = { 0 } ;
			memcpy(pPassword,pBuffer,pRealMsg->nPaswordLen) ;
			pBuffer += pRealMsg->nPaswordLen ;

			// parse name
			char pCharacterName[MAX_LEN_CHARACTER_NAME] = { 0 };
			memcpy(pCharacterName,pBuffer,pRealMsg->nCharacterNameLen) ;

			pAccountCheck->strAccount = pAccount;
			pAccountCheck->strPassword = pPassword ;
			pAccountCheck->strCharacterName = pCharacterName ;
			
			// send a DBRequest ;
			stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
			pRequest->eType = eRequestType_Add ;
			pRequest->nRequestUID = pAccountCheck->nTempUsrUID;
			pRequest->nRequestFlag = eDBRequest_Register;

			// format sql String ;
			pRequest->nSqlBufferLen = sprintf(pRequest->pSqlBuffer,"INSERT INTO `gamedb`.`account` (`Account`, `Password`, `CharacterName`) VALUES ('%s', '%s', '%s');",pAccount,pPassword,pCharacterName ) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	default:
		break; 
	}
}

void CDBPlayerManager::OnProcessRegisterResult(stDBResult* pResult)
{
	stAccountCheckAndRegister* pAcountCheck = NULL ;
	MAP_ACCOUNT_CHECK_REGISTER::iterator iter = m_vAccountChecks.find(pResult->nRequestUID);
	if ( iter != m_vAccountChecks.end() )
	{
		pAcountCheck = iter->second ;
	}

	if ( !pAcountCheck )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Can not find Account register tempID = %d",pResult->nRequestUID ) ;
		delete pAcountCheck ;
		return ;
	}
	
	// send msg to gameServer 
	stMsgTransferData msg ;
	msg.cSysIdentifer = ID_MSG_DB2GM ;
	msg.nTargetPeerUID = pAcountCheck->nTempUsrUID ;

	stMsgRegisterRet msgReal ;
	msgReal.bSuccess = pResult->nAffectRow >= 1 ;
	if ( msgReal.bSuccess == false )
		msgReal.nErrCode = 1 ;
	memcpy(s_gBuffer,(void*)&msg,sizeof(msg)) ;
	memcpy(s_gBuffer + sizeof(msg),(void*)&msgReal,sizeof(msgReal)) ;
	CServerNetwork::SharedNetwork()->SendMsg((char*)s_gBuffer,sizeof(msgReal) + sizeof(msg),pAcountCheck->nFromServerID,false);
}

void CDBPlayerManager::OnProcessAccountCheckResult(stDBResult* pResult)
{
	stAccountCheckAndRegister* pAcountCheck = NULL ;
	MAP_ACCOUNT_CHECK_REGISTER::iterator iter = m_vAccountChecks.find(pResult->nRequestUID);
	if ( iter != m_vAccountChecks.end() )
	{
		pAcountCheck = iter->second ;
	}

	if ( !pAcountCheck )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Can not find Account check tempID = %d",pResult->nRequestUID ) ;
		delete pAcountCheck ;
		return ;
	}
	// if the acccound exist ;
	stMsg2DBLoginCheckRet msgRet ;
	msgRet.nTempUserUID = pAcountCheck->nTempUsrUID ;
	if ( pResult->nAffectRow <= 0 )
	{
		msgRet.nRetFlag = 1 ;   // account don't exsit ;
	}
	else 
	{
		char* pRealPssword = pResult->vResultRows[0]->GetFiledByName("Password")->Value.pBuffer;
		if ( strcmp(pRealPssword,pAcountCheck->strPassword.c_str()))
		{
			msgRet.nRetFlag = 2 ; // password error ;
		}
		else
		{
			msgRet.nRetFlag = 0 ;
			msgRet.nUserUID = pResult->vResultRows[0]->GetFiledByName("UserUID")->Value.llValue;
			// allocate a new DBPlayer ;
			CDBPlayer* pPlayer = GetPlayer(msgRet.nUserUID);
			if ( !pPlayer )
			{
				pPlayer = new CDBPlayer(pAcountCheck->nFromServerID);
				m_vPlayers.push_back(pPlayer);
			}
			else
			{
				pPlayer->SetFromServerGUID(pAcountCheck->nFromServerID) ;
			}
			pPlayer->OnPassAcountCheck(msgRet.nUserUID);
		}
	}
	
	CServerNetwork::SharedNetwork()->SendMsg( (char*)&msgRet,sizeof(msgRet),pAcountCheck->nFromServerID,false);
	delete pAcountCheck ;
}

//void CDBPlayerManager::RemoveDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer )
//{
//	if ( !pPlayer )
//		return ;
//	LIST_DBPLAYER::iterator iter = vPlayers.begin();
//	for ( ; iter != vPlayers.end(); ++iter )
//	{
//		if ( *iter == pPlayer )
//		{
//			vPlayers.erase(iter) ;
//			return ;
//		}
//	}
//}
//
//void CDBPlayerManager::DeleteDBPlayer(LIST_DBPLAYER& vPlayers , CDBPlayer* pPlayer)
//{
//	LIST_DBPLAYER::iterator iter = vPlayers.begin() ;
//	for ( ; iter != vPlayers.end(); ++iter )
//	{
//		if ( pPlayer == NULL )
//		{
//			delete *iter ;
//			*iter = NULL ;
//			continue; 
//		}
//		
//		if ( *iter == pPlayer )
//		{
//			delete *iter ;
//			*iter = NULL ;
//			vPlayers.erase(iter) ;
//			return ;
//		}
//	}
//
//	if ( pPlayer == NULL )
//		vPlayers.clear() ;
//}

void CDBPlayerManager::ClearAllPlayers()
{
	LIST_DBPLAYER::iterator iter = m_vPlayers.begin();
	for ( ; iter != m_vPlayers.end(); ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vPlayers.clear() ; 
}

void CDBPlayerManager::ClearAccountCheck()
{
	LIST_ACCOUNT_CHECK::iterator iter = m_vAccountChecks.begin();
	for ( ; iter != m_vAccountChecks.end(); ++iter )
	{
		delete *iter ;
		*iter = NULL ;
	}
	m_vAccountChecks.clear() ;
}
