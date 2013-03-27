#include "GameTreator.h"
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "User.h"
#include "../Base/InternalMsg.h"
#include "../Base/Handler.h"
#include "../Base/ListenThread.h"
#include "StartServer.h"
#include "UserMgr.h"
#include "TopUser.h"

MsgHandler GameMsgHandlers[eMsg_Count];

void CGameTreator::InitPacketHandlerTable()
{
	for(int i = 0; i < eMsg_Count; ++i)
	{
		GameMsgHandlers[i].handler = 0;
	}

	GameMsgHandlers[eMsg_C2S_Logout].handler = &CGameTreator::OnLogout;
	GameMsgHandlers[eMsg_C2S_Check].handler = &CGameTreator::OnCheckUser;

	GameMsgHandlers[eMsg_C2S_TaskInfo].handler = &CGameTreator::OnTaskInfo;
	GameMsgHandlers[eMsg_C2S_Task].handler = &CGameTreator::OnTask;
	
	GameMsgHandlers[eMsg_C2S_ItemList].handler = &CGameTreator::OnItemList;
	GameMsgHandlers[eMsg_C2S_GodPlay].handler = &CGameTreator::OnGodPlay;
	GameMsgHandlers[eMsg_C2S_EquipPlay].handler = &CGameTreator::OnEquipPlay;
	GameMsgHandlers[eMsg_C2S_GodBack].handler = &CGameTreator::OnGodBack;
	GameMsgHandlers[eMsg_C2S_EquipBack].handler = &CGameTreator::OnEquipBack;
	GameMsgHandlers[eMsg_C2S_GodUpLev].handler = &CGameTreator::OnGodUpLev;
	GameMsgHandlers[eMsg_C2S_EquipUpLev].handler = &CGameTreator::OnEquipUpLev;
	GameMsgHandlers[eMsg_C2S_ChangeSkill].handler = &CGameTreator::OnGeneralChangeSkill;

	GameMsgHandlers[eMsg_C2S_GodCrack].handler = &CGameTreator::OnGodCrack;
	GameMsgHandlers[eMsg_C2S_EquipCrack].handler = &CGameTreator::OnEquipCrack;
	
	//排行榜
	GameMsgHandlers[eMsg_C2S_TopList].handler = &CGameTreator::OnTopUserList;
	//每日排行奖励
	GameMsgHandlers[eMsg_C2S_GetReward].handler = &CGameTreator::OnGetDailyReward;
	//挑战
	GameMsgHandlers[eMsg_C2S_GetChallTask].handler = &CGameTreator::OnGetChallTaskList;
	GameMsgHandlers[eMsg_C2S_ChallTask].handler = &CGameTreator::OnFlushChallTaskList;
	GameMsgHandlers[eMsg_C2S_WarList].handler = &CGameTreator::OnWarUserList;
	GameMsgHandlers[eMsg_C2S_Challenge].handler = &CGameTreator::OnUserChallenge;
	GameMsgHandlers[eMsg_C2S_ChallengeRes].handler = &CGameTreator::OnUserChallengeRes;
	GameMsgHandlers[eMsg_C2S_ChallTaskReward].handler = &CGameTreator::OnChallTaskReward;

	//荣誉兑换
	GameMsgHandlers[eMsg_C2S_HonorExcList].handler = &CGameTreator::OnHonorExcList;
	GameMsgHandlers[eMsg_C2S_HonorExchange].handler = &CGameTreator::OnHonorExchange;
	GameMsgHandlers[eMsg_C2S_UseGift].handler = &CGameTreator::OnUseGift;
	GameMsgHandlers[eMsg_C2S_UseItem].handler = &CGameTreator::OnUseItem;
	//商店
	GameMsgHandlers[eMsg_C2S_ShopList].handler = &CGameTreator::OnShopList;
	GameMsgHandlers[eMsg_C2S_ShopBuy].handler = &CGameTreator::OnShopBuy;
	//副本
	GameMsgHandlers[eMsg_C2S_CopyList].handler = &CGameTreator::OnBattleList;
	GameMsgHandlers[eMsg_C2S_DoCopy].handler = &CGameTreator::OnDoBattle;
	GameMsgHandlers[eMsg_C2S_CopyReset].handler = &CGameTreator::OnBattleReset;
}

