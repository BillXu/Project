#include "DBTreator.h"
#include <string.h>
#include <assert.h>
#include "RecordStructDefine.h"
#include "StartServer.h"

void Test()
{
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	strcpy(sql, "name='abcd'");
	//pHandler->ExecSql(sql, strlen(sql));
	char bufUser[sizeof(t_UserRecord)];
	t_UserRecord* pData = (t_UserRecord*)bufUser;
	//bool bRet = pHandler->SelectDataByColNext(User_Define, (uint8_t*)pData);
	
	if(pHandler->SelectDataByColIT("user", User_Define, sql, NULL))
	{
		if(!pHandler->SelectDataByColNext(User_Define, (uint8_t *)pData))
		{
			MysqlPool->PutHandler(pHandler);
		}
	}
	else
	{
		MysqlPool->PutHandler(pHandler);
	}
	pHandler->ResetResult();
	
}
CDBTreator::CDBTreator()
{
	m_bRun = true;
}
CDBTreator::~CDBTreator()
{
}

void CDBTreator::__run()
{
	InternalMsg* pInmsg;
	while(m_bRun)
	{
		//Test();
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
			//CHandler* pHandler = pInmsg->pHandler;
			if(INMSG_LOGIN_DB == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case L2D_LOGIN:
					{
						stL2DLogin* pMsg = (stL2DLogin*)(pInmsg->pMsg);
						pMsg->select = new char[pMsg->nSelLen];
						memset(pMsg->select, 0, pMsg->nSelLen);
						int offset = sizeof(stL2DLogin);
						memcpy(pMsg->select, (char*)pMsg+offset, pMsg->nSelLen);
						HandleLogin(pMsg);
						delete [](pMsg->select);
						
						//CUser* user = new CUser;
						//user->FillUserInfo(pData);
					}
					break;
					default:
					break;
				}
			}
			else if(INMSG_GAME_DB == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case G2D_SYS_INIT:
					{
						stG2DSys* pMsg = (stG2DSys*)(pInmsg->pMsg);
						OnSysInit(pMsg);
					}
					case G2D_USER_CKECK:
					{
						stG2D_Select* pMsg = (stG2D_Select*)(pInmsg->pMsg);
						pMsg->select = new char[pMsg->nSelLen];
						memset(pMsg->select, 0, pMsg->nSelLen);
						int offset = sizeof(stG2D_Select);
						memcpy(pMsg->select, (char*)pMsg+offset, pMsg->nSelLen);
						OnSelect(pMsg);
					}
					break;
					case G2D_SAVE_ROLE_BASE:
					{
						stG2D_SaveRoleBase* pMsg = (stG2D_SaveRoleBase*)(pInmsg->pMsg);
						OnSaveRoleBaseInfo(pMsg);
					}
					break;
					case G2D_SAVE_ROLE:
					{
						stG2D_SaveRole* pMsg = (stG2D_SaveRole*)(pInmsg->pMsg);
						OnSaveRoleInfo(pMsg);
						
					}
					break;
					case G2D_SAVE_GENERAL:
					{
						stG2D_SaveGeneral* pMsg = (stG2D_SaveGeneral*)(pInmsg->pMsg);
						OnSaveGeneralInfo(pMsg);
					}
					break;
					case G2D_SAVE_EQUIP:
					{
						stG2D_SaveEquip* pMsg = (stG2D_SaveEquip*)(pInmsg->pMsg);
						OnSaveEquipInfo(pMsg);
					}
					break;
					case G2D_SAVE_ITEM:
					{
						stG2D_SaveItem* pMsg = (stG2D_SaveItem*)(pInmsg->pMsg);
						OnSaveItemInfo(pMsg);
					}
					break;
					case G2D_SAVE_RELATION:
					{
						stG2D_SaveRelation* pMsg = (stG2D_SaveRelation*)(pInmsg->pMsg);
						OnSaveRelationInfo(pMsg);
					}
					break;
					case G2D_SAVE_SHOP_HISTORY:
						{
							stG2D_SaveShopHistory* pMsg = (stG2D_SaveShopHistory*)(pInmsg->pMsg);
							OnSaveShopHistory(pMsg);
						}
						break;
					case G2D_SAVE_CHALL_TASK_INFO:
						{
							stG2D_SaveChallTask* pMsg = (stG2D_SaveChallTask*)(pInmsg->pMsg);
							OnSaveChallTask(pMsg);
						}
						break;
					case G2D_SAVE_TOP_INFO:
						{
							stG2D_SaveTopInfo* pMsg = (stG2D_SaveTopInfo*)(pInmsg->pMsg);
							OnSaveTopInfo(pMsg);
						}
						break;
					case G2D_SAVE_BATTLE_INFO:
						{
							stG2D_SaveBattle* pMsg = (stG2D_SaveBattle*)(pInmsg->pMsg);
							OnSaveBattleInfo(pMsg);
						}
						break;
					default:
					break;
				}
			}
			else
			{
				printf("err: pInmsg->pMsg->cSysIdentifer=%d\n", pInmsg->pMsg->cSysIdentifer);
				assert(0);
			}
		}
		if(pInmsg) delete pInmsg;
		pInmsg = NULL;
	}
}

