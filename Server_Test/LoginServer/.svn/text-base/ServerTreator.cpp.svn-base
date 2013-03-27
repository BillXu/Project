#include "ServerTreator.h"
#include <string.h>
#include <assert.h>
#include "User.h"
#include "../Base/InternalMsg.h"
#include "../Base/Handler.h"
#include "../Base/ListenThread.h"
#include "StartServer.h"

CServerTreator::CServerTreator()
{
	m_bRun = true;
}
CServerTreator::~CServerTreator()
{
}

void CServerTreator::__run()
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
			if(INMSG_DB_LOGIN == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case D2L_LOGIN:
					{
						stServerMsg* smsg = (stServerMsg*)pInmsg->pMsg;
						int res = smsg->cmdType;
						if(0 == res)
						{
							HandleLoginSucess(pInmsg);
						}
						else
						{
							HandleLoginErr(pInmsg);
						}
					}
					break;
					default:
					break;
				}
			}
			else if(INMSG_GAME_LOGIN == pInmsg->pMsg->cSysIdentifer)
			{
			}
			else {assert(0);}
		}
		if(pInmsg) delete pInmsg;
		pInmsg = NULL;
	}
}

void CServerTreator::HandleLoginSucess(InternalMsg* pInmsg)
{
	//CHandler* pHandler = pInmsg->pHandler;
	stD2LUser* pUserInfo = (stD2LUser*)(pInmsg->pMsg);
	stUserInfo userInfo;//pUserInfo->pList = new stUserInfo;//[pUserInfo->count];
	stRoleInfo roleInfo;
	memset(&userInfo, 0, sizeof(userInfo));
	memset(&roleInfo, 0, sizeof(roleInfo));
	//memset(pUserInfo->pList, 0, sizeof(stUserInfo));
	//stUserInfo userInfo = pUserInfo->user;
	int offset = sizeof(stServerMsg);
	memcpy(&userInfo, (char*)pUserInfo+offset, sizeof(stUserInfo));
	offset += sizeof(stUserInfo);
	userInfo.name = new char[userInfo.nameLen];
	userInfo.pwd = new char[userInfo.pwdLen];
	userInfo.email = new char[userInfo.emailLen];
	userInfo.mobile = new char[userInfo.mobileLen];
	memcpy(userInfo.name, (char*)pUserInfo+offset, userInfo.nameLen);
	offset += userInfo.nameLen;	
	memcpy(userInfo.pwd, (char*)pUserInfo+offset, userInfo.pwdLen);
	offset += userInfo.pwdLen;
	memcpy(userInfo.email, (char*)pUserInfo+offset, userInfo.emailLen);
	offset += userInfo.emailLen;
	memcpy(userInfo.mobile, (char*)pUserInfo+offset, userInfo.mobileLen);
	offset += userInfo.mobileLen;
	//role info
	memcpy(&roleInfo, (char*)pUserInfo+offset, sizeof(stRoleInfo));
	//memcpy(roleInfo.pRoleName);
	
	CUser* pUser = new CUser(userInfo.id);
	uint32_t checkNo = pUser->Login(userInfo, roleInfo);
	
	//发送的GameServer
	/*char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	stL2G_Login* p = (stL2G_Login*)buf;
	p->cSysIdentifer = INMSG_LOGIN_GAME;
	p->cMsgID = L2G_LOGIN;
	int len = sizeof(stServerMsg);
	memcpy(buf+len, &checkNo, len);
	len += sizeof(uint32_t);
	memcpy(buf+len, &roleInfo, sizeof(stRoleInfo));
	len += sizeof(stRoleInfo);
	StartServer->SendToGame(buf, len);*/
	
	//发送给客户端
	char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	int len = 0;
	stS2CMsgLogin* msg = (stS2CMsgLogin*)buf;
	msg->cSysIdentifer = MSG_LOGIN_S2C; 
	msg->cMsgID = eMsg_S2C_Login;
	msg->nErr = 0;
	msg->nUserId = userInfo.id;//checkNo;
	len += sizeof(stS2CMsgLogin);
	StartServer->SendToClient(pUserInfo->guid, buf, len);
	//pHandler->Send( buf, len);
	
	delete []userInfo.mobile;
	delete []userInfo.email;
	delete []userInfo.pwd;
	delete []userInfo.name;
}

void CServerTreator::HandleLoginErr(InternalMsg* pInmsg)
{
	//CHandler* pHandler = pInmsg->pHandler;
	stD2LLoginErr* err = (stD2LLoginErr*)(pInmsg->pMsg);
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgLogin* pMsg = (stS2CMsgLogin*)buf;
	pMsg->cSysIdentifer = MSG_LOGIN_S2C;
	pMsg->cMsgID = eMsg_S2C_Login;
	switch(err->nErr)
	{
		case 1:
		case 2:
		case 3:
		default:
		{
			pMsg->nErr = err->nErr;
			pMsg->nUserId = 0;
		}
		break;
	}
	int len = sizeof(stS2CMsgLogin);
	StartServer->SendToClient(err->guid, buf, len);//pHandler->Send( buf, len);
	//pHandler->GetListenThread()->RemoveHandler(pHandler->GetClientID());
}


	
