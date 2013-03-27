#include "ServerTreator.h"
#include <string.h>
#include <assert.h>
#include "User.h"
#include "../Base/InternalMsg.h"
#include "../Base/Handler.h"
#include "../Base/ListenThread.h"
#include "StartServer.h"
#include "../Base/Define.h"
#include "UserMgr.h"
#include "TopUser.h"

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
				printf("m_semMsg.Wait return EINTR\n");
				continue;
			}
			else
			{
				assert(0);
			}
		}
		while(Get(&pInmsg))
		{
			//printf("Get msg %d : %d\n", pInmsg->pMsg->cSysIdentifer, pInmsg->pMsg->cMsgID);
			if (INMSG_SYS == pInmsg->pMsg->cSysIdentifer)
			{
				stServerMsg* pMsg = (stServerMsg*)pInmsg->pMsg;
				HandleSysMsg(pMsg->cmdType);
				delete pInmsg->pMsg;
				pInmsg->pMsg = NULL;	//InternalMsg.h:73
			}
			else if(INMSG_LOGIN_GAME == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case L2G_LOGIN:
					{
						stL2G_Login* smsg = (stL2G_Login*)pInmsg->pMsg;
						HandleLoginSucess(smsg);
					}
					break;
					default:
					break;
				}
			}
			else if(INMSG_DB_GAME == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case D2G_SYS_INIT:
					{
						HandleSysInit(pInmsg);
					}
					break;
					case D2G_SYS_LOAD_ALL:
					{
						HandleLoadAllUser(pInmsg);
					}
					break;
					case D2G_USER_CKECK:
					{
						//stD2G_Role* pMsg = (stD2G_Role*)pInmsg->pMsg;
						HandleCheckUser(pInmsg);
					}
					break;
					case D2G_GENERAL:
					{
						HandleLoadGeneralInfo(pInmsg);
					}
					break;
					case D2G_EQUIP:
					{
						HandleLoadEquipInfo(pInmsg);
					}
					break;
					case D2G_ITEM:
					{
						HandleLoadItemInfo(pInmsg);
					}
					break;
					case D2G_RELATION:
					{
						HandleLoadRelationInfo(pInmsg);
					}
					break;
					case D2G_SHOP_HISTORY:
					{
						HandleLoadShopHistoryInfo(pInmsg);
					}
					break;
					case  D2G_TOP_INFO:
						{
							HandleLoadTopInfo(pInmsg);
						}
						break;
					case D2G_CHALLENGE_INFO:
						{
							HandleLoadChallengeInfo(pInmsg);
						}
						break;
					case D2G_CHALL_TASK_INFO:
						{
							HandleLoadChallTaskInfo(pInmsg);
						}
						break;
					case D2G_BATTLE_INFO:
						{
							HandleLoadBattleInfo(pInmsg);
						}
						break;
					default:
					break;
				}
				
			}
			//else {assert(0);}
		}
		if(pInmsg) delete pInmsg;
		pInmsg = NULL;
	}
}

void CServerTreator::InitSysData()
{
	stG2DSys msg;
	msg.maxSerialNo = g_GodSerialNoBegin;
	StartServer->SendToDB((char*)(&msg), sizeof(msg));

}

void CServerTreator::LoginUserId(unsigned int userId)
{
	printf("CServerTreator::LoginUserId(%u)\n", userId);
	char select[BUF_SIZE1];
	memset( select,0, sizeof( select ));
	sprintf(select, "select * from role where userId=%u", userId);
	char buf[BUF_SIZE1];
	stG2D_Select* pSel = (stG2D_Select*)buf;
	pSel->cSysIdentifer = INMSG_GAME_DB;
	pSel->cMsgID = G2D_USER_CKECK;
	pSel->cmdType = G2D_USER_CKECK;
	pSel->guid = 0;	//与客户端登录区别之处
	pSel->nSelLen = strlen(select);
	pSel->select = NULL;
	int len=sizeof(stG2D_Select);

	memcpy(buf+len, select, pSel->nSelLen);
	len += pSel->nSelLen;

	StartServer->SendToDB( buf, len);

}

void CServerTreator::HandleLoadAllUser(InternalMsg* pInmsg)
{
	printf("CServerTreator::HandleLoadAllUser\n");
	stD2GLoadAllUser* pMsg = (stD2GLoadAllUser*)pInmsg->pMsg;
	unsigned short n = pMsg->count;
	unsigned int offset = sizeof(stD2GLoadAllUser);
	unsigned int id = 0;
	//char select[BUF_SIZE1];
	for(int i=0; i<n; i++)
	{
		//id = (unsigned int)((char*)pMsg+offset);
		memcpy(&id, (char*)pMsg+offset, sizeof(unsigned int));
		LoginUserId(id);
		offset += sizeof(unsigned int);
		usleep(100);
	}
}

