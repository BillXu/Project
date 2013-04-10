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
	 if ( MSG_TRANSER_DATA == pMsg->usMsgType )
	 {
		 stMsgTransferData* pMsgTransfer = (stMsgTransferData*)pMsg ;
		 stMsg* pTargetMessage = (stMsg*)(pData->data + sizeof(stMsgTransferData));
		 ProcessTransferedMsg(pTargetMessage,pMsgTransfer->nTargetPeerUID,pData->guid) ;
		 return false;
	 }
	 else if ( MSG_DISCONNECT == pMsg->usMsgType )
	 {
		stMsgPeerDisconnect* pRealMsg = (stMsgPeerDisconnect*)pMsg ;
		pTargetPlayer = GetPlayer(pRealMsg->nPeerUID) ;
		pTargetPlayer->OnDisconnected();
		// remove this player ;
		assert(0) ;
		return false ;
	 }
	 else
	 {
		 stMsgGM2DB* pRealMsg = (stMsgGM2DB*)pMsg ;
		 pTargetPlayer = GetPlayer(pRealMsg->nTargetUserUID) ;
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
	// remove players ;
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
			pAccountCheck->nAccountType = pRealMsg->nAccountType ;

			
			char* pBuffer = (char*)((char*)pMsg + sizeof(stMsgRegister));

			char pAccount[MAX_LEN_ACCOUNT] = { 0 } ;
			char pPassword [MAX_LEN_PASSWORD] = { 0 } ;

			if ( pAccountCheck->nAccountType != 0 ) 
			{
				// parse account 
				memcpy(pAccount,pBuffer,pRealMsg->nAccountLen) ;
				pBuffer += pRealMsg->nAccountLen ;

				// password
				memcpy(pPassword,pBuffer,pRealMsg->nPaswordLen) ;
				pBuffer += pRealMsg->nPaswordLen ;
			}
			else 
			{
				uint64_t tta = RakNet::RakPeerInterface::Get64BitUniqueRandomNumber();
				sprintf(pAccount,"%l64du",tta);
				sprintf(pPassword,"%l64du",tta);
			}


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
			pRequest->nSqlBufferLen = sprintf(pRequest->pSqlBuffer,"INSERT INTO `gamedb`.`account` (`Account`, `Password`, `CharacterName`, `AcountType`) VALUES ('%s', '%s', '%s', '%d');",pAccount,pPassword,pCharacterName,pAccountCheck->nAccountType ) ;
			CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
		}
		break;
	case MSG_LOGIN:
		{
			stMsgLogin* pRealMsg = (stMsgLogin*)pMsg ;
			stAccountCheckAndRegister* pAccountCheck = new stAccountCheckAndRegister ;
			m_vAccountChecks[nTargetUserUID] = pAccountCheck  ;
			pAccountCheck->bCheck = true ;
			pAccountCheck->nTempUsrUID = nTargetUserUID;
			pAccountCheck->nFromServerID = nFromNetUID ;

			// parse account 
			char pAccount[MAX_LEN_ACCOUNT] = { 0 } ;
			char* pBuffer = (char*)((char*)pMsg + sizeof(stMsgLogin));

			memcpy(pAccount,pBuffer,pRealMsg->nAccountLen) ;
			pBuffer += pRealMsg->nAccountLen ;

			// password
			char pPassword [MAX_LEN_PASSWORD] = { 0 } ;
			memcpy(pPassword,pBuffer,pRealMsg->nPaswordLen) ;

			pAccountCheck->strAccount = pAccount;
			pAccountCheck->strPassword = pPassword ;

			// send a DBRequest ;
			stDBRequest* pRequest = CDBRequestQueue::SharedDBRequestQueue()->GetReserveRequest();
			pRequest->eType = eRequestType_Select ;
			pRequest->nRequestUID = pAccountCheck->nTempUsrUID;
			pRequest->nRequestFlag = eDBRequest_AccountCheck;

			// format sql String ;
			char pAccountEString[MAX_LEN_ACCOUNT * 2 + 1 ] = {0} ;
			CDataBaseThread::SharedDBThread()->EscapeString(pAccountEString,pAccount,pRealMsg->nAccountLen + 1 ) ;
			pRequest->nSqlBufferLen = sprintf(pRequest->pSqlBuffer,"SELECT * FROM Account WHERE Account = '%s'",pAccountEString ) ;
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
		m_vAccountChecks.erase(iter) ;
		delete pAcountCheck ;
		return ;
	}
	
	// send msg to gameServer 
	stMsgTransferData msg ;
	msg.cSysIdentifer = ID_MSG_DB2GM ;
	msg.nTargetPeerUID = pAcountCheck->nTempUsrUID ;

	stMsgRegisterRet msgReal ;
	msgReal.bSuccess = pResult->nAffectRow >= 1 ;
	msgReal.nAccountType = pAcountCheck->nAccountType ;
	msgReal.nAccountLen = strlen(pAcountCheck->strAccount.c_str());
	if ( msgReal.bSuccess == false )
	{
		msgReal.nErrCode = 1 ;
#ifdef DEBUG
		if (msgReal.nAccountType == 0 )
		{
			CLogMgr::SharedLogMgr()->ErrorLog("what a pity , visitor login register account repeated ");
		}
#endif
	}
	unsigned short iLenStart = 0 ;
	memcpy(s_gBuffer,(void*)&msg,sizeof(msg)) ;
	iLenStart += sizeof(msg);
	memcpy(s_gBuffer +iLenStart,(void*)&msgReal,sizeof(msgReal)) ;
	iLenStart += sizeof(msgReal);
	if ( msgReal.nAccountType == 0 )  // visitor register , send back accound and password ;
	{
		sprintf(s_gBuffer + iLenStart,"%s",pAcountCheck->strAccount.c_str());
		iLenStart += msgReal.nAccountLen ;
	}
	CServerNetwork::SharedNetwork()->SendMsg((char*)s_gBuffer,iLenStart,pAcountCheck->nFromServerID,false);

	delete pAcountCheck ;
	m_vAccountChecks.erase(iter) ;
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
		m_vAccountChecks.erase(iter) ;
		delete pAcountCheck ;
		return ;
	}
	// if the acccound exist ;
	stMsgLoginRet msgRet ;
	//msgRet.nTempUserUID = pAcountCheck->nTempUsrUID ;
	if ( pResult->nAffectRow <= 0 )
	{
		msgRet.nRetFlag = 1 ;   // account don't exsit ;
		msgRet.bOk = false ;
	}
	else 
	{
		char* pRealPssword = pResult->vResultRows[0]->GetFiledByName("Password")->Value.pBuffer;
		if ( strcmp(pRealPssword,pAcountCheck->strPassword.c_str()))
		{
			msgRet.nRetFlag = 2 ; // password error ;
			msgRet.bOk = false ;
		}
		else
		{
			msgRet.bOk = false ;
			msgRet.nRetFlag = 0 ;
			unsigned int nUserUID = pResult->vResultRows[0]->GetFiledByName("UserUID")->Value.llValue;
			// allocate a new DBPlayer ;
			CDBPlayer* pPlayer = GetPlayer(nUserUID);
			if ( !pPlayer )
			{
				pPlayer = new CDBPlayer(pAcountCheck->nFromServerID);
				m_vPlayers[pAcountCheck->nTempUsrUID] = pPlayer ;
			}
			else
			{
				pPlayer->SetFromServerGUID(pAcountCheck->nFromServerID) ;
			}
			pPlayer->OnPassAcountCheck(nUserUID);
		}
	}
	
	// wrap transfer msg 
	stMsgTransferData msg ;
	msg.cSysIdentifer = ID_MSG_DB2GM ;
	msg.nTargetPeerUID = pAcountCheck->nTempUsrUID ;
	memcpy(s_gBuffer,(void*)&msg,sizeof(msg)) ;
	memcpy(s_gBuffer + sizeof(msg),(void*)&msgRet,sizeof(msgRet)) ;
	CServerNetwork::SharedNetwork()->SendMsg( s_gBuffer,sizeof(msgRet) + sizeof(msg),pAcountCheck->nFromServerID,false);
	m_vAccountChecks.erase(iter) ;
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