CGameTreator::CGameTreator()
{
	m_bRun = true;
}
CGameTreator::~CGameTreator()
{
}

void CGameTreator::__run()
{
	InternalMsg* pInmsg = NULL;
	MsgHandler *pHandler;
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
		while(Get(&pInmsg))
		{
			if(INMSG_CLIENT_GAME == pInmsg->pMsg->cSysIdentifer)
			{	if (pInmsg->pMsg->cMsgID >= eMsg_Count)
				{
					assert(0);
				} 
				else
				{
					pHandler = &GameMsgHandlers[pInmsg->pMsg->cMsgID];
					(this->*(pHandler->handler))(pInmsg);
				}
			}
			else 
			{
				CHandler* pHandler = pInmsg->pHandler;
				printf("CGameTreator: pInmsg->pMsg->cSysIdentifer=%d\n", pInmsg->pMsg->cSysIdentifer);
				stServerMsg* p = (stServerMsg*)(pInmsg->pMsg);
				if(255 == p->cmdType)
				{
					CUser* pUser = (CUser*)(pHandler->GetUser());
					if(pUser)
					{
						pUser->OnDisconnect();
					}
					else
					{
						printf("not find pUser\n");
					}
				}
			}
			/*CHandler* pHandler = pInmsg->pHandler;
			if(INMSG_CLIENT_GAME == pInmsg->pMsg->cSysIdentifer)
			{
				switch(pInmsg->pMsg->cMsgID)
				{
					case eMsg_C2S_Logout:
					{
						OnLogout(pInmsg);
					}
					break;
					case eMsg_C2S_Check:
					{
						//stC2SMsgCheck* pMsg = (stC2SMsgCheck*)(pInmsg->pMsg);
						OnCheckUser(pInmsg);
					}
					break;
					case eMsg_C2S_TaskInfo:
					{
						OnTaskInfo(pInmsg);
					}
					break;
					case eMsg_C2S_Task:
					{
						OnTask(pInmsg);
					}
					case eMsg_C2S_ItemList:
					{
						OnItemList(pInmsg);
					}
					break;
					case eMsg_C2S_GodPlay:
					{
						OnGodPlay(pInmsg);
					}
					break;
					case eMsg_C2S_GodBack:
					{
						OnGodBack(pInmsg);
					}
					break;
					case eMsg_C2S_GodUpLev:
					{
						OnGodUpLev(pInmsg);
					}
					break;
					default:
					break;
				}
			}
			else 
			{
				printf("err: pInmsg->pMsg->cSysIdentifer=%d\n", pInmsg->pMsg->cSysIdentifer);
				stServerMsg* p = (stServerMsg*)(pInmsg->pMsg);
				if(255 == p->cmdType)
				{
					CUser* pUser = (CUser*)(pHandler->GetUser());
					pUser->OnDisconnect();
				}
				//assert(0);
			}*/
		}
		if(pInmsg) delete pInmsg;
		pInmsg = NULL;
	}
}

void CGameTreator::OnLogout(InternalMsg* pInmsg)
{
	printf("CGameTreator::OnLogout\n");
	CHandler* pHandler = pInmsg->pHandler;
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->Logout();
}

void CGameTreator::OnCheckUser(InternalMsg* pInmsg)
{
	printf("CGameTreator::OnCheckUser\n");
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgCheck* pMsg = (stC2SMsgCheck*)(pInmsg->pMsg);
	CheckUser(pMsg->nUserId, pHandler);
}

void CGameTreator::OnTaskInfo(InternalMsg* pInmsg)
{
	printf("CGameTreator::OnTaskInfo\n");
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgTaskInfo* pMsg = (stC2SMsgTaskInfo*)(pInmsg->pMsg);
	unsigned short nMainId = pMsg->nMainID;
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnTaskInfo(nMainId);
}