void CServerTreator::HandleSysInit(InternalMsg* pInmsg)
{
	stD2GSys* pMsg = (stD2GSys*)pInmsg->pMsg;
	assert(pMsg);
	if (pMsg->maxSerialNo > g_GodSerialNoBegin)
	{
		g_GodSerialNoBegin = pMsg->maxSerialNo;
	}
	printf("g_GodSerialNoBegin=%u\n", g_GodSerialNoBegin);
	g_UserMgr->CalculateAllRole();
	printf("......Game Server started******\n");
}

void CServerTreator::HandleSysMsg(int subCmd)
{
	switch(subCmd)
	{
	case SUB_SAVE_DATA:
		{
			ServerTreator->SaveTopInfo(); //暂时
			printf("INFO: g_UserMgr->OnSaveData()\n");
			g_UserMgr->OnSaveData();
		}
		break;
	case SUB_TIME1://每日
		{
			g_UserMgr->OnChallTaskUpdate();
		}
		break;
	case SUB_TIME2://每周
		{
			g_TopInfoMgr->OnWeeklyReward();
		}
		break;
	case SUB_RESET_BATTLE_TIME:
		{
			g_UserMgr->OnBattleReset();
		}
		break;
	}
}

void CServerTreator::HandleLoadTopInfo(InternalMsg* pInmsg)
{
	stD2G_TopInfo* pMsg = (stD2G_TopInfo*)pInmsg->pMsg;
	int offset = sizeof(stD2G_TopInfo);
	//
	short count = pMsg->count;
	if(count > 0)
	{
		char* p = (char*)pMsg + offset;
		LoadTopInfo(p, count);
	}
}

void CServerTreator::HandleCheckUser(InternalMsg* pInmsg)
{
	//CHandler* pHandler = pInmsg->pHandler;
	stD2G_Role* pMsg = (stD2G_Role*)pInmsg->pMsg;
	int nRes = pMsg->cmdType;
	if(nRes > 0 && pMsg->guid > 0)
	{//未通过
		char buf[BUF_SIZE1];
		stS2CMsgCheck* pCheck = (stS2CMsgCheck*)(buf);
		pCheck->cSysIdentifer = INMSG_GAME_CLIENT;
		pCheck->cMsgID = eMsg_S2C_Check;
		pCheck->nErr = 1;//1 角色不存在
		int len = sizeof(stS2CMsgCheck);
		StartServer->SendToClient(pMsg->guid, buf, len);
	}
	else
	{
		if(pMsg->guid > 0)
		{
			//通知客户端
			char buf2[BUF_SIZE1];
			stS2CMsgCheck* pSendMsg = (stS2CMsgCheck*)(buf2);
			pSendMsg->cSysIdentifer = INMSG_GAME_CLIENT;
			pSendMsg->cMsgID = eMsg_S2C_Check;
			pSendMsg->nErr = 0;//0 成功
			int len2 = sizeof(stS2CMsgCheck);
			StartServer->SendToClient(pMsg->guid, buf2, len2);
		}
		//角色信息
		t_ROLE_INFO role;
		memset(&role, 0, sizeof(t_ROLE_INFO));
		int offset = sizeof(stD2G_Role);
		//memcpy(&role, &(pMsg->role), offset);
		role.userId = pMsg->role.userId;
		role.exp = pMsg->role.exp;
		role.level = pMsg->role.level;
		role.coin = pMsg->role.coin;
		role.gold = pMsg->role.gold;
		role.vitality = pMsg->role.vitality;
		role.ability = pMsg->role.ability;
		role.psychicPower = pMsg->role.psychicPower;
		role.honor = pMsg->role.honor;
		role.curTask = pMsg->role.curTask;
		role.leftChallengeTimes = pMsg->role.leftChallengeTimes;
		role.wonTimes = pMsg->role.wonTimes;
		role.challengeWonTimes = pMsg->role.challengeWonTimes;
		//role.compDoubleWonTimes = pMsg->role.compDoubleWonTimes;
		role.lastHonorExcTime = pMsg->role.lastHonorExcTime;
		role.lastActiveTime = pMsg->role.lastActiveTime;
		role.oldTopId = pMsg->role.oldTopId;
		role.newTopId = pMsg->role.newTopId;
		memcpy(role.szName, (char*)pMsg+offset, pMsg->role.roleNameLen);
		offset += pMsg->role.roleNameLen;
		CUser* pUser = new CUser(role.userId);
		if(pMsg->guid > 0)
		{
			CHandler* pHandler = StartServer->m_ListenClientThread.GetHandler(pMsg->guid);
			pUser->SetHandler(pHandler);
			pHandler->SetUser(pUser);
		}
		pUser->Login(role); //Login内 发送角色信息给客户端	
		//加载可执行任务列表
		char* p = (char*)pMsg + offset;
		short count;
		memcpy(&count, p, sizeof(count));
		if(count > 0)
		{
			p += sizeof(count);
			pUser->LoadTaskInfo(p, count);
		}
	}
}