void CDBTreator::OnSysInit(stG2DSys* pMsg)
{
	printf("******CDBTreator::OnSysInit......\n");
	char sql[BUF_SIZE1];
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);

	//Load user list
	/*char buf[BUF_SIZE_1M];
	memset(buf, 0, sizeof(buf));
	stD2GLoadAllUser* pSendMsg = (stD2GLoadAllUser*)buf;
	pSendMsg->cSysIdentifer = INMSG_DB_GAME;
	pSendMsg->cMsgID = D2G_SYS_LOAD_ALL;
	pSendMsg->pList = NULL;*/
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "select `userId` from `role` where `userId`>=1000 limit 20000");
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();
	t_UserIdRecord record2;
	unsigned short n=0;
	//int len = sizeof(stD2GLoadAllUser);
	while(pHandler->SelectDataByColNext(UserId_Define, (uint8_t*)(&record2)))
	{
		//memcpy(buf+len, &(record2.userId), sizeof(unsigned int));
		//len += sizeof(unsigned int);
		n++;
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "select * from role where userId=%u", record2.userId);
		Login(sql, strlen(sql), 0);
		usleep(100);
		//
	}
	//pSendMsg->count = n;
	pHandler->ResetResult();
	//StartServer->SendToGame( buf, len);

	LoadTop50Info();
	//
	stD2GSys msg;
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "call get_max_serialNo();");
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();
	t_SysInitRecord record;
	bool bRet = pHandler->SelectDataByColNext(SysInit_Define, (uint8_t*)(&record));
	pHandler->ResetResult();
	if (bRet)
	{
		msg.maxSerialNo = record.maxSerialNo;
	} 
	else
	{
		msg.maxSerialNo = 0;
	}
	printf("*********msg.maxSerialNo=%u***\n", msg.maxSerialNo);
	StartServer->SendToGame( (char*)(&msg), sizeof(msg));

	MysqlPool->PutHandler(pHandler);
}

int CDBTreator::OnSaveRoleBaseInfo(stG2D_SaveRoleBase* pMsg)
{
	//role info
	stRoleInfo* pRole = (stRoleInfo*)(&(pMsg->role));
	int offset = sizeof(stG2D_SaveRoleBase);
	char name[MAX_NAME_SIZE];
	memset(name, 0, sizeof(name));
	memcpy(name, (char*)pMsg+offset, pRole->roleNameLen);
	//offset += pRole->roleNameLen;
	char sql[BUF_SIZE1];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "update `role` set exp=%u,level=%u,coin=%u,gold=%u,vitality=%u,ability=%u,psychicPower=%u,honor=%u,curTask=%u,leftChallengeTimes=%u,wonTimes=%u,challengeWonTimes=%u,lastHonorExcTime=%u,lastActiveTime=%u,oldTopId=%u,newTopId=%u where userId=%u",\
		pRole->exp,pRole->level,pRole->coin,pRole->gold,pRole->vitality,pRole->ability,pRole->psychicPower,pRole->honor,pRole->curTask,\
		pRole->leftChallengeTimes,pRole->wonTimes,pRole->challengeWonTimes,pRole->lastHonorExcTime,pRole->lastActiveTime,pRole->oldTopId,pRole->newTopId,pRole->userId);

	//printf("CDBTreator::OnSaveRoleBaseInfo:\n**** %s ****\n",sql);
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	pHandler->ExecSql(sql, strlen(sql));
	MysqlPool->PutHandler(pHandler);
	
	return 0;
}

