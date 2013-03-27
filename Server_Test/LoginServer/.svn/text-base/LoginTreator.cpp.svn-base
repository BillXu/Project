#include "LoginTreator.h"
#include <string.h>
#include <assert.h>
#include "StartServer.h"

CLoginTreator::CLoginTreator()
{
	m_bRun = true;
}
CLoginTreator::~CLoginTreator()
{
}

void CLoginTreator::__run()
{
	InternalMsg* pInmsg = NULL;
	while(m_bRun)
	{
		if( m_semMsg.Wait() < 0 )
		{
			if(EINTR == errno)
			{
				continue;
			}
			else
			{
				assert(0);
			}
		}
		if(Get(&pInmsg))
		{
			CHandler* pHandler = pInmsg->pHandler;
			switch(pInmsg->pMsg->cMsgID)
			{
				case eMsg_C2S_Login:
				{
					stC2SMsgLogin msg;
					stC2SMsgLogin* pMsg = (stC2SMsgLogin*)(pInmsg->pMsg);
					
					msg.pName = new char[pMsg->nNameLen];
					memset(msg.pName, 0, pMsg->nNameLen);
					int offset = sizeof(stC2SMsgLogin);
					memcpy(msg.pName, (char*)pMsg+offset, pMsg->nNameLen);
					msg.nNameLen = pMsg->nNameLen;
					
					offset += pMsg->nNameLen;
					msg.pPassword = new char[pMsg->nPasswordLen];
					memset(msg.pPassword, 0, pMsg->nPasswordLen);
					memcpy(msg.pPassword, (char*)pMsg+offset, pMsg->nPasswordLen);
					msg.nPasswordLen = pMsg->nPasswordLen;
					
					OnLogin(&msg, pHandler->GetGuid());
					delete []msg.pName;
					delete []msg.pPassword;
				}
				break;
				case INMSG_DB_LOGIN:
				{
				}
				break;
				case INMSG_GAME_LOGIN:
				{
				}
				break;
				default:
				break;
			}
		}
		if(pInmsg) delete pInmsg;
		pInmsg = NULL;
	}
}

void CLoginTreator::OnLogin(stC2SMsgLogin* pMsg, uint64_t guid)
{
	char select[BUF_SIZE1];
	memset( select,0, sizeof( select ));
	char name[BUF_SIZE1];
	char pwd[BUF_SIZE1];
	memset(name, 0, sizeof(name));
	memset(pwd, 0, sizeof(pwd));
	memcpy(name, pMsg->pName, pMsg->nNameLen);
	memcpy(pwd, pMsg->pPassword, pMsg->nPasswordLen);
	sprintf(select, "select * from user where name='%s' and password='%s'", name, pwd);
	
	char buf[BUF_SIZE1];
	stL2DLogin* pSel = (stL2DLogin*)buf;
	pSel->cSysIdentifer = INMSG_LOGIN_DB;
	pSel->cMsgID = L2D_LOGIN;//pMsg->cMsgID;
	pSel->cmdType = L2D_LOGIN;
	pSel->guid = guid;
	pSel->nSelLen = strlen(select);
	pSel->select = NULL;
	int len=sizeof(stL2DLogin);
	
	memcpy(buf+len, select, pSel->nSelLen);
	len += pSel->nSelLen;
	
	StartServer->SendToDB( buf, len);
}


	