void CServerTreator::HandleLoadGeneralInfo(InternalMsg* pInmsg)
{
	stD2G_General* pMsg = (stD2G_General*)pInmsg->pMsg;
	int offset = sizeof(stD2G_General);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	char* p = (char*)pMsg + offset;
	short count = pMsg->count;
	if(count > 0)
	{
		pUser->LoadGeneralInfo(p, count);
	}
	else
	{
		pUser->SendRoleInfo();
	}
}

void CServerTreator::HandleLoadEquipInfo(InternalMsg* pInmsg)
{
	stD2G_Equip* pMsg = (stD2G_Equip*)pInmsg->pMsg;
	int offset = sizeof(stD2G_Equip);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	char* p = (char*)pMsg + offset;
	short count = pMsg->count;
	if(count > 0)
	{
		pUser->LoadEquipInfo(p, count);
	}
}

void CServerTreator::HandleLoadItemInfo(InternalMsg* pInmsg)
{
	stD2G_Item* pMsg = (stD2G_Item*)pInmsg->pMsg;
	int offset = sizeof(stD2G_Item);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	char* p = (char*)pMsg + offset;
	short count = pMsg->count;
	if(count > 0)
	{
		pUser->LoadItemInfo(p, count);
	}
	else
	{
		pUser->OnItemList();
	}
}

void CServerTreator::HandleLoadRelationInfo(InternalMsg* pInmsg)
{
	stD2G_RELATION* pMsg = (stD2G_RELATION*)pInmsg->pMsg;
	int offset = sizeof(stD2G_RELATION);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	char* p = (char*)pMsg + offset;
	short count = pMsg->count;
	if(count > 0)
	{
		pUser->LoadRelationInfo(p, count);
	}
}

void CServerTreator::HandleLoadShopHistoryInfo(InternalMsg* pInmsg)
{
	stD2G_ShopHistory* pMsg = (stD2G_ShopHistory*)pInmsg->pMsg;
	int offset = sizeof(stD2G_ShopHistory);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	short count = pMsg->count;
	if(count > 0)
	{
		char* p = (char*)pMsg + offset;
		pUser->LoadShopHistoryInfo(p, count);
	}
}

void CServerTreator::HandleLoadChallengeInfo(InternalMsg* pInmsg)
{
	stD2G_ShopHistory* pMsg = (stD2G_ShopHistory*)pInmsg->pMsg;
	int offset = sizeof(stD2G_ShopHistory);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	short count = pMsg->count;
	if(count > 0)
	{
		char* p = (char*)pMsg + offset;
		pUser->LoadChallengeInfo(p, count);
	}
}

void CServerTreator::HandleLoadChallTaskInfo(InternalMsg* pInmsg)
{
	stD2G_ChallTask* pMsg = (stD2G_ChallTask*)pInmsg->pMsg;
	int offset = sizeof(stD2G_ChallTask);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	short count = pMsg->count;
	if(count > 0)
	{
		char* p = (char*)pMsg + offset;
		pUser->LoadChallTaskInfo(p, count);
	}
}

void CServerTreator::HandleLoadBattleInfo(InternalMsg* pInmsg)
{
	stD2G_Battle* pMsg = (stD2G_Battle*)pInmsg->pMsg;
	int offset = sizeof(stD2G_Battle);
	CUser* pUser = g_UserMgr->GetUser(pMsg->userId);
	if(!pUser)
	{
		printf("user 不存在\n");
		return;
	}
	//
	short count = pMsg->count;
	if(count > 0)
	{
		char* p = (char*)pMsg + offset;
		pUser->LoadBattleInfo(p, count);
	}
}

void CServerTreator::HandleLoginSucess(stL2G_Login* pMsg)
{
	uint32_t checkNo = pMsg->checkNo;
	stRoleInfo role;
	memset(&role, 0, sizeof(stRoleInfo));
	memcpy(&role, &(pMsg->role), sizeof(stRoleInfo));
	
	CUser* pUser = new CUser(role.userId);
	pUser->Login(checkNo, role);
}

void CServerTreator::LoadTopInfo(char* buf, short count)
{
	printf("CServerTreator::LoadTopInfo");
	assert(buf && count > 0);
	char* p = buf;
	stTopInfo* p2 = (stTopInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stTopInfo*)p;

		CTopUser* pObj = new CTopUser(p2);
		g_TopInfoMgr->Add(pObj);
		//int sz = g_TopInfoMgr->Add(pObj);
		//printf("----%d\n", sz);
		p += sizeof(stTopInfo);
	}
}

void CServerTreator::SaveTopInfo()
{
	printf("CServerTreator::SaveTopInfo\n");
	char buf[BUF_SIZE2];
	stG2D_SaveTopInfo* pMsg = (stG2D_SaveTopInfo*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_TOP_INFO;
	pMsg->cmdType = G2D_SAVE_TOP_INFO;
	pMsg->pList = NULL;
	int len = sizeof(stG2D_SaveTopInfo);
	pMsg->count = g_TopInfoMgr->CopyTopInfo(buf+len, BUF_SIZE2-len);
	len += sizeof(stTopInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

	
