#include "DBPlayerManager.h"
#include "DBPlayer.h"
#include "DBRequest.h"
#include "ServerMessageDefine.h"
#include "CommonDefine.h"
#include "DBRequestFlags.h"
#include "DataBaseThread.h"
#include "LogManager.h"
CDBPlayerManager::CDBPlayerManager()
{
	ClearAllPlayers();
	ClearAccountCheck();
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
		 stMsg2DBLoginCheck* pMsgCheck = (stMsg2DBLoginCheck*)pMsg ;
		 stAccountCheck* pAccountCheck = new stAccountCheck ;
		 m_vAccountChecks.push_back(pAccountCheck) ;
		 pAccountCheck->nTempUsrUID = pMsgCheck->nTempUID;
		 pAccountCheck->nFromServerID = pData->guid ;
		
		 // pase account 
		 char pAccount[MAX_LEN_ACCOUNT] = { 0 } ;
		 char* pBuffer = (char*)((char*)pData->data + sizeof(stMsg2DBLoginCheck));
		 
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
		 pRequest->nSqlBufferLen = sprintf(pRequest->pSqlBuffer,"SELECT * FROM accountTable WHERE account = '%s'",pAccountEString ) ;
		 CDBRequestQueue::SharedDBRequestQueue()->PushRequest(pRequest) ;
	 }
	 else if ( MSG_LOGIN_DIRECT == pMsg->usMsgType )
	 {
		
	 }
		
	return false ;
}

void CDBPlayerManager::OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
{

}

void CDBPlayerManager::OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
{

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

void CDBPlayerManager::OnProcessAccountCheckResult(stDBResult* pResult)
{
	LIST_ACCOUNT_CHECK::iterator iter = m_vAccountChecks.begin();
	stAccountCheck* pAcountCheck = NULL ;
	for ( ; iter != m_vAccountChecks.end(); ++iter )
	{
		pAcountCheck = *iter ;
		if ( pAcountCheck != NULL && pAcountCheck->nTempUsrUID == pResult->nRequestUID )
		{
			m_vAccountChecks.erase(iter) ;
			break; ;
		}
	}

	if ( !pAcountCheck )
	{
		CLogMgr::SharedLogMgr()->ErrorLog( "Can not find Acount check tempID = %d",pResult->nRequestUID ) ;
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
		char* pRealPssword = pResult->vResultRows[0]->GetFiledByName("password")->Value.pBuffer;
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