void CGameTreator::OnTask(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgTask* pMsg = (stC2SMsgTask*)(pInmsg->pMsg);
	unsigned short nMainId = pMsg->nMainID;
	unsigned short nLeafId = pMsg->nLeafID;	
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnTask(nMainId, nLeafId);
}

void CGameTreator::CheckUser(uint32_t uId, CHandler* pHandler)
{
	CUser* pUser = g_UserMgr->GetUser(uId);
	if (pUser)
	{
		pUser->SetHandler(pHandler);
		pHandler->SetUser(pUser);
		//通知客户端
		char buf[BUF_SIZE1];
		stS2CMsgCheck* pSendMsg = (stS2CMsgCheck*)(buf);
		pSendMsg->cSysIdentifer = INMSG_GAME_CLIENT;
		pSendMsg->cMsgID = eMsg_S2C_Check;
		pSendMsg->nErr = 0;//0 成功
		int len = sizeof(stS2CMsgCheck);
		pUser->Send(buf, len);
		pUser->SendRoleInfo();
		pUser->SendWarGenInfo();
		pUser->OnItemList();
		return;
	}

	char select[BUF_SIZE1];
	memset( select,0, sizeof( select ));
	sprintf(select, "select * from role where userId=%u", uId);
	char buf[BUF_SIZE1];
	stG2D_Select* pSel = (stG2D_Select*)buf;
	pSel->cSysIdentifer = INMSG_GAME_DB;
	pSel->cMsgID = G2D_USER_CKECK;
	pSel->cmdType = G2D_USER_CKECK;
	pSel->guid = pHandler->GetGuid();
	pSel->nSelLen = strlen(select);
	pSel->select = NULL;
	int len=sizeof(stG2D_Select);
	
	memcpy(buf+len, select, pSel->nSelLen);
	len += pSel->nSelLen;
	
	StartServer->SendToDB( buf, len);
}

void CGameTreator::OnItemList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgItemList* pMsg = (stC2SMsgItemList*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	switch(pMsg->type)
	{//0 出战武将; 1 武将; 2 装备; 3 材料; 4 回复活力值道具; 5 增加挑战次数道具; 6 礼包
		case 0:
		{
			pUser->SendWarGenInfo();
		}
		break;
		case 1:
		{
			pUser->OnGodList();
		}
		break;
		case 2:
		{
			pUser->OnEquipList();
		}
		break;
		case 3:
		{
			pUser->OnItemList();
		}
		break;
		default:
		break;
	}
	
}