int CDBTreator::OnSaveRoleInfo(stG2D_SaveRole* pMsg)
{
	//role info
	stRoleInfo* pRole = (stRoleInfo*)(&(pMsg->role));
	int offset = sizeof(stG2D_SaveRole);//sizeof(stRoleInfo);
	char name[MAX_NAME_SIZE];
	memset(name, 0, sizeof(name));
	memcpy(name, (char*)pMsg+offset, pRole->roleNameLen);
	offset += pRole->roleNameLen;
	char sql[BUF_SIZE1];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "update `role` set exp=%u,level=%u,coin=%u,gold=%u,vitality=%u,ability=%u,psychicPower=%u,honor=%u,curTask=%u,leftChallengeTimes=%u,wonTimes=%u,challengeWonTimes=%u,lastHonorExcTime=%u,lastActiveTime=%u,oldTopId=%u,newTopId=%u where userId=%u",\
					pRole->exp,pRole->level,pRole->coin,pRole->gold,pRole->vitality,pRole->ability,pRole->psychicPower,pRole->honor,pRole->curTask,\
					pRole->leftChallengeTimes,pRole->wonTimes,pRole->challengeWonTimes,pRole->lastHonorExcTime,pRole->lastActiveTime,pRole->oldTopId,pRole->newTopId,pRole->userId);
	
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	pHandler->ExecSql(sql, strlen(sql));
	
	//task info
	
	short n = pMsg->taskCount;
	//offset += sizeof(n);
	char* p = (char*)pMsg+offset;
	stTaskInfo* pTask = NULL;
	while(n > 0)
	{
		pTask = (stTaskInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pTask->changeFlag)
		{
			sprintf(sql, "insert into `task` values(%u,%u,%u,%u)",pTask->userId,pTask->mainId,pTask->leafId,pTask->leftTimes);
		}
		else if (2 == pTask->changeFlag)
		{
			sprintf(sql, "delete from `task` where userId=%u and mainId=%u and leafId=%u",pTask->userId,pTask->mainId,pTask->leafId);	
		}
		else if (3 == pTask->changeFlag)
		{
			sprintf(sql, "update `task` set leftTimes=%u where userId=%u and mainId=%u and leafId=%u",\
						pTask->leftTimes, pTask->userId,pTask->mainId,pTask->leafId);
		}
		else
		{
			assert(0);
		}

		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stTaskInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveGeneralInfo(stG2D_SaveGeneral* pMsg)
{
	int offset = sizeof(stG2D_SaveGeneral);

	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stGeneralInfo* pG = NULL;
	while(n > 0)
	{
		pG = (stGeneralInfo*)p;
		memset(sql, 0, sizeof(sql));
		//(`roleId`,`itemId`,`guid`,`warFlag`,`group`,`curStar`,`curLevel`,`godSkill`) 
		if(1 == pG->changeFlag)
		{
			sprintf(sql, "update `general` set warFlag=%u,curStar=%u,curLevel=%u,godSkill=%u where serialNo=%u",\
						pG->warFlag,pG->curStar,pG->curLevel,pG->godSkill,pG->serialNo);
		}
		else if (2 == pG->changeFlag)
		{
			sprintf(sql, "delete from `general` where serialNo=%u", pG->serialNo);	
		}
		else if (3 == pG->changeFlag)
		{
			sprintf(sql, "insert into `general` values(%u,%u,%u,%u,%u,%u,%u,%u,%u)",\
				pG->serialNo,pG->roleId,pG->itemId,pG->guid,pG->warFlag,pG->group,pG->curStar,\
				pG->curLevel,pG->godSkill);
		}
		else
		{
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stGeneralInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveEquipInfo(stG2D_SaveEquip* pMsg)
{
	int offset = sizeof(stG2D_SaveEquip);

	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stEquipInfo* pItem = NULL;
	while(n > 0)
	{
		pItem = (stEquipInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pItem->changeFlag)
		{
			sprintf(sql, "update `equip` set warFlag=%u,curStar=%u,curLevel=%u where serialNo=%u",\
				pItem->warFlag,pItem->curStar,pItem->curLevel,pItem->serialNo);
		}
		else if (2 == pItem->changeFlag)
		{
			sprintf(sql, "delete from `equip` where serialNo=%u", pItem->serialNo);	
		}
		else if (3 == pItem->changeFlag)
		{
			sprintf(sql, "insert into `equip` values(%u,%u,%u,%u,%u,%u,%u,%u)",\
				pItem->serialNo,pItem->type,pItem->roleId,pItem->guid,pItem->itemId,pItem->warFlag,pItem->curStar,pItem->curLevel);
		}
		else
		{
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stEquipInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveItemInfo(stG2D_SaveItem* pMsg)
{
	int offset = sizeof(stG2D_SaveItem);

	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stItemInfo* pItem = NULL;
	while(n > 0)
	{
		pItem = (stItemInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pItem->changeFlag)
		{
			sprintf(sql, "update `item` set count=%u where roleId=%u and itemId=%u",\
				pItem->count,pItem->roleId,pItem->itemId);
		}
		else if (2 == pItem->changeFlag)
		{
			sprintf(sql, "delete from `item` where roleId=%u and itemId=%u", pItem->roleId,pItem->itemId);	
		}
		else if (3 == pItem->changeFlag)
		{
			sprintf(sql, "insert into `item` values(%u,%u,%u,%u)",\
				pItem->roleId,pItem->type,pItem->itemId,pItem->count);
		}
		else
		{
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stItemInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveRelationInfo(stG2D_SaveRelation* pMsg)
{
	int offset = sizeof(stG2D_SaveRelation);

	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stRelationInfo* pObj = NULL;
	unsigned int userId = pMsg->userId;
	while(n > 0)
	{
		pObj = (stRelationInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pObj->changeFlag)
		{
			sprintf(sql, "update `relation` set roleId2=%u,type=%u,addTime=%u where roleId1=%u",\
				pObj->roleId,pObj->type,pObj->addTime,userId);
		}
		else if (2 == pObj->changeFlag)
		{
			sprintf(sql, "delete from `relation` where roleId1=%u", userId);	
		}
		else if (3 == pObj->changeFlag)
		{
			sprintf(sql, "insert into `relation` values(%u,%u,%u,%u)",\
				userId,pObj->roleId,pObj->type,pObj->addTime);
		}
		else
		{
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stRelationInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveShopHistory(stG2D_SaveShopHistory* pMsg)
{
	int offset = sizeof(stG2D_SaveShopHistory);
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_WRITE);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stShopHistoryInfo* pObj = NULL;
	unsigned int userId = pMsg->userId;
	while(n > 0)
	{
		pObj = (stShopHistoryInfo*)p;
		memset(sql, 0, sizeof(sql));
		unsigned int curTime = getSecTime();
		if(1 == pObj->changeFlag)
		{
			sprintf(sql, "update `shopHistory` set count=%u,price=%u,lastTime=%u where roleId=%u",\
				pObj->count,pObj->price,curTime,userId);
		}
		else if (2 == pObj->changeFlag)
		{
			sprintf(sql, "delete from `shopHistory` where roleId=%u", userId);	
		}
		else if (3 == pObj->changeFlag)
		{
			sprintf(sql, "insert into `shopHistory` values(%u,%u,%u,%u,%u,%u)",\
				userId,pObj->guid,pObj->count,pObj->priceType,pObj->price,curTime);
		}
		else
		{
			printf("Err: CDBTreator::OnSaveShopHistory changeFlag=%u!!!!!!!!!\n", pObj->changeFlag);
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stShopHistoryInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveChallTask(stG2D_SaveChallTask* pMsg)
{
	int offset = sizeof(stG2D_SaveChallTask);
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_WRITE);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stChallTaskInfo* pObj = NULL;
	unsigned int userId = pMsg->userId;
	while(n > 0)
	{
		pObj = (stChallTaskInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pObj->changeFlag)
		{
			sprintf(sql, "update `challTask` set roleId2=%u,flag=%u where roleId=%u and subTaskId=%u",\
				pObj->roleId2,pObj->flag,userId,pObj->subTaskId);
		}
		else if (2 == pObj->changeFlag)
		{
			sprintf(sql, "delete from `challTask` where roleId=%u", userId);	
		}
		else if (3 == pObj->changeFlag)
		{
			sprintf(sql, "insert into `challTask` values(%u,%u,%u,%u)",\
				userId,pObj->subTaskId,pObj->roleId2,pObj->flag);
		}
		else
		{
			printf("Err: CDBTreator::OnSaveChallTask changeFlag=%u!!!!!!!!!\n", pObj->changeFlag);
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stChallTaskInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveTopInfo(stG2D_SaveTopInfo* pMsg)
{
	int offset = sizeof(stG2D_SaveTopInfo);
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_WRITE);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stTopInfo* pObj = NULL;
	while(n > 0)
	{
		pObj = (stTopInfo*)p;
		memset(sql, 0, sizeof(sql));
		unsigned int curTime = getSecTime();
		if(1 == pObj->changeFlag)
		{//改变 top50表只要改
			sprintf(sql, "update `top50` set roleId=%u,lastTime=%u where topId=%u",\
				pObj->roleId,curTime,pObj->topId);
		}
		else
		{
			assert(0);
		}
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stTopInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSaveBattleInfo(stG2D_SaveBattle* pMsg)
{
	//printf("CDBTreator::OnSaveBattleInfo\n");
	int offset = sizeof(stG2D_SaveBattle);
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_WRITE);

	short n = pMsg->count;
	char sql[BUF_SIZE1];
	char* p = (char*)pMsg+offset;
	stBattleInfo* pObj = NULL;
	unsigned int userId = pMsg->userId;
	while(n > 0)
	{
		pObj = (stBattleInfo*)p;
		memset(sql, 0, sizeof(sql));
		if(1 == pObj->changeFlag)
		{
			sprintf(sql, "update `copy` set curLeafId=%u where roleId=%u and mainId=%u",\
				pObj->curLeafId,userId,pObj->mainId);
		}
		else if (2 == pObj->changeFlag)
		{
			sprintf(sql, "delete from `copy` where roleId=%u", userId);	
		}
		else if (3 == pObj->changeFlag)
		{
			sprintf(sql, "insert into `copy` values(%u,%u,%u)",\
				userId,pObj->mainId,pObj->curLeafId);
		}
		else
		{
			printf("Err: CDBTreator::OnSaveBattleInfo changeFlag=%u!!!!!!!!!\n", pObj->changeFlag);
			assert(0);
		}
		//printf("sql: %s \n", sql);
		pHandler->ExecSql(sql, strlen(sql));
		p += sizeof(stBattleInfo);
		--n;
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

int CDBTreator::OnSelect(stG2D_Select* pMsg)
{
	assert(pMsg);
	switch(pMsg->cmdType)
	{
		case G2D_USER_CKECK:
		{
			CheckUserId(pMsg);
		}
		break;
		default:
		break;
	}
	return 0;
}

void CDBTreator::Login(char* sql, short sqlLen, uint64_t clientGuid)
{
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	pHandler->ExecSql(sql, sqlLen);
	pHandler->UseResult();
	char bufRole[sizeof(t_RoleRecord)];
	t_RoleRecord* pRoleInfo = (t_RoleRecord*)bufRole;
	bool bRet = pHandler->SelectDataByColNext(Role_Define, (uint8_t*)pRoleInfo);
	pHandler->ResetResult();
	if(bRet)
	{
		char buf[BUF_SIZE1*10];//task记录最多5k
		stD2G_Role* pRole = (stD2G_Role*)(buf);
		pRole->cSysIdentifer = INMSG_DB_GAME;
		pRole->cMsgID = D2G_USER_CKECK;
		pRole->cmdType = 0;//验证通过
		pRole->guid = clientGuid;
		pRole->role.userId = pRoleInfo->userId;
		pRole->role.roleNameLen = strlen(pRoleInfo->roleName);
		pRole->role.exp = pRoleInfo->exp;
		pRole->role.level = pRoleInfo->level;
		pRole->role.coin = pRoleInfo->coin;
		pRole->role.gold = pRoleInfo->gold;
		pRole->role.vitality = pRoleInfo->vitality;
		pRole->role.ability = pRoleInfo->ability;
		pRole->role.psychicPower = pRoleInfo->psychicPower;
		pRole->role.honor = pRoleInfo->honor;
		pRole->role.curTask = pRoleInfo->curTask;
		pRole->role.leftChallengeTimes = pRoleInfo->leftChallengeTimes;
		pRole->role.oldTopId = pRoleInfo->oldTopId;
		pRole->role.newTopId = pRoleInfo->newTopId;
		//pRole->role.competitionTimes = pRoleInfo->competitionTimes;
		pRole->role.wonTimes = pRoleInfo->wonTimes;
		pRole->role.challengeWonTimes = pRoleInfo->challengeWonTimes;
		//pRole->role.compDoubleWonTimes = pRoleInfo->compDoubleWonTimes;
		//pRole->role.attackTimes = pRoleInfo->attackTimes;
		//pRole->role.defenceTimes = pRoleInfo->defenceTimes;
		//pRole->role.attackWonTimes = pRoleInfo->attackWonTimes;
		//pRole->role.defenceWonTimes = pRoleInfo->defenceWonTimes;

		int len = sizeof(stD2G_Role);
		memcpy(buf+len, pRoleInfo->roleName, pRole->role.roleNameLen);
		len += pRole->role.roleNameLen;
		//取任务信息
		char sql2[BUF_SIZE1];
		memset(sql2, 0, sizeof(BUF_SIZE1));
		sprintf(sql2, "select * from task where userId=%u and leftTimes>0", pRoleInfo->userId);
		pHandler->ExecSql(sql2, strlen(sql2));
		pHandler->UseResult();
		char bufTask[sizeof(t_TaskRecord)];
		t_TaskRecord* pTaskInfo = (t_TaskRecord*)bufTask;
		short count = 0;
		int countOffset = len;
		len += sizeof(count);
		stTaskInfo* pTask = NULL;
		while(pHandler->SelectDataByColNext(Task_Define, (uint8_t*)pTaskInfo))
		{
			pTask = (stTaskInfo*)(buf+len);
			pTask->userId = pTaskInfo->userId;
			pTask->mainId = pTaskInfo->mainId;
			pTask->leafId = pTaskInfo->leafId;
			pTask->leftTimes = pTaskInfo->leftTimes;
			count++;
			len += sizeof(stTaskInfo);
		}
		memcpy(buf+countOffset, &count, sizeof(count));
		pHandler->ResetResult();
		//通过uid验证并发送角色信息给Game
		StartServer->SendToGame( buf, len);
		LoadUserInfo(pRoleInfo->userId);
	}
	else
	{
		char buf[BUF_SIZE1];
		stServerMsg* pServerMsg = (stServerMsg*)(buf);
		pServerMsg->cSysIdentifer = INMSG_DB_GAME;
		pServerMsg->cMsgID = D2G_USER_CKECK;
		pServerMsg->cmdType = 1;//未通过验证
		pServerMsg->guid = clientGuid;
		int len = sizeof(stServerMsg);
		//通过uid验证并发送角色信息给Game
		StartServer->SendToGame( buf, len);
	}
	MysqlPool->PutHandler(pHandler);

}

void CDBTreator::CheckUserId(stG2D_Select* pMsg)
{
	Login(pMsg->select, pMsg->nSelLen, pMsg->guid);
}
	//printf("CDBTreator::CheckUserId\n");
	/*CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	memcpy(sql, pMsg->select, pMsg->nSelLen);
	pHandler->ExecSql(sql, pMsg->nSelLen);
	pHandler->UseResult();
	char bufRole[sizeof(t_RoleRecord)];
	t_RoleRecord* pRoleInfo = (t_RoleRecord*)bufRole;
	bool bRet = pHandler->SelectDataByColNext(Role_Define, (uint8_t*)pRoleInfo);
	pHandler->ResetResult();
	if(bRet)
	{
		char buf[BUF_SIZE1*10];//task记录最多5k
		stD2G_Role* pRole = (stD2G_Role*)(buf);
		pRole->cSysIdentifer = INMSG_DB_GAME;
		pRole->cMsgID = D2G_USER_CKECK;
		pRole->cmdType = 0;//验证通过
		pRole->guid = pMsg->guid;
		pRole->role.userId = pRoleInfo->userId;
		pRole->role.roleNameLen = strlen(pRoleInfo->roleName);
		pRole->role.exp = pRoleInfo->exp;
		pRole->role.level = pRoleInfo->level;
		pRole->role.coin = pRoleInfo->coin;
		pRole->role.gold = pRoleInfo->gold;
		pRole->role.vitality = pRoleInfo->vitality;
		pRole->role.ability = pRoleInfo->ability;
		pRole->role.psychicPower = pRoleInfo->psychicPower;
		pRole->role.honor = pRoleInfo->honor;
		pRole->role.curTask = pRoleInfo->curTask;
		pRole->role.leftChallengeTimes = pRoleInfo->leftChallengeTimes;
		pRole->role.oldTopId = pRoleInfo->oldTopId;
		pRole->role.newTopId = pRoleInfo->newTopId;
		//pRole->role.competitionTimes = pRoleInfo->competitionTimes;
		pRole->role.wonTimes = pRoleInfo->wonTimes;
		pRole->role.challengeWonTimes = pRoleInfo->challengeWonTimes;
		//pRole->role.compDoubleWonTimes = pRoleInfo->compDoubleWonTimes;
		//pRole->role.attackTimes = pRoleInfo->attackTimes;
		//pRole->role.defenceTimes = pRoleInfo->defenceTimes;
		//pRole->role.attackWonTimes = pRoleInfo->attackWonTimes;
		//pRole->role.defenceWonTimes = pRoleInfo->defenceWonTimes;
		
		int len = sizeof(stD2G_Role);
		memcpy(buf+len, pRoleInfo->roleName, pRole->role.roleNameLen);
		len += pRole->role.roleNameLen;
		//取任务信息
		char sql2[BUF_SIZE1];
		memset(sql2, 0, sizeof(BUF_SIZE1));
		sprintf(sql2, "select * from task where userId=%u and leftTimes>0", pRoleInfo->userId);
		pHandler->ExecSql(sql2, strlen(sql2));
		pHandler->UseResult();
		char bufTask[sizeof(t_TaskRecord)];
		t_TaskRecord* pTaskInfo = (t_TaskRecord*)bufTask;
		short count = 0;
		int countOffset = len;
		len += sizeof(count);
		stTaskInfo* pTask = NULL;
		while(pHandler->SelectDataByColNext(Task_Define, (uint8_t*)pTaskInfo))
		{
			pTask = (stTaskInfo*)(buf+len);
			pTask->userId = pTaskInfo->userId;
			pTask->mainId = pTaskInfo->mainId;
			pTask->leafId = pTaskInfo->leafId;
			pTask->leftTimes = pTaskInfo->leftTimes;
			count++;
			len += sizeof(stTaskInfo);
		}
		memcpy(buf+countOffset, &count, sizeof(count));
		pHandler->ResetResult();
		//通过uid验证并发送角色信息给Game
		StartServer->SendToGame( buf, len);
		LoadUserInfo(pRoleInfo->userId);
	}
	else
	{
		char buf[BUF_SIZE1];
		stServerMsg* pServerMsg = (stServerMsg*)(buf);
		pServerMsg->cSysIdentifer = INMSG_DB_GAME;
		pServerMsg->cMsgID = D2G_USER_CKECK;
		pServerMsg->cmdType = 1;//未通过验证
		pServerMsg->guid = pMsg->guid;
		int len = sizeof(stServerMsg);
		//通过uid验证并发送角色信息给Game
		StartServer->SendToGame( buf, len);
	}
	MysqlPool->PutHandler(pHandler);*/
//}

int CDBTreator::HandleLogin(stL2DLogin* pMsg)
{
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	memcpy(sql, pMsg->select, pMsg->nSelLen);
	//strcpy(sql, "select * from user where name='abcd'");
	pHandler->ExecSql(sql, pMsg->nSelLen);
	pHandler->UseResult();
	char bufUser[sizeof(t_UserRecord)];
	t_UserRecord* pData = (t_UserRecord*)bufUser;
	bool bRet = pHandler->SelectDataByColNext(User_Define, (uint8_t*)pData);
	pHandler->ResetResult();
	if(!bRet)
	{
		//1 帐号或密码错误
		char buf[BUF_SIZE1];
		memset(buf, 0, BUF_SIZE1);
		stD2LLoginErr* err = (stD2LLoginErr*)buf;
		err->cSysIdentifer = INMSG_DB_LOGIN;
		err->cMsgID = D2L_LOGIN;
		err->cmdType = 1;
		err->guid = pMsg->guid;
		err->nErr = 1;
		int len =sizeof(stD2LLoginErr);
		StartServer->SendToLogin( buf, len);
	}
	else
	{
		if(pData->onlineFlag)
		{
			char buf[BUF_SIZE1];
			memset(buf, 0, BUF_SIZE1);
			stD2LLoginErr* err = (stD2LLoginErr*)buf;
			err->cSysIdentifer = INMSG_DB_LOGIN;
			err->cMsgID = D2L_LOGIN;
			err->cmdType = 2;
			err->guid = pMsg->guid;
			err->nErr = 2;
			int len =sizeof(stD2LLoginErr);
			StartServer->SendToLogin( buf, len);
		}
		else
		{
			//打包
			char buf[BUF_SIZE2];
			memset(buf, 0, BUF_SIZE2);
			stD2LUser* pUser = (stD2LUser*)buf;
			pUser->cSysIdentifer = INMSG_DB_LOGIN;
			pUser->cMsgID = D2L_LOGIN;
			pUser->cmdType = 0;
			pUser->guid = pMsg->guid;
			//pUser->count = 1;
			//pUser->pList = NULL;
			int len =sizeof(stServerMsg);
			//user info
			stUserInfo* pInfo = (stUserInfo*)(buf+len);
			pInfo->id = pData->id;
			pInfo->pwdLen = strlen(pData->password);
			pInfo->nameLen = strlen(pData->name);
			pInfo->onlineFlag = pData->onlineFlag;
			pInfo->emailLen = strlen(pData->email);
			pInfo->mobileLen = strlen(pData->mobile);
			len += sizeof(stUserInfo);
			memcpy(buf+len, pData->name, pInfo->nameLen);
			len += pInfo->nameLen;
			memcpy(buf+len, pData->password, pInfo->pwdLen);
			len += pInfo->pwdLen;
			memcpy(buf+len, pData->email, pInfo->emailLen);
			len += pInfo->emailLen;
			memcpy(buf+len, pData->mobile, pInfo->mobileLen);
			len += pInfo->mobileLen;
			//取roleInfo
			char tmpSql[BUF_SIZE1];
			memset(tmpSql, 0, BUF_SIZE1);
			sprintf(tmpSql, "select * from role where userId=%u", pInfo->id);
			pHandler->ExecSql(tmpSql, strlen(tmpSql));
			pHandler->UseResult();
			char bufUser[sizeof(t_RoleRecord)];
			t_RoleRecord* pRoleInfo = (t_RoleRecord*)bufUser;
			bool bRet = pHandler->SelectDataByColNext(Role_Define, (uint8_t*)pRoleInfo);
			pHandler->ResetResult();
			if(!bRet)
			{
				//3 其他错误(没创建角色)
				char tmp[BUF_SIZE1];
				memset(tmp, 0, BUF_SIZE1);
				stD2LLoginErr* err = (stD2LLoginErr*)tmp;
				err->cSysIdentifer = INMSG_DB_LOGIN;
				err->cMsgID = D2L_LOGIN;
				err->cmdType = 3;
				err->guid = pMsg->guid;
				err->nErr = 3;
				int len =sizeof(stD2LLoginErr);
				StartServer->SendToLogin( buf, len);
			}
			else
			{
				//role info
				stRoleInfo* pRole = (stRoleInfo*)(buf+len);
				pRole->userId = pRoleInfo->userId;
				pRole->exp = pRoleInfo->exp;
				pRole->level = pRoleInfo->level;
				pRole->coin = pRoleInfo->coin;
				pRole->gold = pRoleInfo->gold;
				pRole->vitality = pRoleInfo->vitality;
				pRole->ability = pRoleInfo->ability;
				pRole->psychicPower = pRoleInfo->psychicPower;
				pRole->honor = pRoleInfo->honor;
				pRole->curTask = pRoleInfo->curTask;
				pRole->leftChallengeTimes = pRoleInfo->leftChallengeTimes;
				pRole->wonTimes = pRoleInfo->wonTimes;
				pRole->challengeWonTimes = pRoleInfo->challengeWonTimes;
				//pRole->compDoubleWonTimes = pRoleInfo->compDoubleWonTimes;
				pRole->lastHonorExcTime = pRoleInfo->lastHonorExcTime;
				pRole->lastActiveTime = pRoleInfo->lastActiveTime;
				pRole->oldTopId = pRoleInfo->oldTopId;
				pRole->newTopId = pRoleInfo->newTopId;
				pRole->roleNameLen = strlen(pRoleInfo->roleName);
				//pRole->attackTimes = pRoleInfo->attackTimes;
				//pRole->defenceTimes = pRoleInfo->defenceTimes;
				//pRole->attackWonTimes = pRoleInfo->attackWonTimes;
				//pRole->defenceWonTimes = pRoleInfo->defenceWonTimes;
				len += sizeof(stRoleInfo);
				memcpy(buf+len, pRoleInfo->roleName, pRole->roleNameLen);
				len += pRole->roleNameLen;
				//通过登录验证并发送玩家、角色信息给Login
				StartServer->SendToLogin( buf, len);
			}
		}
	}
	MysqlPool->PutHandler(pHandler);
	return 0;
}

void CDBTreator::LoadGeneralInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadGeneralInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from general where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();
	
	char buf[BUF_SIZE2*5];
	stD2G_General* pInfo = (stD2G_General*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_GENERAL;
	pInfo->cmdType = D2G_GENERAL;
	pInfo->guid = 0;
	
	pInfo->userId = userId;
	int len = sizeof(stD2G_General);
	//取武将信息
	t_GeneralRecord generalInfo;
	short count = 0;
	stGeneralInfo* pGeneral = NULL;
	while(pHandler->SelectDataByColNext(General_Define, (uint8_t*)(&generalInfo)))
	{
		pGeneral = (stGeneralInfo*)(buf+len);
		pGeneral->serialNo = generalInfo.serialNo;
		pGeneral->roleId = generalInfo.roleId;
		pGeneral->itemId = generalInfo.itemId;
		pGeneral->guid = generalInfo.guid;
		pGeneral->warFlag = generalInfo.warFlag;
		pGeneral->group = generalInfo.group;
		pGeneral->curStar = generalInfo.curStar;
		pGeneral->curLevel = generalInfo.curLevel;
		pGeneral->godSkill = generalInfo.godSkill;
		count++;
		len += sizeof(stGeneralInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	//通过uid验证并发送角色信息给Game
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadEquipInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadEquipInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from equip where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2*5];
	stD2G_Equip* pInfo = (stD2G_Equip*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_EQUIP;
	pInfo->cmdType = D2G_EQUIP;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_Equip);
	//取装备信息
	t_EquipRecord equipInfo;
	short count = 0;
	stEquipInfo* pEquip = NULL;
	while(pHandler->SelectDataByColNext(Equip_Define, (uint8_t*)(&equipInfo)))
	{
		pEquip = (stEquipInfo*)(buf+len);
		pEquip->serialNo = equipInfo.serialNo;
		pEquip->type = equipInfo.type;
		pEquip->roleId = equipInfo.roleId;
		pEquip->guid = equipInfo.guid;
		pEquip->itemId = equipInfo.itemId;
		pEquip->warFlag = equipInfo.warFlag;	
		pEquip->curStar = equipInfo.curStar;
		pEquip->curLevel = equipInfo.curLevel;
		count++;
		len += sizeof(stEquipInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	//通过uid验证并发送角色信息给Game
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadItemInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadItemInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from item where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2*5];
	stD2G_Item* pInfo = (stD2G_Item*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_ITEM;
	pInfo->cmdType = D2G_ITEM;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_Item);
	//取信息
	t_ItemRecord itemInfo;
	short count = 0;
	stItemInfo* pItem = NULL;
	while(pHandler->SelectDataByColNext(Item_Define, (uint8_t*)(&itemInfo)))
	{
		pItem = (stItemInfo*)(buf+len);
		pItem->roleId = itemInfo.roleId;
		pItem->type = itemInfo.type;
		pItem->itemId = itemInfo.itemId;
		pItem->count = itemInfo.count;	
		count++;
		len += sizeof(stItemInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	//通过uid验证并发送角色信息给Game
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadTop50Info()
{
	//printf("CDBTreator::LoadTop50Info\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from top50");
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2];
	stD2G_TopInfo* pInfo = (stD2G_TopInfo*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_TOP_INFO;
	pInfo->cmdType = D2G_TOP_INFO;
	pInfo->guid = 0;

	int len = sizeof(stD2G_TopInfo);
	//取信息
	t_TopInfo Info;
	short count = 0;
	stTopInfo* pObj = NULL;
	while(pHandler->SelectDataByColNext(TopInfo_Define, (uint8_t*)(&Info)))
	{
		pObj = (stTopInfo*)(buf+len);
		pObj->topId = Info.topId;
		pObj->roleId = Info.roleId;
		pObj->lastTime = Info.lastTime;
		count++;
		len += sizeof(stTopInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadRelationInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadRelationInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from relation where roleId1=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE10];
	stD2G_RELATION* pInfo = (stD2G_RELATION*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_RELATION;
	pInfo->cmdType = D2G_RELATION;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_RELATION);
	//取信息
	t_RelationRecord info;
	short count = 0;
	stRelationInfo* pObj = NULL;
	while(pHandler->SelectDataByColNext(Relation_Define, (uint8_t*)(&info)))
	{
		pObj = (stRelationInfo*)(buf+len);
		pObj->type = info.type;
		pObj->roleId = info.roleId2;
		pObj->addTime = info.addTime;
		count++;
		len += sizeof(stRelationInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	//通过uid验证并发送角色信息给Game
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadShopHistoryInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadShopHistoryInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from shopHistory where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE10];
	stD2G_ShopHistory* pInfo = (stD2G_ShopHistory*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_SHOP_HISTORY;
	pInfo->cmdType = D2G_SHOP_HISTORY;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_ShopHistory);
	//取信息
	t_ShopHistory info;
	short count = 0;
	stShopHistoryInfo* pShopInfo = NULL;
	while(pHandler->SelectDataByColNext(ShopHistory_Define, (uint8_t*)(&info)))
	{
		pShopInfo = (stShopHistoryInfo*)(buf+len);
		pShopInfo->roleId = info.roleId;
		pShopInfo->guid = info.guid;
		pShopInfo->count = info.count;
		pShopInfo->priceType = info.priceType;
		pShopInfo->price = info.price;
		pShopInfo->lastTime = info.lastTime;
		count++;
		len += sizeof(stShopHistoryInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadChallTaskInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadChallTaskInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from `challTask` where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2];
	stD2G_ChallTask* pInfo = (stD2G_ChallTask*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_CHALL_TASK_INFO;
	pInfo->cmdType = D2G_CHALL_TASK_INFO;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_ChallTask);
	//取信息
	t_ChallTaskInfo info;
	short count = 0;
	stChallTaskInfo* pTaskInfo = NULL;
	while(pHandler->SelectDataByColNext(ChallTaskInfo_Define, (uint8_t*)(&info)))
	{
		pTaskInfo = (stChallTaskInfo*)(buf+len);
		pTaskInfo->roleId = info.roleId;
		pTaskInfo->subTaskId = info.subTaskId;
		pTaskInfo->roleId2 = info.roleId2;
		pTaskInfo->flag = info.flag;
		
		count++;
		len += sizeof(stChallTaskInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadChallengeInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadChallengeInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from `challenge` where myRoleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2];
	stD2G_Challenge* pInfo = (stD2G_Challenge*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_CHALLENGE_INFO;
	pInfo->cmdType = D2G_CHALLENGE_INFO;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_Challenge);
	//取信息
	t_ChallengeInfo info;
	short count = 0;
	stChallengeInfo* pChallInfo = NULL;
	while(pHandler->SelectDataByColNext(ChallengeInfo_Define, (uint8_t*)(&info)))
	{
		pChallInfo = (stChallengeInfo*)(buf+len);
		pChallInfo->myRoleId = info.myRoleId;
		pChallInfo->roleId = info.roleId;
		pChallInfo->times = info.times;
		pChallInfo->type = info.type;
		pChallInfo->lastTime = info.lastTime;
		count++;
		len += sizeof(stChallengeInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadBattleInfo(unsigned int userId)
{
	//printf("CDBTreator::LoadBattleInfo\n");
	CMysqlClientHandler * pHandler = MysqlPool->GetHandlerByCharID(DB_READ);
	char sql[BUF_SIZE1];
	memset(sql, 0, BUF_SIZE1);
	sprintf(sql, "select * from `copy` where roleId=%u", userId);
	pHandler->ExecSql(sql, strlen(sql));
	pHandler->UseResult();

	char buf[BUF_SIZE2];
	stD2G_Battle* pInfo = (stD2G_Battle*)(buf);
	pInfo->cSysIdentifer = INMSG_DB_GAME;
	pInfo->cMsgID = D2G_BATTLE_INFO;
	pInfo->cmdType = D2G_BATTLE_INFO;
	pInfo->guid = 0;

	pInfo->userId = userId;
	int len = sizeof(stD2G_Battle);
	//取信息
	t_BattleInfo info;
	short count = 0;
	stBattleInfo* pBattleInfo = NULL;
	while(pHandler->SelectDataByColNext(BattleInfo_Define, (uint8_t*)(&info)))
	{
		pBattleInfo = (stBattleInfo*)(buf+len);
		pBattleInfo->roleId = info.roleId;
		pBattleInfo->mainId = info.mainId;
		pBattleInfo->curLeafId = info.curLeafId;
		count++;
		len += sizeof(stBattleInfo);
	}
	pInfo->count = count;
	pHandler->ResetResult();
	MysqlPool->PutHandler(pHandler);
	StartServer->SendToGame( buf, len);
}

void CDBTreator::LoadUserInfo(unsigned int userId)
{
	LoadRelationInfo(userId);
	LoadShopHistoryInfo(userId);
	LoadChallTaskInfo(userId);
	LoadChallengeInfo(userId);
	LoadBattleInfo(userId);
	//
	LoadGeneralInfo(userId);
	LoadEquipInfo(userId);
	//最后加载物品
	LoadItemInfo(userId);
}


	
