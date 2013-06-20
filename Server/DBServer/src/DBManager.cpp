#include "DBManager.h"
#include "LogManager.h"
#include "DBRequest.h"
#include "ServerMessageDefine.h"
CDBManager::CDBManager()
{
	m_vReserverArgData.clear();
	m_vMsgFunc.clear();
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
	CHECK_MSG_SIZE_VOID(stMsg,packet->length);
	stMsg* pmsg = (stMsg*)packet->data ;
	MAP_MSG_FUNC::iterator iter = m_vMsgFunc.find(pmsg->usMsgType) ;
	if ( iter == m_vMsgFunc.end() )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("Recieved unregister msg , msg = %d",pmsg->usMsgType) ;
		return ;
	}
	stMsgAndDBRetFunc& ret = iter->second ;
	(this->*(ret.msgFunc))(packet) ;
}

void CDBManager::OnDBResult(stDBResult* pResult)
{
	unsigned int nid = pResult->nRequestUID ;
	MAP_MSG_FUNC::iterator iter = m_vMsgFunc.find(nid) ;
	if ( iter == m_vMsgFunc.end() )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("Get unregister DBResult ID , ID = %d",nid ) ;
		return ;
	}
	(this->*iter->second.dbretFunc)(pResult);
	if ( pResult->pUserData != NULL )
	{
		m_vReserverArgData.push_back((stArgData*)pResult->pUserData) ;
	}
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

void CDBManager::RegisterMsgFunc(unsigned int nMsg ,lpMsgFunc msgFunc , lpDBResultFunc bdresultFunc )
{
	MAP_MSG_FUNC::iterator iter = m_vMsgFunc.find(nMsg) ;
	if ( iter != m_vMsgFunc.end() )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("double register the same msg , msg = %d",nMsg ) ;
		m_vMsgFunc.erase(iter) ;
	}
	stMsgAndDBRetFunc twofunc ;
	twofunc.dbretFunc = bdresultFunc ;
	twofunc.msgFunc = msgFunc ;
	m_vMsgFunc[nMsg] = twofunc ;
}

// common msg rand db ret func
void RequestBaseDataMsg(RakNet::Packet*pmsg)
{

}

void RequestBasetDataDBResult(stDBResult* pResult)
{

}