void CGameTreator::OnGodPlay(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgGodPlay* pMsg = (stC2SMsgGodPlay*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGeneralPlay(pMsg->serialNo, pMsg->warPos);
}

void CGameTreator::OnEquipPlay(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgEquipPlay* pMsg = (stC2SMsgEquipPlay*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnEquipPlay(pMsg->serialNo, pMsg->warPos);
}

void CGameTreator::OnGodBack(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgGodPlay* pMsg = (stC2SMsgGodPlay*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGeneralBack(pMsg->serialNo, pMsg->warPos);
}

void CGameTreator::OnEquipBack(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgEquipBack* pMsg = (stC2SMsgEquipBack*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnEquipBack(pMsg->serialNo, pMsg->warPos);
}

void CGameTreator::OnGodUpLev(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgGodUpLev* pMsg = (stC2SMsgGodUpLev*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGeneralUpLev(pMsg->godGuid, pMsg->serialNo, pMsg->serialNo2);
}

void CGameTreator::OnEquipUpLev(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgEquipUpLev* pMsg = (stC2SMsgEquipUpLev*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnEquipUpLev(pMsg->equipGuid, pMsg->serialNo);
}

void CGameTreator::OnGeneralChangeSkill(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgChangeSkill* pMsg = (stC2SMsgChangeSkill*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGeneralChangeSkill(pMsg->guid, pMsg->serialNo);
}

void CGameTreator::OnGodCrack(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgGodCrack* pMsg = (stC2SMsgGodCrack*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGodCrack(pMsg->guid, pMsg->serialNo);
}

void CGameTreator::OnEquipCrack(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgEquipCrack* pMsg = (stC2SMsgEquipCrack*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnEquipCrack(pMsg->guid, pMsg->serialNo);
}

void CGameTreator::OnTopUserList(InternalMsg* pInmsg)
{
	//printf("CGameTreator::OnTopUserList\n");
	CHandler* pHandler = pInmsg->pHandler;
	char buf[BUF_SIZE2];
	stS2CMsgTopList* pMsg = (stS2CMsgTopList*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_CLIENT;
	pMsg->cMsgID = eMsg_S2C_TopList;
	pMsg->pList = NULL;
	int len = sizeof(stS2CMsgTopList);
	len += g_TopInfoMgr->FillTopRoleInfo(buf+len, BUF_SIZE2-len, &(pMsg->count));
	pHandler->Send(buf, len);
}

void CGameTreator::OnGetDailyReward(InternalMsg* pInmsg)
{
	printf("CGameTreator::OnGetDailyReward\n");
	CHandler* pHandler = pInmsg->pHandler;
	//stC2SMsgGetReward* pMsg = (stC2SMsgGetReward*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnDailyReward();
}

void CGameTreator::OnGetChallTaskList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnGetChallTaskList();
}

void CGameTreator::OnFlushChallTaskList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	//stC2SMsgChallTask* pMsg = (stC2SMsgChallTask*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnFlushChallTaskList();
}

void CGameTreator::OnWarUserList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgWarList* pMsg = (stC2SMsgWarList*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	if(pMsg->nameLen > 0)
	{
		char szName[MAX_NAME_SIZE];
		memset(szName, 0, sizeof(szName));
		int offset = sizeof(stC2SMsgWarList);
		memcpy(szName, (char*)pMsg + offset, pMsg->nameLen);
		pUser->OnWarUserList(pMsg->type, szName, pMsg->count);
	}
	else
	{
		pUser->OnWarUserList(pMsg->type, NULL, pMsg->count);
	}
}

void CGameTreator::OnUserChallenge(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgChallenge* pMsg = (stC2SMsgChallenge*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnUserChallenge(pMsg->userId, pMsg->type, pMsg->topId);
}

void CGameTreator::OnUserChallengeRes(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgChallenge* pMsg = (stC2SMsgChallenge*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnUserChallengeRes(pMsg->userId);
}

void CGameTreator::OnChallTaskReward(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	//stC2SMsgChallTaskReward* pMsg = (stC2SMsgChallTaskReward*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnChallTaskReward();
}

void CGameTreator::OnHonorExcList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	//stC2SMsgHonorExcList* pMsg = (stC2SMsgHonorExcList*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnHonorExcList();
}

void CGameTreator::OnHonorExchange(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgHonorExchange* pMsg = (stC2SMsgHonorExchange*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnHonorExchange(pMsg->id);
}

void CGameTreator::OnUseGift(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgUseGift* pMsg = (stC2SMsgUseGift*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->UseGift(pMsg->giftId);
}

void CGameTreator::OnUseItem(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgUserItem* pMsg = (stC2SMsgUserItem*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->UseItem(pMsg->id, pMsg->count);
}

void CGameTreator::OnShopList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnShopList();
}

void CGameTreator::OnShopBuy(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgShopBuy* pMsg = (stC2SMsgShopBuy*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnShopBuy(pMsg->id);
}

void CGameTreator::OnBattleList(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnBattleList();
}

void CGameTreator::OnDoBattle(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgDoCopy* pMsg = (stC2SMsgDoCopy*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnDoBattle(pMsg->mainCopyId, pMsg->leafCopyId);
}

void CGameTreator::OnBattleReset(InternalMsg* pInmsg)
{
	CHandler* pHandler = pInmsg->pHandler;
	stC2SMsgCopyReset* pMsg = (stC2SMsgCopyReset*)(pInmsg->pMsg);
	CUser* pUser = (CUser*)(pHandler->GetUser());
	pUser->OnBattleReset(pMsg->mainId);
}

