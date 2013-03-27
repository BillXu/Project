//#include "User.h"
#include "UserMgr.h"
#include "StartServer.h"
#include "Skill.h"
#include "SkillManager.h"
#include "ExchangeConfig.h"
#include "ShopConfig.h"
#include "ServerTreator.h"
#include "CompeteRankConfig.h"
#include "TopUser.h"

CUser::CUser(uint32_t uid)
{
	m_pHandler = NULL;
	m_nUserId = uid;
	memset(m_szName, 0, MAX_NAME_SIZE);
	memset(m_szPwd, 0, MAX_PASS_SIZE);
	m_nOnlineFlag = 0;
	memset(m_szEmail, 0, MAX_EMAIL_SIZE);
	memset(m_szMobile, 0, MAX_MOBILE_LEN);
	m_bChangeFlag = false;
	memset(&m_roleInfo, 0, sizeof(m_roleInfo));
	memset(&m_curLevelInfo, 0, sizeof(m_curLevelInfo));
	m_nCheckNo = 0;
	memset(m_warGeneral, 0, sizeof(t_WarGeneralInfo)*WAR_POS_MAX);
	m_msLastChangeVitality = 0;
	m_msLastChangeChallTime = 0;

	m_status = STATUS_IDLE;
	memset(&m_challengeRes, 0, sizeof(m_challengeRes));
}

CUser::~CUser()
{
	
}

void CUser::SendPropBack()
{
	stVitalityBack msg;
	int len = sizeof(msg);
	Lock();
	msg.vitality = m_roleInfo.vitality;
	msg.leftChallengeTimes = m_roleInfo.leftChallengeTimes;
	UnLock();
	if (m_pHandler)
	{
		m_pHandler->Send( (char*)(&msg), len);
	}
}

void CUser::Update(uint32_t tim)
{
	//回复体力
	if (tim - m_msLastChangeVitality >= m_curLevelInfo.actvieRestoreSpeed*1000)
	{
		m_msLastChangeVitality = tim;
		Lock();
		if (m_roleInfo.vitality < m_curLevelInfo.maxActive)
		{
			m_roleInfo.vitality++;
			m_bChangeFlag = true;
			stVitalityBack msg;
			int len = sizeof(msg);
			msg.vitality = m_roleInfo.vitality;
			msg.leftChallengeTimes = m_roleInfo.leftChallengeTimes;
			UnLock();
			if (m_pHandler)
			{
				m_pHandler->Send( (char*)(&msg), len);
			}
		}
		else
		{
			UnLock();
		}
	}
	if (tim - m_msLastChangeChallTime >= 3600*1000)	//1小时暂时写死
	{
		m_msLastChangeChallTime = tim;
		Lock();
		if (m_roleInfo.leftChallengeTimes < 20) //暂时写死
		{
			m_roleInfo.leftChallengeTimes++;
			stVitalityBack msg;
			int len = sizeof(msg);
			msg.vitality = m_roleInfo.vitality;
			msg.leftChallengeTimes = m_roleInfo.leftChallengeTimes;
			UnLock();
			if (m_pHandler)
			{
				m_pHandler->Send( (char*)(&msg), len);
			}
		} 
		else
		{
			UnLock();
		}
	}
}

void CUser::LoadTaskInfo(char* buf, short count)
{
	//printf("CUser::LoadTaskInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stTaskInfo* p2 = (stTaskInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stTaskInfo*)p;
		t_TaskInfo* pTask = new t_TaskInfo;
		pTask->mainId = p2->mainId;
		pTask->leafId = p2->leafId;
		pTask->leftTimes = p2->leftTimes;
		//memcpy(pTask, p, sizeof(t_TaskInfo));
		m_task.Insert(pTask);
		p += sizeof(stTaskInfo);
	}
}

void CUser::LoadGeneralInfo(char* buf, short count)
{
	printf("CUser::LoadGeneralInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stGeneralInfo* p2 = (stGeneralInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stGeneralInfo*)p;
		
		CGeneral* pGen = new CGeneral(p2);
		pGen->SetType(eItem_God);
		pGen->SetSerialNo(p2->serialNo);
		pGen->Add();
		m_ItemMgr.AddItem(pGen);
		p += sizeof(stGeneralInfo);
	}
	//加载出战武将信息
	LoadWarGeneral();
	//暂时放此处发送
	//SendRoleInfo();
	//发送客户端出战武将列表
	//SendWarGenInfo();
}

void CUser::LoadEquipInfo(char* buf, short count)
{
	printf("CUser::LoadEquipInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stEquipInfo* p2 = (stEquipInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stEquipInfo*)p;

		CEquip* pEquip = new CEquip(p2);
		pEquip->SetType(eItem_Equip);
		pEquip->SetSerialNo(p2->serialNo);
		pEquip->Add();
		m_ItemMgr.AddItem(pEquip);
		p += sizeof(stEquipInfo);
	}
	//加载使用装备信息
	LoadWarEquip();
	//武将 装备加载完毕后计算攻防信息
	for (int i=0; i < WAR_POS_MAX; i++)
	{
		Calculate(i+1, true);
	}
	//暂时放此处发送
	//SendRoleInfo();
	//发送客户端出战武将列表
	//SendWarGenInfo();
}

void CUser::LoadItemInfo(char* buf, short count)
{
	//printf("CUser::LoadItemInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stItemInfo* p2 = (stItemInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stItemInfo*)p;

		CCommItem* pItem = new CCommItem(p2);
		if(eItem_Material == p2->type)
		{
			pItem->SetType(eItem_Material);
		}
		else if(eItem_RestoreActive == p2->type)
		{
			pItem->SetType(eItem_RestoreActive);
		}
		else if (eItem_AddChallengeTimes == p2->type)
		{
			pItem->SetType(eItem_AddChallengeTimes);
		}
		else if (eItem_Gift == p2->type)
		{
			pItem->SetType(eItem_Gift);
		}
		else
		{
			printf("Err: CUser::LoadItemInfo type=%d\n", p2->type);
		}
		pItem->Add(p2->count);
		m_ItemMgr.AddItem(pItem, true);
		p += sizeof(stItemInfo);
	}
	SendRoleInfo();
	OnItemList();
	SendWarGenInfo();
	//数据加载结束，通知客户端登陆成功(避免取不到item列表等数据)
	/*stS2CMsgCheck msg;// pSendMsg = (stS2CMsgCheck*)(buf2);
	msg.nErr = 0;//0 成功
	int len = sizeof(stS2CMsgCheck);
	m_pHandler->Send((char*)(&msg), len);*/
}

void CUser::LoadRelationInfo(char* buf, short count)
{
	//printf("CUser::LoadRelationInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stRelationInfo* p2 = (stRelationInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stRelationInfo*)p;

		CRelation* pObj = new CRelation();
		pObj->SetType(p2->type);
		pObj->SetUserId(p2->roleId);
		/*if(!g_UserMgr->GetUser(p2->roleId))
		{
			ServerTreator->LoginUserId(p2->roleId);
		}*/
		pObj->SetTime(p2->addTime);
		m_RelationMgr.Add(pObj);
		p += sizeof(stRelationInfo);
	}
}

void CUser::LoadShopHistoryInfo(char* buf, short count)
{
	//printf("CUser::LoadShopHistoryInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stShopHistoryInfo* p2 = (stShopHistoryInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stShopHistoryInfo*)p;

		CShopHistory* pObj = new CShopHistory(p2);
		m_ShopHistoryMgr.Add(pObj);
		p += sizeof(stShopHistoryInfo);
	}
}

void CUser::LoadChallengeInfo(char* buf, short count)
{
	//printf("CUser::LoadChallengeInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stChallengeInfo* p2 = (stChallengeInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stChallengeInfo*)p;

		CChallenge* pObj = new CChallenge(p2);
		m_ChallengeMgr.Add(pObj);
		p += sizeof(stChallengeInfo);
	}
}

void CUser::LoadChallTaskInfo(char* buf, short count)
{
	//printf("CUser::LoadChallTaskInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stChallTaskInfo* p2 = (stChallTaskInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stChallTaskInfo*)p;

		CChallSubTask* pObj = new CChallSubTask(p2);
		m_ChallTaskMgr.Add(pObj);
		p += sizeof(stChallTaskInfo);
	}
}

void CUser::LoadBattleInfo(char* buf, short count)
{
	printf("CUser::LoadBattleInfo\n");
	assert(buf && count > 0);
	char* p = buf;
	stBattleInfo* p2 = (stBattleInfo*)p;
	for(int i = 0; i<count; i++)
	{
		p2 = (stBattleInfo*)p;

		CBattle* pObj = new CBattle(p2);
		m_battleMgr.Add(pObj);
		p += sizeof(stBattleInfo);
	}
}

stGodCard* CUser::GetGodStaticInfo(unsigned int guid)
{
	return CItemManager::SharedItemMgr()->GetGodCardByUID(guid);
}

stConferGod* CUser::GetConferGodStaticInfo(unsigned int guid)
{
	return CGodOperateConfigMgr::ShareGodOperateConfigMgr()->GetConferGodConfig(guid);
}

stUnconferGod* CUser::GetUnconferGodStaticInfo(unsigned int guid)
{
	return CGodOperateConfigMgr::ShareGodOperateConfigMgr()->GetUnconferGodConfig(guid);
}

stEquip* CUser::GetEquipStaticInfo(unsigned int guid)
{
	return CItemManager::SharedItemMgr()->GetEquipByUID(guid);
}

void CUser::LoadWarGeneral()
{
	printf("CUser::LoadWarGeneral\n");
	CPlrItemMgr::MAP_ITEMS::iterator ipos = m_ItemMgr.m_vItems2[eItem_God].begin();
	CPlrItemMgr::MAP_ITEMS::iterator iend = m_ItemMgr.m_vItems2[eItem_God].end();
	//int offset = 0;
	//t_WarGeneralInfo* pInfo = NULL;
	CGeneral* pGen = NULL;
	//stGodCard* pGenStatic = NULL;
	//int n = 0;
	while(ipos != iend)
	{
		pGen = (CGeneral*)(ipos->second);
		int pos = pGen->m_info.warFlag;
		if(pos > 0)
		{
			if(pos<=WAR_POS_MAX)
			{
				m_warGeneral[pos-1].warPos = pos;
				m_warGeneral[pos-1].serialNo = pGen->GetSerialNo();
				m_warGeneral[pos-1].godSkill[0] = pGen->m_info.godSkill;
				m_warGeneral[pos-1].guid = pGen->GetGuid();

				CalculateBasic(pos-1);
				//pGenStatic = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[pos-1].guid);
				//assert(pGenStatic);
				//m_warGeneral[pos-1].ability += pGenStatic->nAbility;
				/*m_warGeneral[pos-1].ltAttack += pGenStatic->nAttackLow;
				m_warGeneral[pos-1].utAttack += pGenStatic->nAttackUp;
				m_warGeneral[pos-1].ltDefence += pGenStatic->nDefendLow;
				m_warGeneral[pos-1].utDefence += pGenStatic->nDefendUp;*/
			}
			else
			{
				//副将信息
				int mainPos, nPos;
				assert( GetAdjutantPos(pos, &mainPos, &nPos) );
				m_warGeneral[mainPos-1].adjutantGuid[nPos-1] = pGen->GetGuid();
				m_warGeneral[mainPos-1].adjutantNo[nPos-1] = pGen->GetSerialNo();

			}
		}
		++ipos;
	}
}

void CUser::LoadWarEquip()
{
	printf("CUser::LoadWarEquip\n");
	CPlrItemMgr::MAP_ITEMS::iterator ipos = m_ItemMgr.m_vItems2[eItem_Equip].begin();
	CPlrItemMgr::MAP_ITEMS::iterator iend = m_ItemMgr.m_vItems2[eItem_Equip].end();
	//int offset = 0;
	//t_WarGeneralInfo* pInfo = NULL;
	CEquip* pEquip = NULL;
	//int n = 0;
	while(ipos != iend)
	{
		pEquip = (CEquip*)(ipos->second);
		int posIndex = pEquip->m_info.warFlag-1;
		if (posIndex >= 0)
		{
			if(posIndex >= 0 && posIndex < WAR_POS_MAX)
			{
				if ( eEquip_Weapon == pEquip->GetEquipType())
				{
					m_warGeneral[posIndex].weaponGuid = pEquip->GetGuid();
					m_warGeneral[posIndex].weaponNo = pEquip->GetSerialNo();
				}
				else if (eEquip_Defend == pEquip->GetEquipType())
				{
					m_warGeneral[posIndex].armorGuid =pEquip->GetGuid();
					m_warGeneral[posIndex].armorNo = pEquip->GetSerialNo();
				}
				else if (eEquip_Treasure == pEquip->GetEquipType())
				{
					m_warGeneral[posIndex].treasureGuid = pEquip->GetGuid();
					m_warGeneral[posIndex].treasureNo = pEquip->GetSerialNo();
				}
			}
			CalculateEquip(pEquip);
		}
		++ipos;
	}
}

void CUser::SendWarGenInfo()
{
	if (!m_pHandler)
	{
		return;
	}
	printf("CUser::SendWarGenInfo\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgWarGodList* pWarInfo = (stS2CMsgWarGodList*)buf;
	pWarInfo->cSysIdentifer = MSG_GAME_S2C;
	pWarInfo->cMsgID = eMsg_S2C_WarGodList;
	int len = sizeof(stS2CMsgWarGodList);
	int n = 0;
	t_WarGodInfo* pTmp = NULL;
	for(int i=0;i < WAR_POS_MAX; i++)
	{
		if(m_warGeneral[i].guid > 0)
		{
			++n;
			pTmp = (t_WarGodInfo*)(buf+len);
			FillWarGodInfo(m_warGeneral[i].warPos, pTmp);
			len += sizeof(t_WarGodInfo);
		}
	}
	pWarInfo->count = n;
	
	m_pHandler->Send( buf, len);
}

bool CUser::SendOneWarGenInfo(int index)
{
	printf("CUser::SendOneWarGenInfo\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgWarGodList* pWarInfo = (stS2CMsgWarGodList*)buf;
	pWarInfo->cSysIdentifer = MSG_GAME_S2C;
	pWarInfo->cMsgID = eMsg_S2C_WarGodList;
	int len = sizeof(stS2CMsgWarGodList);
	int n = 0;
	t_WarGodInfo* pTmp = NULL;
	if(m_warGeneral[index].guid > 0)
	{
		++n;
		pTmp = (t_WarGodInfo*)(buf+len);
		FillWarGodInfo(m_warGeneral[index].warPos, pTmp);
		len += sizeof(t_WarGodInfo);
	}
	pWarInfo->count = n;
	
	m_pHandler->Send( buf, len);
	return true;
}

bool CUser::OnReward(stRewardGroup* pRewardGroup, unsigned char type)
{
	printf("CUser::OnReward\n");
	//奖励
	stReward* pReward = pRewardGroup->RandReward();
	//更新角色信息
	Lock();
	m_roleInfo.coin += pReward->iCoin;
	m_roleInfo.exp += pReward->iExp;
	m_roleInfo.honor += pReward->iHonour;
	m_roleInfo.gold += pReward->iGolden;
	m_bChangeFlag = true;
	unsigned int curExp = m_roleInfo.exp;
	UnLock();
	//奖励
	char buf2[BUF_SIZE1];
	memset(buf2, 0, BUF_SIZE1);
	stS2CMsgReward* pRewardMsg = (stS2CMsgReward*)buf2;
	pRewardMsg->cSysIdentifer = MSG_GAME_S2C;
	pRewardMsg->cMsgID = eMsg_S2C_Reward;
	pRewardMsg->nType = type;		 //1 任务奖励; 2 升级奖励 3 礼包奖励
	pRewardMsg->nCoin = pReward->iCoin;
	pRewardMsg->nExp = pReward->iExp;
	pRewardMsg->nHonour = pReward->iHonour;
	pRewardMsg->nGolden = pReward->iGolden;
	pRewardMsg->nItemId = pReward->RandItemId();
	//printf("CUser::OnReward RandItemId=%u\n", pRewardMsg->nItemId);
	pRewardMsg->nSerialNo = 0;
	if (pRewardMsg->nItemId > 0)
	{
		IItemBase* pItem = m_ItemMgr.Add(pRewardMsg->nItemId);
		if(pItem)
		{
			pRewardMsg->nSerialNo = pItem->GetSerialNo();
		}
	}

	int sendLen = sizeof(stS2CMsgReward);
	if (m_pHandler)
	{
		m_pHandler->Send( buf2, sendLen);
	}
	else
	{
		assert(0);
	}
	return CheckLevelUp(curExp);
}

void CUser::OnDailyReward()
{
	stS2CMsgGetReward msg;
	if (m_roleInfo.oldTopId > 0)
	{
		CCompeteRankConfig::stRankClearing* pClear = CCompeteRankConfig::SharedMgr()->GetRankClearingForRank(m_roleInfo.oldTopId, false);;
		if (pClear)
		{
			stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(pClear->nGroupID);
			OnReward(pRewardGroup, 3);//3 每日排行奖励
			return;
		}
		else
		{
			msg.nErr = 2;
		}
	}
	msg.nErr = 1;
	m_pHandler->Send((char*)(&msg), sizeof(msg));
}
	
bool CUser::CheckLevelUp(unsigned int exp)
{
	Lock();
	stLevelData* pNextLevel = CLevelDataManager::ShardLevelDataMgr()->GetLevelData(m_roleInfo.level+1);
	if(pNextLevel && exp >= (unsigned int)(pNextLevel->nExpNeed) )
	{
		m_roleInfo.exp -= pNextLevel->nExpNeed;
		m_roleInfo.level += 1;
		m_bChangeFlag = true;
		UnLock();
		OnLevelUp(pNextLevel);
		return true;
	}
	else
	{
		UnLock();
	}
	return false;
}

void CUser::OnLevelUp(stLevelData* pcurLevel)
{
	printf("CUser::OnLevelUp\n");
	Lock();
	m_curLevelInfo.maxActive = pcurLevel->nMaxActive;
	m_curLevelInfo.actvieRestoreSpeed = pcurLevel->nActiveRestorePerSec;
	m_curLevelInfo.attackLow = pcurLevel->nAttackLow;
	m_curLevelInfo.attackUp = pcurLevel->nAttackUp;
	m_curLevelInfo.defendLow = pcurLevel->nDefendLow;
	m_curLevelInfo.defendUp = pcurLevel->nDefendUp;
	//加满活力值
	m_roleInfo.vitality = m_curLevelInfo.maxActive;
	m_bChangeFlag = true;
	//升级
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgLevelUp* pMsg = (stS2CMsgLevelUp*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_LevelUp;
	pMsg->nExp = m_roleInfo.exp;
	pMsg->newLev = m_roleInfo.level;
	UnLock();
	int sendLen = sizeof(stS2CMsgLevelUp);
	m_pHandler->Send( buf, sendLen);
	//升级奖励
	stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(pcurLevel->nRewardID);
	OnReward(pRewardGroup, 2);//2 升级奖励
	//m_battleMgr.CheckOpenBattle(m_roleInfo.userId, m_roleInfo.level);
}

void CUser::OnTaskInfo(unsigned int nMainId)
{
	printf("CUser::OnTaskInfo\n");
	char buf[BUF_SIZE2];
	memset(buf, 0, BUF_SIZE2);
	stS2CMsgTaskInfo* pMsg = (stS2CMsgTaskInfo*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_TaskInfo;
	//int countOffset = sizeof(stMsg);
	pMsg->pInfo = NULL;
	int len = sizeof(stS2CMsgTaskInfo);
	unsigned short count;
	len += m_task.FillMainTaskInfo(nMainId, buf+len, &count);
	pMsg->nMainID = nMainId;
	pMsg->nCount = count;
	m_pHandler->Send( buf, len);
}

void CUser::OnTask(unsigned int nMainId, unsigned int nLeafId)
{
	//printf("CUser::OnTask\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgTask* pMsg = (stS2CMsgTask*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_Task;
	pMsg->nMainID = nMainId;
	pMsg->nLeafID = nLeafId;
	pMsg->nVitality = m_roleInfo.vitality;
	int len = 0;
	stMainMissionConfig* pMainTask = CMissionMgr::SharedMissionMgr()->GetMainMission(nMainId);
	if(!pMainTask)
	{
		pMsg->nErr = 1;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		return;
	}
	if(m_roleInfo.level < pMainTask->iLevelNeed)
	{
		pMsg->nErr = 2;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		//printf("info: pMainTask->iLevelNeed(%d) < m_roleInfo.level(%d)\n", pMainTask->iLevelNeed, m_roleInfo.level);
		return;
	}
	stLeafMissionConfig* pSubTask = pMainTask->GetLeafMissionByIndex( nLeafId );
	if(!pSubTask)
	{
		pMsg->nErr = 3;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		//printf("info: client main id=%d leaf id=%d\n", nMainId, nLeafId);
		return;
	}
	Lock();
	if(pSubTask->iConsumeActive > m_roleInfo.vitality)
	{
		UnLock();
		pMsg->nErr = 4;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		return;
	}
	else
	{
		UnLock();
	}
	//完成次数
	t_TaskInfo* pTaskInfo = m_task.GetTaskInfo(nMainId, nLeafId);
	if(pTaskInfo && pTaskInfo->leftTimes > 0)
	{
		//更新可执行次数
		pTaskInfo->leftTimes -= 1;
		int finishIndex = pSubTask->iMaxfinish - pTaskInfo->leftTimes;
		stRewardGroup* pRewardGroup = pSubTask->GetRewardGroup(finishIndex);
		if(!pRewardGroup)
		{
			pMsg->nErr = 6;
			len = sizeof(stS2CMsgTask);
			m_pHandler->Send( buf, len);
			return;
		}
		//扣减活力值ֵ
		Lock();
		m_roleInfo.vitality -= pSubTask->iConsumeActive;
		pMsg->nVitality = m_roleInfo.vitality;
		m_bChangeFlag = true;
		UnLock();
		m_msLastChangeVitality = getMSTime();
		pTaskInfo->SetFlag(3);
		pMsg->nErr = 0;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		//奖励
		OnReward(pRewardGroup, 1);//1 任务奖励
	}
	else
	{
		pMsg->nErr = 5;
		len = sizeof(stS2CMsgTask);
		m_pHandler->Send( buf, len);
		return;
	}
	
	if (nLeafId >= (unsigned int)pMainTask->GetLeafMissionCount()-1)
	{
		m_task.SetAllLeafDone();
	}
	if(m_task.IsAllLeafDone())
	{
		int nextMainIndex = CMissionMgr::SharedMissionMgr()->GetMainMissionIndexByID(m_roleInfo.curTask) + 1;
		stMainMissionConfig* pNextMainTask = CMissionMgr::SharedMissionMgr()->GetMainMissionByIndex(nextMainIndex);
		if (pNextMainTask && m_roleInfo.level >= pNextMainTask->iLevelNeed)
		{//开启新主任务
			m_task.AddTaskInfo(m_roleInfo.userId, pNextMainTask);
			m_task.ResetAllLeafDone();
			Lock();
			m_roleInfo.curTask = pNextMainTask->iMainID;
			UnLock();
			SaveRoleBase();	//即使存储数据
		}
	}
}


void CUser::OnBattleList()
{
	//printf("CUser::OnBattleList\n");
	char buf[BUF_SIZE2];
	memset(buf, 0, BUF_SIZE2);
	stS2CMsgCopyList* pMsg = (stS2CMsgCopyList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_CopyList;
	pMsg->pList = NULL;
	int len = sizeof(stS2CMsgCopyList);
	unsigned short count;
	len += m_battleMgr.FillBattleInfo(buf+len, &count);
	pMsg->leftTime = m_battleMgr.GetResetLeftTime();
	pMsg->count = count;
	m_pHandler->Send( buf, len);
}

void CUser::OnDoBattle(unsigned short mainId, unsigned short curLeafId)
{
	//printf("CUser::OnDoBattle\n");
	stS2CMsgDoCopy msg;
	CMissionMgr* pMgr = CMissionMgr::SharedMissionMgr();
	//int n = pMgr->GetMainMissionCount(eMission_Battle);
	stMainMissionConfig* pMainConfig = pMgr->GetMainMission(mainId, eMission_Battle);
	if (!pMainConfig)
	{
		msg.nErr = 3;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	stLeafMissionBattle* pLeafConfig = (stLeafMissionBattle*)pMainConfig->GetLeafMissionByIndex(curLeafId);
	if (!pLeafConfig)
	{
		msg.nErr = 4;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	CBattle* pBattle = m_battleMgr.Get(mainId);
	if (pBattle && pBattle->m_info.curLeafId != curLeafId)
	{
		msg.nErr = 4;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	if (m_roleInfo.level < (unsigned char)pMainConfig->iLevelNeed)
	{
		msg.nErr = 1;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	if (m_roleInfo.vitality < (unsigned int)pLeafConfig->iConsumeActive)
	{
		msg.nErr = 2;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	Lock();
	m_roleInfo.vitality -= pLeafConfig->iConsumeActive;
	UnLock();

	//attack
	unsigned int attack[WAR_POS_MAX]={0};
	unsigned int attack2[WAR_POS_MAX]={0};
	CGeneral* pAttackGen[WAR_POS_MAX] = {NULL,NULL,NULL,NULL,NULL};
	//defence
	unsigned int defence = 0;
	unsigned int defence2 = 0;
	//出战成员数值
	printf("*********OnDoBattle(): ");
	for (int i=0; i<WAR_POS_MAX; i++)
	{
		attack[i] = GetGeneralAttack(i);
		printf("attack[%d]=%u\n", i, attack[i]);
		attack2[i] = attack[i];
	}
	//副本
	int up = pLeafConfig->iUpDefend;
	int down = pLeafConfig->iLowDefend;
	int sz = up - down;
	if (sz <= 0)
	{
		defence = 0;
	} 
	else
	{
		defence = rand()%(sz+1) + down;
	}
	defence2 = defence;

	//封神技能
	stConferGodSkll* pAttackSkill[WAR_POS_MAX][SKILL_MAX_PER_GOD] = {{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL}};
	stConferGodSkll* pDefenceSkill[SKILL_MAX_PER_GOD] = {NULL,NULL,NULL,NULL};
	stConferGodSkll* pTmpSkill = NULL;
	for (int i=0; i<WAR_POS_MAX; i++)
	{
		pAttackGen[i] = GetWarGeneral(i);
		for(int j=0;j<SKILL_MAX_PER_GOD;j++)
		{
			pTmpSkill = (stConferGodSkll*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(GetGodSkillGuid(i,j));
			if(pTmpSkill && pTmpSkill->CanInvoke())
			{
				pAttackSkill[i][j] = pTmpSkill;
			}
		}
	}
	//defence
	for(int j=0;j<BATTLE_MISSION_SKILL;j++)
	{
		pTmpSkill = (stConferGodSkll*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(pLeafConfig->vSkillID[j]);
		if(pTmpSkill && pTmpSkill->CanInvoke())
		{
			pDefenceSkill[j] = pTmpSkill;
		}
	}
	//计算攻方封神技
	for(int i=0;i<WAR_POS_MAX;i++)
	{
		for(int j=0;j<SKILL_MAX_PER_GOD;j++)
		{
			if (pAttackSkill[i][j])
			{
				eCalculateType type = pAttackSkill[i][j]->GetCalculateType();
				switch (type)
				{
				case eSelf_Add_RenGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Ren == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}

					}
					break;
				case eSelf_Add_ChanGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Chan == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}
					}
					break;
				case eSelf_Add_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Jie == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}
					}
					break;
				case eOther_Reduce_RenGroup:
					{
		
						if ( eCardGroup_Ren == pLeafConfig->eGroupType )
						{
							defence2 -= (unsigned int)(defence*pAttackSkill[i][j]->fDefend);
						}
					}
					break;
				case eOther_Reduce_ChanGroup:
					{
						if ( eCardGroup_Chan == pLeafConfig->eGroupType )
						{
							defence2 -= (unsigned int)(defence*pAttackSkill[i][j]->fDefend);
						}
					}
					break;
				case eOther_Reduce_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( eCardGroup_Jie == pLeafConfig->eGroupType )
							{
								defence2 -= (unsigned int)(defence*pAttackSkill[i][j]->fDefend);
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
	//计算守方封神技
	for(int j=0;j<SKILL_MAX_PER_GOD;j++)
	{
		if (pDefenceSkill[j])
		{
			eCalculateType type = pDefenceSkill[j]->GetCalculateType();
			switch (type)
			{
			case eSelf_Add_RenGroup:
				{
					if ( eCardGroup_Ren == pLeafConfig->eGroupType )
					{
						defence2 += (unsigned int)(defence*pDefenceSkill[j]->fDefend);
					}
				}
				break;
			case eSelf_Add_ChanGroup:
				{
					if ( eCardGroup_Chan == pLeafConfig->eGroupType )
					{
						defence2 += (unsigned int)(defence*pDefenceSkill[j]->fDefend);
					}
				}
				break;
			case eSelf_Add_JieGroup:
				{
					if ( eCardGroup_Jie == pLeafConfig->eGroupType )
					{
						defence2 += (unsigned int)(defence*pDefenceSkill[j]->fDefend);
					}
				}
				break;
			case eOther_Reduce_RenGroup:
				{
					for (int k=0;k<WAR_POS_MAX;k++)
					{
						if ( pAttackGen[k] && (eCardGroup_Ren == pAttackGen[k]->GetGroupType()) ) 
						{
							attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[j]->fAttack);
						}
					}
				}
				break;
			case eOther_Reduce_ChanGroup:
				{
					for (int k=0;k<WAR_POS_MAX;k++)
					{
						if ( pAttackGen[k] && (eCardGroup_Chan == pAttackGen[k]->GetGroupType()) )
						{
							attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[j]->fAttack);
						}
					}
				}
				break;
			case eOther_Reduce_JieGroup:
				{
					for (int k=0;k<WAR_POS_MAX;k++)
					{
						if ( pAttackGen[k] && (eCardGroup_Jie == pAttackGen[k]->GetGroupType()) )
						{
							attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[j]->fAttack);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	//攻防总值
	unsigned int attackAll = GetLevelAttack();
	unsigned int defenceAll = defence2;
	for (int i=0;i<WAR_POS_MAX;i++)
	{
		attackAll += attack2[i];
	}
	msg.nErr = 0;
	if (attackAll > defenceAll)
	{
		msg.result = 0;
	}
	else
	{
		msg.result = 1;
	}
	msg.attactVal = attackAll;
	msg.defenceVal = defenceAll;
	printf("attactVal=%u, defenceVal=%u\n*****",attackAll, defenceAll);
	msg.vitality = m_roleInfo.vitality;
	msg.mainCopyId = mainId;
	if (0 == msg.result)
	{
		msg.nextLeafCopyId = curLeafId + 1;
	} 
	else
	{
		msg.nextLeafCopyId = curLeafId;
	}
	m_pHandler->Send((char*)(&msg), sizeof(msg));
	if (0 == msg.result)
	{
		if(m_battleMgr.Update(mainId, msg.nextLeafCopyId))
		{//添加
			SaveBattleInfo();
		}
		stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(pLeafConfig->iDefaultRewardID);
		OnReward(pRewardGroup, 5);//5 副本奖励
	}
}

void CUser::Login(uint32_t checkNo, stRoleInfo roleInfo)
{
	//fill data
	m_nCheckNo = checkNo;
	memcpy(&m_roleInfo, &roleInfo, sizeof(roleInfo));
	
	g_UserMgr->AddUser(this);
}

void CUser::SendRoleInfo()
{
	if (!m_pHandler)
	{
		return;
	}
	printf("CUser::SendRoleInfo\n");
	//发送角色信息
	char buf2[BUF_SIZE1];
	stS2CMsgRoleInfo* pRoleInfo = (stS2CMsgRoleInfo*)(buf2);
	pRoleInfo->cSysIdentifer = INMSG_GAME_CLIENT;
	pRoleInfo->cMsgID = eMsg_S2C_Role_Info;
	pRoleInfo->exp = m_roleInfo.exp;
	pRoleInfo->level = m_roleInfo.level;
	pRoleInfo->coin = m_roleInfo.coin;
	pRoleInfo->gold = m_roleInfo.gold;
	pRoleInfo->vitality = m_roleInfo.vitality;
	pRoleInfo->ability = m_roleInfo.ability;
	pRoleInfo->psychicPower = m_roleInfo.psychicPower;
	pRoleInfo->honor = m_roleInfo.honor;
	printf("CUser::SendRoleInfo honor=%u\n",pRoleInfo->honor);
	pRoleInfo->curTask = m_roleInfo.curTask;
	pRoleInfo->leftChallengeTimes = m_roleInfo.leftChallengeTimes;
	pRoleInfo->wonTimes = m_roleInfo.wonTimes;
	pRoleInfo->challengeWonTimes = m_roleInfo.challengeWonTimes;
	pRoleInfo->oldTopId = m_roleInfo.oldTopId;
	pRoleInfo->newTopId = m_roleInfo.newTopId;
	//pRoleInfo->compDoubleWonTimes = m_roleInfo.compDoubleWonTimes;
	//pRoleInfo->competitionTimes = m_roleInfo.competitionTimes;
	//pRoleInfo->attackTimes = m_roleInfo.attackTimes;
	//pRoleInfo->defenceTimes = m_roleInfo.defenceTimes;
	//pRoleInfo->attackWonTimes = m_roleInfo.attackWonTimes;
	//pRoleInfo->defenceWonTimes = m_roleInfo.defenceWonTimes;
	pRoleInfo->ltAttack = m_roleInfo.ltAttack;
	pRoleInfo->utAttack = m_roleInfo.utAttack;
	pRoleInfo->ltDefence = m_roleInfo.ltDefence;
	pRoleInfo->utDefence = m_roleInfo.utDefence;
	pRoleInfo->nRoleNameLen = strlen(m_roleInfo.szName);
	pRoleInfo->pRoleName = NULL;
	int len2 = sizeof(stS2CMsgRoleInfo);
	memcpy(buf2+len2, m_roleInfo.szName, pRoleInfo->nRoleNameLen);
	len2 += pRoleInfo->nRoleNameLen;
	printf("rome Name: %s\n", m_roleInfo.szName);
	m_pHandler->Send( buf2, len2);
}

void CUser::Login(t_ROLE_INFO roleInfo)
{
	m_ItemMgr.SetUser(this);
	m_RelationMgr.SetUser(this);
	m_ChallengeMgr.SetUser(this);
	m_ChallTaskMgr.SetUser(this);
	//fill data
	memcpy(&m_roleInfo, &roleInfo, sizeof(roleInfo));

	stLevelData* pLevelData = CLevelDataManager::ShardLevelDataMgr()->GetLevelData(m_roleInfo.level);
	m_curLevelInfo.maxActive = pLevelData->nMaxActive;
	m_curLevelInfo.actvieRestoreSpeed = pLevelData->nActiveRestorePerSec;
	m_curLevelInfo.attackLow = pLevelData->nAttackLow;
	m_curLevelInfo.attackUp = pLevelData->nAttackUp;
	m_curLevelInfo.defendLow = pLevelData->nDefendLow;
	m_curLevelInfo.defendUp = pLevelData->nDefendUp;
	//计算攻防数据
	//CalculateRole();	
	if(-1 == g_UserMgr->AddUser(this))
	{
		//断线重连
		printf("recontion XXXXXXXXXXXX!\n");
	}
}

unsigned int CUser::GetGeneralAttack(int index)
{
	if (m_warGeneral[index].utAttack <= m_warGeneral[index].ltAttack)
	{
		return m_warGeneral[index].utAttack;
	}
	unsigned int sz = m_warGeneral[index].utAttack - m_warGeneral[index].ltAttack;
	return (unsigned int)(rand() % (sz+1) + m_warGeneral[index].ltAttack);
}

unsigned int CUser::GetGeneralDefence(int index)
{
	if (m_warGeneral[index].utDefence <= m_warGeneral[index].ltDefence)
	{
		return m_warGeneral[index].utDefence;
	}
	unsigned int sz = m_warGeneral[index].utDefence - m_warGeneral[index].ltDefence;
	return (unsigned int)(rand() % (sz+1) + m_warGeneral[index].ltDefence);
}

unsigned int CUser::GetLevelAttack()
{
	if (m_curLevelInfo.attackUp <= m_curLevelInfo.attackLow)
	{
		return m_curLevelInfo.attackUp;
	}
	unsigned int sz = m_curLevelInfo.attackUp - m_curLevelInfo.attackLow;
	return (unsigned int)(rand() % (sz+1) + m_curLevelInfo.attackLow);
}

unsigned int CUser::GetLevelDefence()
{
	if (m_curLevelInfo.defendUp <= m_curLevelInfo.defendLow)
	{
		return m_curLevelInfo.defendUp;
	}
	unsigned int sz = m_curLevelInfo.defendUp - m_curLevelInfo.defendLow;
	return (unsigned int)(rand() % (sz+1) + m_curLevelInfo.defendLow);
}

bool CUser::CalculateBasic(unsigned char posIndex)
{
	stGodCard* pGenStatic = GetGodStaticInfo(m_warGeneral[posIndex].guid);
	if (!pGenStatic)
	{
		return false;
	}
	m_warGeneral[posIndex].ltAttack = pGenStatic->nAttackLow;
	m_warGeneral[posIndex].utAttack = pGenStatic->nAttackUp;
	m_warGeneral[posIndex].ltDefence = pGenStatic->nDefendLow;
	m_warGeneral[posIndex].utDefence = pGenStatic->nDefendUp;

	m_warGeneral[posIndex].ability = pGenStatic->nAbility;
	//副将
	//int pos = 0;
	stGodCard* pStatic = NULL;
	for(int i=0; i < 3; i++)
	{
		if(m_warGeneral[posIndex].adjutantGuid[i] > 0)
		{
			pStatic = GetGodStaticInfo(m_warGeneral[posIndex].adjutantGuid[i]);
			assert(pStatic);
			m_warGeneral[posIndex].ltAttack += pStatic->nAttackLow;
			m_warGeneral[posIndex].utAttack += pStatic->nAttackUp;
			m_warGeneral[posIndex].ltDefence += pStatic->nDefendLow;
			m_warGeneral[posIndex].utDefence += pStatic->nDefendUp;
			m_warGeneral[posIndex].ability += pStatic->nAbility;
		}
	}
	//装备
	CEquip* pEquip = NULL;
	if (m_warGeneral[posIndex].weaponGuid > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[posIndex].weaponNo);
		if (pEquip)
		{
			CalculateEquip(pEquip, pGenStatic->IsSpecailEquip(pEquip->GetId()));
		}
	}

	if (m_warGeneral[posIndex].armorGuid > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[posIndex].armorNo);
		if (pEquip)
		{
			CalculateEquip(pEquip, pGenStatic->IsSpecailEquip(pEquip->GetId()));
		}
	}
	if (m_warGeneral[posIndex].treasureGuid > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[posIndex].treasureNo);
		if (pEquip)
		{
			CalculateEquip(pEquip, pGenStatic->IsSpecailEquip(pEquip->GetId()));
		}
	}
	m_warGeneral[posIndex].ltBasicAttack = m_warGeneral[posIndex].ltAttack;
	m_warGeneral[posIndex].utBasicAttack = m_warGeneral[posIndex].utAttack;
	m_warGeneral[posIndex].ltBasicDefence = m_warGeneral[posIndex].ltDefence;
	m_warGeneral[posIndex].utBasicDefence = m_warGeneral[posIndex].utDefence;
	memset(m_warGeneral[posIndex].combSkillFlag, 0, sizeof(m_warGeneral[posIndex].combSkillFlag));
	
	return true;
}

void CUser::CalculateRole()
{
	printf("CUser::CalculateRole(%u)\n", m_roleInfo.userId);
	//角色等级攻防数据
	m_roleInfo.ltAttack = m_curLevelInfo.attackLow;
	m_roleInfo.utAttack = m_curLevelInfo.attackUp;
	m_roleInfo.ltDefence = m_curLevelInfo.defendLow;
	m_roleInfo.utDefence = m_curLevelInfo.defendUp;
	//重置组合技标识
	for(int i=0; i < WAR_POS_MAX; i++)
	{
		memset(m_warGeneral[i].combSkillFlag, 0, sizeof(m_warGeneral[i].combSkillFlag));
	}
	//武将攻防数据
	for(int i=0; i < WAR_POS_MAX; i++)
	{
		if(m_warGeneral[i].guid > 0)
		{
			Calculate(i+1, true);
			m_roleInfo.ltAttack += m_warGeneral[i].ltAttack;
			m_roleInfo.utAttack += m_warGeneral[i].utAttack;
			m_roleInfo.ltDefence += m_warGeneral[i].ltDefence;
			m_roleInfo.utDefence += m_warGeneral[i].utDefence;
		}
	}
}

void CUser::Calculate(CGeneral* pGen, bool bNeedSendFlag)
{
	Calculate(pGen->m_info.warFlag, bNeedSendFlag);
}

void CUser::Calculate(unsigned char warPos, bool bNeedSendFlag)
{
	//武将自身
	int posIndex = warPos-1;
	if (posIndex < 0)
	{
		return;
	}
	if(!CalculateBasic(posIndex))
	{
		return;
	}

	bool bSendFlag = false;
	for(int i=0; i<WAR_POS_MAX; i++)
	{
		if (m_warGeneral[i].guid<=0)
		{
			continue;
		}
		stGodCard* pGod = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[i].guid);
		for(int j=0; j<MAX_COMB_SKILL; j++)
		{
			if (pGod->nCombSkill[j]<=0)
			{
				continue;
			}
			stCombSkill* pComSkill = (stCombSkill*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(pGod->nCombSkill[j]);
			for (int k=0; k<WAR_POS_MAX;k++)
			{
				if(m_warGeneral[k].guid <= 0)
				{
					continue;
				}
				stGodCard* pTmpGod = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[k].guid);
				if (pComSkill->CanInvoke(pTmpGod->nID))// && 0 == m_warGeneral[i].combSkillFlag[j])
				{
					if (pTmpGod->nID != pGod->nID && 0 == m_warGeneral[i].combSkillFlag[j])
					{//普通组合技 自己加成
						m_warGeneral[i].combSkillFlag[j] = 1;	//激活
						m_warGeneral[i].ltAttack += (unsigned int)( m_warGeneral[i].ltBasicAttack * (pComSkill->fAttack) );
						m_warGeneral[i].utAttack += (unsigned int)( m_warGeneral[i].utBasicAttack * (pComSkill->fAttack) );
						m_warGeneral[i].ltDefence += (unsigned int)( m_warGeneral[i].ltBasicDefence * (pComSkill->fDefend) );
						m_warGeneral[i].utDefence += (unsigned int)( m_warGeneral[i].utBasicDefence * (pComSkill->fDefend) );
					}
					else if(pTmpGod->nID == pGod->nID )//&& 0 == m_warGeneral[i].combSkillFlag[MAX_COMB_SKILL+i])
					{//五星武将 组合技作用于整个阵营
						for(int ii = 0; ii < WAR_POS_MAX; ii++)
						{
							stGodCard* pGodConfig = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[ii].guid);
							if (pGodConfig && pGodConfig->eGroup == pTmpGod->eGroup && 0 == m_warGeneral[ii].combSkillFlag[MAX_COMB_SKILL+i])
							{
								if (i == ii && 1 == m_warGeneral[i].combSkillFlag[j])
								{
									continue;
								}
								if (i == ii)
								{
									m_warGeneral[i].combSkillFlag[j] = 1;
								}
								else
								{
									m_warGeneral[ii].combSkillFlag[MAX_COMB_SKILL+i] = 1;
								}
								m_warGeneral[ii].ltAttack += (unsigned int)( m_warGeneral[ii].ltBasicAttack * (pComSkill->fAttack) );
								m_warGeneral[ii].utAttack += (unsigned int)( m_warGeneral[ii].utBasicAttack * (pComSkill->fAttack) );
								m_warGeneral[ii].ltDefence += (unsigned int)( m_warGeneral[ii].ltBasicDefence * (pComSkill->fDefend) );
								m_warGeneral[ii].utDefence += (unsigned int)( m_warGeneral[ii].utBasicDefence * (pComSkill->fDefend) );
							}
						}
					}
					bSendFlag = true;
				}
			}
		}
	}

	//CalculateRole();
	if (bNeedSendFlag && bSendFlag)
	{
		SendWarGenInfo();
	}
}

void CUser::CalculateEquip(CEquip* pEquip, bool bAddFlag)
{
	assert(pEquip->m_info.warFlag > 0);
	int posIndex = pEquip->m_info.warFlag-1;
	stEquip* pEquipStatic = GetEquipStaticInfo(pEquip->GetGuid());
	stGodCard* pGenStatic = GetGodStaticInfo(m_warGeneral[posIndex].guid);
	assert(pEquipStatic);
	m_warGeneral[posIndex].ability += pEquipStatic->nAbility;
	if(!pGenStatic) 
	{
		return;
	}
	int factor = 1;
	if (bAddFlag)
	{
		factor = 2;
	}
	if (pEquipStatic->nAttackUp.bFloat)
	{
		m_warGeneral[posIndex].utAttack += (unsigned int)(pEquipStatic->nAttackUp.Value.fValue * pGenStatic->nAttackUp) * factor;
	}
	else
	{
		m_warGeneral[posIndex].utAttack += pEquipStatic->nAttackUp.Value.nValue * factor;
	}
	if (pEquipStatic->nAttackLow.bFloat)
	{
		m_warGeneral[posIndex].ltAttack += (unsigned int)(pEquipStatic->nAttackLow.Value.fValue * pGenStatic->nAttackLow) * factor;
	}
	else
	{
		m_warGeneral[posIndex].ltAttack += pEquipStatic->nAttackLow.Value.nValue * factor;
	}
	if (pEquipStatic->nDefendLow.bFloat)
	{
		m_warGeneral[posIndex].ltDefence += (unsigned int)(pEquipStatic->nDefendLow.Value.fValue * pGenStatic->nDefendLow) * factor;
	}
	else
	{
		m_warGeneral[posIndex].ltDefence += pEquipStatic->nDefendLow.Value.nValue * factor;
	}
	if (pEquipStatic->nDefendUp.bFloat)
	{
		m_warGeneral[posIndex].utDefence += (unsigned int)(pEquipStatic->nDefendUp.Value.fValue * pGenStatic->nDefendUp) * factor;
	}
	else
	{
		m_warGeneral[posIndex].utDefence += pEquipStatic->nDefendUp.Value.nValue * factor;
	}
}

void CUser::Logout()
{
	printf("CUser::Logout\n");
	m_roleInfo.lastActiveTime = getSecTime();
	SaveData();
	//g_UserMgr->DelUser(m_roleInfo.userId);
}

void CUser::OnDisconnect()
{
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printf("CUser::OnDisconnect\n");
	Logout();
}

short CUser::CopyGeneralInfo(unsigned int roleId, char* buf, short bufLen)
{
	return m_ItemMgr.CopyGeneralInfo(roleId, buf, bufLen);
}

void CUser::SaveRoleBase()
{
	//printf("CUser::SaveRoleBase\n");
	char buf[BUF_SIZE1];
	stG2D_SaveRoleBase* p = (stG2D_SaveRoleBase*)buf;
	p->cSysIdentifer = INMSG_GAME_DB;
	p->cMsgID = G2D_SAVE_ROLE_BASE;
	p->cmdType = G2D_SAVE_ROLE_BASE;
	Lock();
	p->role.userId = m_roleInfo.userId;
	p->role.roleNameLen = strlen(m_roleInfo.szName);
	p->role.pRoleName = NULL;
	p->role.exp = m_roleInfo.exp;
	p->role.level = m_roleInfo.level;
	p->role.coin = m_roleInfo.coin;
	p->role.gold = m_roleInfo.gold;
	p->role.vitality = m_roleInfo.vitality;
	p->role.ability = m_roleInfo.ability;
	p->role.psychicPower = m_roleInfo.psychicPower;
	p->role.honor = m_roleInfo.honor;
	p->role.curTask = m_roleInfo.curTask;
	printf("CUser::SaveRoleBase curTask=%u\n", p->role.curTask);
	p->role.leftChallengeTimes = m_roleInfo.leftChallengeTimes;
	p->role.wonTimes = m_roleInfo.wonTimes;
	p->role.challengeWonTimes = m_roleInfo.challengeWonTimes;
	//p->role.compDoubleWonTimes = m_roleInfo.compDoubleWonTimes;
	m_bChangeFlag = false;
	UnLock();
	//role name
	int len = sizeof(stG2D_SaveRoleBase);
	memcpy(buf+len, m_roleInfo.szName, p->role.roleNameLen);
	len += p->role.roleNameLen;
	StartServer->SendToDB( buf, len);
}

void CUser::SaveOneGenInfo(unsigned int godNo)
{
	//
}

void CUser::SaveGeneralInfo()
{
	//printf("CUser::SaveGeneralInfo\n");
	char buf[BUF_SIZE2];
	stG2D_SaveGeneral* pMsg = (stG2D_SaveGeneral*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_GENERAL;
	pMsg->cmdType = G2D_SAVE_GENERAL;
	pMsg->pGeneral = NULL;
	int len = sizeof(stG2D_SaveGeneral);
	pMsg->count = CopyGeneralInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stGeneralInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveEquipInfo()
{
	//printf("CUser::SaveEquipInfo\n");
	char buf[BUF_SIZE2];
	stG2D_SaveEquip* pMsg = (stG2D_SaveEquip*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_EQUIP;
	pMsg->cmdType = G2D_SAVE_EQUIP;
	pMsg->pList = NULL;
	int len = sizeof(stG2D_SaveEquip);
	pMsg->count = m_ItemMgr.CopyEquipInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stEquipInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveRelationInfo()
{
	//printf("CUser::SaveRelationInfo\n");
	char buf[BUF_SIZE2];
	stG2D_SaveRelation* pMsg = (stG2D_SaveRelation*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_RELATION;
	pMsg->cmdType = G2D_SAVE_RELATION;
	pMsg->pList = NULL;
	pMsg->userId = m_roleInfo.userId;
	int len = sizeof(stG2D_SaveRelation);
	pMsg->count = m_RelationMgr.CopyRelationInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stRelationInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveShopHistory()
{
	//printf("CUser::SaveShopHistory\n");
	char buf[BUF_SIZE2];
	stG2D_SaveShopHistory* pMsg = (stG2D_SaveShopHistory*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_SHOP_HISTORY;
	pMsg->cmdType = G2D_SAVE_SHOP_HISTORY;
	pMsg->pList = NULL;
	pMsg->userId = m_roleInfo.userId;
	int len = sizeof(stG2D_SaveShopHistory);
	pMsg->count = m_ShopHistoryMgr.CopyShopHistoryInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stShopHistoryInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveChallengeHistory()
{
	char buf[BUF_SIZE2];
	stG2D_SaveChallenge* pMsg = (stG2D_SaveChallenge*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_CHALLENGE_INFO;
	pMsg->cmdType = G2D_SAVE_CHALLENGE_INFO;
	pMsg->pList = NULL;
	pMsg->userId = m_roleInfo.userId;
	int len = sizeof(stG2D_SaveChallenge);
	pMsg->count = m_ChallengeMgr.CopyChallengeInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stChallengeInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveChallTask()
{
	//printf("CUser::SaveChallTask\n");
	char buf[BUF_SIZE2];
	stG2D_SaveChallTask* pMsg = (stG2D_SaveChallTask*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_CHALL_TASK_INFO;
	pMsg->cmdType = G2D_SAVE_CHALL_TASK_INFO;
	pMsg->pList = NULL;
	pMsg->userId = m_roleInfo.userId;
	int len = sizeof(stG2D_SaveChallTask);
	pMsg->count = m_ChallTaskMgr.CopyChallTaskInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stChallTaskInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveBattleInfo()
{
	//printf("CUser::SaveBattleInfo\n");
	char buf[BUF_SIZE2];
	stG2D_SaveBattle* pMsg = (stG2D_SaveBattle*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_BATTLE_INFO;
	pMsg->cmdType = G2D_SAVE_BATTLE_INFO;
	pMsg->pList = NULL;
	pMsg->userId = m_roleInfo.userId;
	int len = sizeof(stG2D_SaveBattle);
	pMsg->count = m_battleMgr.CopyBattleInfo(buf+len, BUF_SIZE2-len);
	len += sizeof(stBattleInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveCommItem()
{
	//printf("CUser::SaveCommItem\n");
	char buf[BUF_SIZE2];
	stG2D_SaveItem* pMsg = (stG2D_SaveItem*)buf;
	pMsg->cSysIdentifer = INMSG_GAME_DB;
	pMsg->cMsgID = G2D_SAVE_ITEM;
	pMsg->cmdType = G2D_SAVE_ITEM;
	pMsg->pList = NULL;
	int len = sizeof(stG2D_SaveItem);
	pMsg->count = m_ItemMgr.CopyItemInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
	len += sizeof(stItemInfo)*(pMsg->count);
	if (pMsg->count > 0)
	{
		StartServer->SendToDB( buf, len);
	}
}

void CUser::SaveData()
{
	//printf("CUser::SaveData\n");
	Lock();
	if (m_bChangeFlag)
	{
		char buf[BUF_SIZE2];
		stG2D_SaveRole* p = (stG2D_SaveRole*)buf;
		p->cSysIdentifer = INMSG_GAME_DB;
		p->cMsgID = G2D_SAVE_ROLE;
		p->cmdType = G2D_SAVE_ROLE;
		//int len = sizeof(stServerMsg);
		//role info
		p->role.userId = m_roleInfo.userId;
		p->role.roleNameLen = strlen(m_roleInfo.szName);
		p->role.pRoleName = NULL;
		p->role.exp = m_roleInfo.exp;
		p->role.level = m_roleInfo.level;
		p->role.coin = m_roleInfo.coin;
		p->role.gold = m_roleInfo.gold;
		p->role.vitality = m_roleInfo.vitality;
		p->role.ability = m_roleInfo.ability;
		p->role.psychicPower = m_roleInfo.psychicPower;
		p->role.honor = m_roleInfo.honor;
		p->role.curTask = m_roleInfo.curTask;
		p->role.leftChallengeTimes = m_roleInfo.leftChallengeTimes;
		p->role.wonTimes = m_roleInfo.wonTimes;
		p->role.challengeWonTimes = m_roleInfo.challengeWonTimes;
		//p->role.compDoubleWonTimes = m_roleInfo.compDoubleWonTimes;
		p->role.lastHonorExcTime = m_roleInfo.lastHonorExcTime;
		p->role.lastActiveTime = m_roleInfo.lastActiveTime;
		m_bChangeFlag = false;
		UnLock();
		//len += sizeof(stRoleInfo);
		//memcpy(buf+len, m_roleInfo.szName, p->role.roleNameLen);
		//len += p->role.roleNameLen;
		//task info
		//short taskCount = 0;
		//len += sizeof(p->taskCount);
		//role name
		int len = sizeof(stG2D_SaveRole);
		memcpy(buf+len, m_roleInfo.szName, p->role.roleNameLen);
		len += p->role.roleNameLen;
		//task info
		p->taskCount = m_task.CopyTaskInfo(m_roleInfo.userId, buf+len, BUF_SIZE2-len);
		len += sizeof(stTaskInfo)*(p->taskCount);
		StartServer->SendToDB( buf, len);
	}
	else
	{
		UnLock();
	}
	
	//general info
	SaveGeneralInfo();
	//equip info
	SaveEquipInfo();
	//CommItem info
	SaveCommItem();
	//relation info
	SaveRelationInfo();
	//shop history
	SaveShopHistory();
	//
	SaveChallengeHistory();
	SaveChallTask();
	SaveBattleInfo();
}

void CUser::ChallTaskUpdate()
{
	m_ChallTaskMgr.Reset();
}

void CUser::OnBattleReset()
{
	//printf("CUser::OnBattleReset()\n");
	m_battleMgr.Reset();
	if(m_pHandler)
	{
		stS2CMsgCopyReset msg;
		msg.nErr = 0;
		msg.mainId = 0;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
	}
}

void CUser::OnBattleReset(unsigned short mainId)
{
	//printf("CUser::OnBattleReset\n");
	stS2CMsgCopyReset msg;
	stMainMissionConfig* pMainTask = CMissionMgr::SharedMissionMgr()->GetMainMission(mainId, eMission_Battle);
	if(!pMainTask)
	{
		msg.nErr = 3;
		m_pHandler->Send( (char*)(&msg), sizeof(msg) );
		return;
	}
	if ( m_ItemMgr.GetMaterial(BATTLE_RESET_ITEM)->GetCount() < (unsigned int)(pMainTask->iRestItemCountNeed) )
	{
		msg.nErr = 2;
		m_pHandler->Send( (char*)(&msg), sizeof(msg) );
		return;
	}
	m_ItemMgr.ReduceCommItem(BATTLE_RESET_ITEM, pMainTask->iRestItemCountNeed);
	m_battleMgr.Reset();
	msg.nErr = 1;
	msg.mainId = mainId;
	m_pHandler->Send( (char*)(&msg), sizeof(msg) );
}

void CUser::OnGodList()
{
	//printf("CUser::OnGodList\n");
	char buf[BUF_SIZE2];
	memset(buf, 0, BUF_SIZE2);
	stS2CMsgGodList* pMsg = (stS2CMsgGodList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GodList;
	pMsg->pGodList = NULL;
	int len = sizeof(stS2CMsgGodList);
	unsigned short count;
	len += m_ItemMgr.FillGeneralInfo(buf+len, &count);
	pMsg->count = count;
	m_pHandler->Send( buf, len);
}

void CUser::OnEquipList()
{
	printf("CUser::OnEquipList\n");
	char buf[BUF_SIZE2];
	memset(buf, 0, BUF_SIZE2);
	stS2CMsgEquipList* pMsg = (stS2CMsgEquipList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_EquipList;
	pMsg->pEquipList = NULL;
	int len = sizeof(stS2CMsgEquipList);
	unsigned short count;
	len += m_ItemMgr.FillEquipInfo(buf+len, &count);
	pMsg->count = count;
	m_pHandler->Send( buf, len);
}

void CUser::OnItemList()
{
	if (!m_pHandler)
	{
		return;
	}
	printf("CUser::OnItemList\n");
	char buf[BUF_SIZE2];
	memset(buf, 0, BUF_SIZE2);
	stS2CMsgItemList* pMsg = (stS2CMsgItemList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_ItemList;
	pMsg->pItemList = NULL;
	int len = sizeof(stS2CMsgItemList);
	unsigned short count;
	len += m_ItemMgr.FillItemInfo(buf+len, &count);
	pMsg->count = count;
	m_pHandler->Send( buf, len);
}

void CUser::FillWarGodInfo(unsigned char warPos, t_WarGodInfo* pWarGod)
{
	int index = warPos-1;
	pWarGod->warPos = m_warGeneral[index].warPos;
	pWarGod->serialNo = m_warGeneral[index].serialNo;
	pWarGod->guid = m_warGeneral[index].guid;				
	pWarGod->adjutant[0] = m_warGeneral[index].adjutantGuid[0];
	pWarGod->adjutant[1] = m_warGeneral[index].adjutantGuid[1];
	pWarGod->adjutant[2] = m_warGeneral[index].adjutantGuid[2];
	pWarGod->adjutantNo[0] = m_warGeneral[index].adjutantNo[0];
	pWarGod->adjutantNo[1] = m_warGeneral[index].adjutantNo[1];
	pWarGod->adjutantNo[2] = m_warGeneral[index].adjutantNo[2];
	pWarGod->weapon = m_warGeneral[index].weaponGuid;	
	pWarGod->weaponNo = m_warGeneral[index].weaponNo;
	pWarGod->armor = m_warGeneral[index].armorGuid;
	pWarGod->armorNo = m_warGeneral[index].armorNo;
	pWarGod->treasure = m_warGeneral[index].treasureGuid;
	pWarGod->treasureNo = m_warGeneral[index].treasureNo;
	pWarGod->combSkill[0] = m_warGeneral[index].combSkillFlag[0];
	pWarGod->combSkill[1] = m_warGeneral[index].combSkillFlag[1];
	pWarGod->combSkill[2] = m_warGeneral[index].combSkillFlag[2];
	pWarGod->combSkill[3] = m_warGeneral[index].combSkillFlag[3];
	pWarGod->godSkill[0] = m_warGeneral[index].godSkill[0];
	pWarGod->godSkill[1] = m_warGeneral[index].godSkill[1];
	pWarGod->godSkill[2] = m_warGeneral[index].godSkill[2];
	pWarGod->godSkill[3] = m_warGeneral[index].godSkill[3];
	pWarGod->ltAttack = m_warGeneral[index].ltAttack;// > m_warGeneral[index].utAttack ? m_warGeneral[index].utAttack : m_warGeneral[index].ltAttack;
	pWarGod->utAttack = m_warGeneral[index].utAttack;
	pWarGod->ltDefence = m_warGeneral[index].ltDefence;// > m_warGeneral[index].utDefence ? m_warGeneral[index].utDefence : m_warGeneral[index].ltDefence;
	pWarGod->utDefence = m_warGeneral[index].utDefence;
}

void CUser::GenWar(CGeneral* pGen)
{
	int pos = pGen->m_info.warFlag - 1;
	m_warGeneral[pos].serialNo = pGen->GetSerialNo();
	m_warGeneral[pos].roleId = m_roleInfo.userId;
	m_warGeneral[pos].warPos = pos+1;
	m_warGeneral[pos].guid = pGen->GetGuid();
	m_warGeneral[pos].godSkill[0] = pGen->m_info.godSkill;
	/*stGodCard* pInfo = GetGodStaticInfo(m_warGeneral[pos].guid);
	assert(pInfo);
	m_warGeneral[pos].ltAttack = pInfo->nAttackLow;
	m_warGeneral[pos].utAttack = pInfo->nAttackUp;
	m_warGeneral[pos].ltDefence = pInfo->nDefendLow;
	m_warGeneral[pos].utDefence = pInfo->nDefendUp;*/
}

void CUser::EquipWar(CEquip* pEquip)
{
	int pos = pEquip->m_info.warFlag - 1;
	int type = pEquip->GetEquipType();
	if (eEquip_Weapon == type)
	{
		m_warGeneral[pos].weaponGuid = pEquip->GetGuid();
		m_warGeneral[pos].weaponNo = pEquip->GetSerialNo();
	} 
	else if(eEquip_Defend == type)
	{
		m_warGeneral[pos].armorGuid = pEquip->GetGuid();
		m_warGeneral[pos].armorNo = pEquip->GetSerialNo();
	}
	else
	{
		m_warGeneral[pos].treasureGuid = pEquip->GetGuid();
		m_warGeneral[pos].treasureNo = pEquip->GetSerialNo();
	}
}

void CUser::AdjutantWar(CGeneral* pGen, int mainPos, int pos)
{
	//assert(mainPos == pGen->m_info.warFlag);
	m_warGeneral[mainPos-1].adjutantGuid[pos-1] = pGen->GetGuid();
	m_warGeneral[mainPos-1].adjutantNo[pos-1] = pGen->GetSerialNo();
	m_warGeneral[mainPos-1].godSkill[pos] = pGen->m_info.godSkill;
}

void CUser::GenBack(CGeneral* pGen)
{
	int index = pGen->m_info.warFlag - 1;
	assert(index>=0);
	//stGodCard* pTmpGod = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[index].guid);
	//卸载该武将的装备
	CEquip* pEquip = NULL;
	if (m_warGeneral[index].weaponNo > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[index].weaponNo);
		pEquip->m_info.warFlag = 0;
		pEquip->SetChangeFlag();
	}
	if (m_warGeneral[index].armorNo > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[index].armorNo);
		pEquip->m_info.warFlag = 0;
		pEquip->SetChangeFlag();
	}
	if (m_warGeneral[index].treasureNo > 0)
	{
		pEquip = m_ItemMgr.GetEquip(m_warGeneral[index].treasureNo);
		pEquip->m_info.warFlag = 0;
		pEquip->SetChangeFlag();
	}
	memset(&(m_warGeneral[index]), 0, sizeof(t_WarGeneralInfo));
	//判断是否影响组合技
	/*bool bChangeFlag = false;
	for(int i=0; i<WAR_POS_MAX; i++)
	{
		if (m_warGeneral[i].guid<=0)
		{
			continue;
		}
		for (int j=0; j<MAX_COMB_SKILL; j++)
		{
			stGodCard* pGod = CItemManager::SharedItemMgr()->GetGodCardByUID(m_warGeneral[i].guid);
			stCombSkill* pComSkill = (stCombSkill*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(pGod->nCombSkill[j]);
			if (pComSkill && pComSkill->CanInvoke(pTmpGod->nID))
			{
				if (i != index && 1 == m_warGeneral[i].combSkillFlag[j])
				{//普通组合技 
					m_warGeneral[i].combSkillFlag[j] = 0;
					Calculate(i+1);
				}
				else if(i == index)
				{//五星武将 组合技作用于整个阵营
					for(int k = 0; k < WAR_POS_MAX; k++)
					{
						if (1 == m_warGeneral[k].combSkillFlag[MAX_COMB_SKILL+index])
						{
							m_warGeneral[k].combSkillFlag[MAX_COMB_SKILL+index] = 0;
							Calculate(k+1);
						}
					}
				}
				bChangeFlag = true;
			}
		}
	}
	if (bChangeFlag)
	{
		SendWarGenInfo();
	}*/
}

void CUser::EquipBack(CEquip* pEquip)
{
	int pos = pEquip->m_info.warFlag;
	assert(pos>0);
	if (eEquip_Weapon == pEquip->GetEquipType())
	{
		m_warGeneral[pos-1].weaponGuid = 0;
		m_warGeneral[pos-1].weaponNo = 0;
	} 
	else if(eEquip_Defend == pEquip->GetEquipType())
	{
		m_warGeneral[pos-1].armorGuid = 0;
		m_warGeneral[pos-1].armorNo = 0;
	}
	else
	{
		m_warGeneral[pos-1].treasureGuid = 0;
		m_warGeneral[pos-1].treasureNo = 0;
	}
}

/*void CUser::GenBack(int pos)
{
	assert(pos>0);
	memset(&(m_warGeneral[pos-1]), 0, sizeof(t_WarGeneralInfo));
}*/

void CUser::OnAdjutantPlay(unsigned int serialNo, unsigned char warPos)
{
	printf("CUser::OnAdjutantPlay\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgGodPlay* pMsg = (stS2CMsgGodPlay*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GodPlay;
	
	pMsg->nErr = 0;
	int len = sizeof(stS2CMsgGodPlay);
	
	int mainPos, pos;
	if( GetAdjutantPos(warPos, &mainPos, &pos) )
	{
		AdjutantPlay(serialNo, mainPos, pos);
	}
	else
	{
		pMsg->nErr = 2;	//
		m_pHandler->Send( buf, len);
		return;
	}
}

void CUser::AdjutantPlay(unsigned int serialNo, int mainPos, int pos)
{
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgGodPlay* pMsg = (stS2CMsgGodPlay*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GodPlay;
	
	pMsg->nErr = 0;
	int len = sizeof(stS2CMsgGodPlay);
	
	unsigned int oldGodGuid = m_warGeneral[mainPos-1].adjutantGuid[pos-1];
	if(oldGodGuid > 0)
	{//替换武将
		assert(0);
		OnAdjutantBack(oldGodGuid, mainPos, pos);
	}
	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen || pGen->m_info.warFlag > 0)
	{
		pMsg->nErr = 1;	//
		m_pHandler->Send( buf, len);
		return;
	}
	pGen->m_info.warFlag = mainPos*10+pos;
	pGen->SetChangeFlag();
	

	//m_warGeneral[mainPos-1].adjutantGuid[pos-1] = pGen->GetGuid();//
	//m_warGeneral[mainPos-1].adjutantNo[pos-1] = pGen->GetSerialNo();
	AdjutantWar(pGen, mainPos, pos);
	CGeneral* pMainGen = m_ItemMgr.GetGeneral(m_warGeneral[mainPos-1].serialNo);
	assert(pMainGen);
	Calculate(pMainGen);
	pMsg->nErr = 0;	//
	FillWarGodInfo(mainPos, &(pMsg->warGodInfo));
	m_pHandler->Send( buf, len);
}

void CUser::OnGeneralPlay(unsigned int serialNo, unsigned char warPos)
{
	//printf("CUser::OnGeneralPlay\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgGodPlay* pMsg = (stS2CMsgGodPlay*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GodPlay;
	
	//pMsg->godGuid = godGuid;
	//pMsg->warPos = warPos;
	int len = sizeof(stS2CMsgGodPlay);
	
	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen || pGen->m_info.warFlag > 0)
	{
		pMsg->nErr = 1;	//
		m_pHandler->Send( buf, len);
		return;
	}
	if(warPos > 10)
	{
		return OnAdjutantPlay(serialNo, warPos);
	}
	else if(warPos>WAR_POS_MAX)
	{
		pMsg->nErr = 2;	//
		m_pHandler->Send( buf, len);
		return;
	}
	pGen->m_info.warFlag = warPos;
	pGen->SetChangeFlag();
	if(m_warGeneral[warPos-1].guid > 0)
	{//替换武将
		OnGeneralBack(m_warGeneral[warPos].serialNo, warPos);
		
		/*CGeneral* pGenOld = m_ItemMgr.GetGeneral(m_warGeneral[warPos].serialNo);
		assert(pGenOld);
		pGenOld->m_info.warFlag = 0;
		pGen->SetChangeFlag();*/
	}
	GenWar(pGen);
	Calculate(warPos, true);
	pMsg->nErr = 0;	//
	FillWarGodInfo(warPos, &(pMsg->warGodInfo));
	m_pHandler->Send( buf, len);
}

void CUser::OnEquipPlay(unsigned int serialNo, unsigned char warPos)
{
	printf("CUser::OnEquipPlay\n");
	stS2CMsgEquipPlay msg;
	int len = sizeof(stS2CMsgEquipPlay);
	
	CEquip* pEquip = m_ItemMgr.GetEquip(serialNo);
	if(!pEquip)
	{
		msg.nErr = 1;	//
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if(warPos > WAR_POS_MAX)
	{
		msg.nErr = 2;	//
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	pEquip->m_info.warFlag = warPos;
	pEquip->SetChangeFlag();
	EquipWar(pEquip);
	if(m_warGeneral[warPos-1].guid > 0)
	{
		Calculate(warPos);
	}
	
	msg.nErr = 0;	//
	FillWarGodInfo(warPos, &(msg.warGodInfo));
	m_pHandler->Send( (char*)(&msg), len);
}

void CUser::OnGeneralBack(unsigned int serialNo, unsigned char warPos)
{
	//printf("CUser::OnGeneralBack\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgGodBack* pMsg = (stS2CMsgGodBack*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GodBack;
	
	//pMsg->godGuid = godGuid;
	pMsg->warPos = warPos;
	int len = sizeof(stS2CMsgGodBack);
	
	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen)
	{
		pMsg->nErr = 1;	//	
		m_pHandler->Send( buf, len);
		return;
	}
	//副将
	if(warPos > 10)
	{
		int mainPos, pos;
		if( GetAdjutantPos(warPos, &mainPos, &pos) )
		{
			OnAdjutantBack(serialNo, mainPos, pos);
		}
		else
		{
			pMsg->nErr = 2;	//
			m_pHandler->Send( buf, len);
		}
		return;
	}
	//主将
	if(warPos>WAR_POS_MAX || 0 == m_warGeneral[warPos-1].guid || pGen->m_info.warFlag != warPos)
	{
		pMsg->nErr = 2;	//
		m_pHandler->Send( buf, len);
		return;
	}
	//卸载其所有副将
	for(int i=0; i<MAX_AJUTANT; i++)
	{
		if (m_warGeneral[warPos-1].adjutantGuid[i]<=0)
		{
			continue;
		}
		CGeneral* pAdj = m_ItemMgr.GetGeneral(m_warGeneral[warPos-1].adjutantNo[i]);
		if(pAdj)
		{
			pAdj->m_info.warFlag = 0;
			pAdj->SetChangeFlag();
		}
		else
		{
			assert(0);
		}
		m_warGeneral[warPos-1].adjutantGuid[i] = 0;
		m_warGeneral[warPos-1].adjutantNo[i] = 0;
	}

	GenBack(pGen);
	pGen->m_info.warFlag = 0;
	pGen->SetChangeFlag();

	CalculateRole();//
	pMsg->nErr = 0;	//
	m_pHandler->Send( buf, len);
}

void CUser::OnEquipBack(unsigned int serialNo, unsigned char warPos)
{
	printf("CUser::OnEquipBack\n");
	stS2CMsgEquipBack msg;
	int len = sizeof(stS2CMsgEquipBack);

	CEquip* pEquip = m_ItemMgr.GetEquip(serialNo);
	if(!pEquip)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if(warPos > WAR_POS_MAX)
	{
		msg.nErr = 2;	//
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	EquipBack(pEquip);
	pEquip->m_info.warFlag = 0;
	pEquip->SetChangeFlag();
	if(m_warGeneral[warPos-1].guid > 0)
	{
		Calculate(warPos);
	}

	msg.nErr = 0;	//
	FillWarGodInfo(warPos, &(msg.warGodInfo));
	m_pHandler->Send( (char*)(&msg), len);
}

void CUser::OnAdjutantBack(unsigned int serialNo, int mainPos, int pos)
{
	printf("CUser::OnAdjutantBack\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, BUF_SIZE1);
	stS2CMsgAdjutantBack* pMsg = (stS2CMsgAdjutantBack*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_AdjutantBack;
	
	int len = sizeof(stS2CMsgAdjutantBack);
	
	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen)
	{
		pMsg->nErr = 1;	//	
		m_pHandler->Send( buf, len);
		return;
	}
	{
		m_warGeneral[mainPos-1].adjutantGuid[pos-1] = 0;
		m_warGeneral[mainPos-1].adjutantNo[pos-1] = 0;
		m_warGeneral[mainPos-1].godSkill[pos] = 0;
	}
	pGen->m_info.warFlag = 0;
	pGen->SetChangeFlag();

	Calculate(mainPos);
	pMsg->nErr = 0;	//
	FillWarGodInfo(mainPos, &(pMsg->warGodInfo));
	m_pHandler->Send( buf, len);
}

void CUser::OnGeneralUpLev(unsigned int godGuid, unsigned int serialNo, unsigned int serialNo2)
{
	//printf("CUser::OnGeneralUpLev\n");
	stS2CMsgGodUpLev msg;
	msg.godGuid = 0;
	int len = sizeof(msg);
	
	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stGodCard* pGodStatic = GetGodStaticInfo(godGuid);
	assert(pGodStatic);
	if (pGodStatic->nConfferID > 0)
	{
		return OnGeneralUpStar(godGuid, serialNo, serialNo2);
	}
	if(pGodStatic->nSoulNeededToLevelUp > m_roleInfo.psychicPower)
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if((unsigned int)pGodStatic->nCoinNeedToLevelUp > m_roleInfo.coin)
	{
		msg.nErr = 3;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if(pGodStatic->nNextLevelID > 0)
	{
		msg.nErr = 0;
		msg.oldGodGuid = godGuid;
		msg.oldSerialNo = pGen->GetSerialNo();
		//change
		Lock();
		m_roleInfo.psychicPower -= pGodStatic->nSoulNeededToLevelUp;
		m_bChangeFlag = true;
		UnLock();
		unsigned char warPos = pGen->GetWarFlag();
		unsigned int godSkill = pGen->GetGodSkill();
		pGen->SetDelFlag();
		CGeneral* pNewGen = (CGeneral*)m_ItemMgr.AddNewItem(pGodStatic->nNextLevelID, 1, godSkill);//m_ItemMgr.GetGeneral(g_GodSerialNoBegin);
		assert(pNewGen);
		/*(if (godSkill > 0)
		{
			pNewGen->SetGodSkill(godSkill);
		}*/
		int mainPos = warPos;
		pNewGen->m_info.warFlag = warPos;
		if( warPos > 0 && warPos < 10)
		{//出战武将
			GenWar(pNewGen);
			//Calculate(pNewGen);
		}
		else if(warPos > 10)
		{//出战副将
			int pos;
			assert( GetAdjutantPos(warPos, &mainPos, &pos) );
			//CGeneral* pMainGen = m_ItemMgr.GetGeneral(m_warGeneral[mainPos-1].serialNo);
			//assert(pMainGen);
			AdjutantWar(pNewGen, mainPos, pos);
			
		}
		Calculate(mainPos);
		
		msg.godGuid = pNewGen->GetGuid();
		msg.serialNo = pNewGen->GetSerialNo();
		m_pHandler->Send( (char*)(&msg), len);
		//客户端要求顺序
		if(warPos > 0)
		{
			SendOneWarGenInfo(mainPos-1);
		}
	}
	else
	{
		msg.nErr = 4;	//	
		m_pHandler->Send( (char*)(&msg), len);
	}
}

void CUser::OnGeneralUpStar(unsigned int godGuid, unsigned int serialNo, unsigned int serialNo2)
{
	printf("CUser::OnGeneralUpStar\n");
	stS2CMsgGodUpStar msg;
	msg.godGuid = 0;
	int len = sizeof(msg);

	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	CGeneral* pGen2 = m_ItemMgr.GetGeneral(serialNo2);
	if(!pGen || !pGen2)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stGodCard* pGodStatic = GetGodStaticInfo(godGuid);
	assert(pGodStatic);
	stConferGod* pConferGod = GetConferGodStaticInfo(pGodStatic->nConfferID);
	assert(pConferGod);
	if((unsigned int)pConferGod->nCoinNeed > m_roleInfo.coin)
	{
		msg.nErr = 3;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	CCommItem* pNeedItem = m_ItemMgr.GetMaterial(pConferGod->nItemUIDNeed);
	if (!pNeedItem || (unsigned int)pConferGod->nItemCountNeed > pNeedItem->GetCount())
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}

	if (pConferGod->nConferredGodUID)
	{
		Lock();
		m_roleInfo.coin -= pConferGod->nCoinNeed;
		m_bChangeFlag = true;
		UnLock();
		m_ItemMgr.ReduceCommItem(pNeedItem->GetGuid(), pConferGod->nItemCountNeed);

		msg.nErr = 0;
		msg.oldGodGuid = godGuid;
		msg.oldSerialNo = pGen->GetSerialNo();
		msg.oldSerialNo2 = pGen2->GetSerialNo();
		//change
		unsigned char warPos = pGen->GetWarFlag();
		//unsigned int godSkill = pGen->GetGodSkill();
		pGen->SetDelFlag();
		pGen2->SetDelFlag();
		msg.godSkill = pConferGod->RandGodSkill(pGen->m_info.curStar);
		CGeneral* pNewGen = (CGeneral*)m_ItemMgr.AddNewItem(pConferGod->nConferredGodUID, 1, msg.godSkill);//m_ItemMgr.GetGeneral(g_GodSerialNoBegin);
		assert(pNewGen);
		//printf("CUser::OnGeneralUpStar godSkill=%u\n", pNewGen->m_info.godSkill);
		int mainPos = warPos;
		pNewGen->m_info.warFlag = warPos;
		if( warPos > 0 && warPos < 10)
		{//出战武将
			GenWar(pNewGen);
			//Calculate(pNewGen);
		}
		else if(warPos > 10)
		{//出战副将
			int pos;
			assert( GetAdjutantPos(warPos, &mainPos, &pos) );
			//CGeneral* pMainGen = m_ItemMgr.GetGeneral(m_warGeneral[mainPos-1].serialNo);
			//assert(pMainGen);
			AdjutantWar(pNewGen, mainPos, pos);
		}
		Calculate(mainPos);
		msg.godGuid = pNewGen->GetGuid();
		msg.serialNo = pNewGen->GetSerialNo();
		m_pHandler->Send( (char*)(&msg), len);
		if(warPos > 0)
		{
			SendOneWarGenInfo(mainPos-1);
		}
	}
	else
	{
		msg.nErr = 4;	//	
		m_pHandler->Send( (char*)(&msg), len);
	}
}

void CUser::OnGeneralChangeSkill(unsigned int godGuid, unsigned int serialNo)
{
	printf("CUser::OnGeneralChangeSkill\n");
	stS2CMsgChangeSkill msg;
	int len = sizeof(msg);

	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stGodCard* pGodStatic = GetGodStaticInfo(godGuid);
	assert(pGodStatic);
	stUnconferGod* pUnconferGod = GetUnconferGodStaticInfo(pGodStatic->nUnconfferID);
	assert(pUnconferGod);
	if((unsigned int)pUnconferGod->nCoinNeed > m_roleInfo.coin)
	{
		msg.nErr = 3;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	CCommItem* pNeedItem = m_ItemMgr.GetMaterial(pUnconferGod->nItemUIDNeed);
	if (!pNeedItem || (unsigned int)pUnconferGod->nItemCountNeed > pNeedItem->GetCount())
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	Lock();
	m_roleInfo.coin -= pUnconferGod->nCoinNeed;
	m_bChangeFlag = true;
	UnLock();
	m_ItemMgr.ReduceCommItem(pNeedItem->GetGuid(), pUnconferGod->nItemCountNeed);

	msg.nErr = 0;
	msg.guid = godGuid;
	msg.serialNo = pGen->GetSerialNo();
	//change
	msg.godSkill = pUnconferGod->RandGodSkill(pGen->m_info.curStar);
	//printf("RandGodSkill return msg.godSkill=%u\n", msg.godSkill);
	pGen->m_info.godSkill = msg.godSkill;
	unsigned char warPos = pGen->GetWarFlag();
	int mainPos = warPos;
	if(warPos > 10)
	{//出战副将
		int pos;
		assert( GetAdjutantPos(warPos, &mainPos, &pos) );
		m_warGeneral[mainPos-1].godSkill[pos] = msg.godSkill;
	}
	else if(warPos > 0)
	{//出战主将
		m_warGeneral[mainPos-1].godSkill[0] = msg.godSkill;
	}
	Calculate(mainPos);
	m_pHandler->Send( (char*)(&msg), len);
	if(warPos > 0)
	{
		SendOneWarGenInfo(mainPos-1);
	}
}

void CUser::OnGodCrack(unsigned int godGuid, unsigned int serialNo)
{
	printf("CUser::OnGodCrack\n");
	stS2CMsgGodCrack msg;
	int len = sizeof(msg);

	CGeneral* pGen = m_ItemMgr.GetGeneral(serialNo);
	if(!pGen)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	//出战武将不可拆解
	if(pGen->GetWarFlag() > 0)
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stGodCard* pGodStatic = GetGodStaticInfo(godGuid);
	assert(pGodStatic);
	unsigned short sAdd = pGodStatic->RandCrackMat();
	printf("RandCrackMat() return %u\n", sAdd);
	Lock();
	m_roleInfo.psychicPower += sAdd;
	m_bChangeFlag = true;
	UnLock();

	msg.nErr = 0;
	msg.oldGuid = pGen->GetGuid();
	msg.oldSerialNo = pGen->GetSerialNo();
	msg.count = sAdd;
	
	pGen->SetDelFlag();
	//SaveGeneralInfo();//SaveOneGenInfo(pGen->GetSerialNo());
	//m_ItemMgr.DelItem(pGen);
	
	m_pHandler->Send( (char*)(&msg), len);
}

void CUser::OnEquipUpLev(unsigned int equipGuid, unsigned int serialNo)
{
	printf("CUser::OnEquipUpLev\n");
	stS2CMsgEquipUpLev msg;
	int len = sizeof(msg);

	CEquip* pEquip = m_ItemMgr.GetEquip(serialNo);
	if(!pEquip)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stEquip* pStatic = GetEquipStaticInfo(equipGuid);
	assert(pStatic);
	CCommItem *pNeedItem = m_ItemMgr.GetMaterial( pStatic->nItemUIDLevelUp);
	if(!pNeedItem || pNeedItem->GetCount() < pStatic->nLevelUpMatertail)
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if((unsigned int)pStatic->nCoinNeedToLevelUp > m_roleInfo.coin)
	{
		msg.nErr = 3;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	if(pStatic->nNextLevelID > 0)
	{
		msg.nErr = 0;
		msg.oldGuid = equipGuid;
		msg.oldSerialNo = pEquip->GetSerialNo();
		//change
		unsigned char warPos = pEquip->GetWarFlag();
		pEquip->SetDelFlag();
		CEquip* pNewEquip = (CEquip*)m_ItemMgr.AddNewItem(pStatic->nNextLevelID);//m_ItemMgr.GetEquip(g_GodSerialNoBegin);
		assert(pNewEquip);

		if( warPos > 0)
		{//
			pNewEquip->m_info.warFlag = warPos;
			EquipWar(pNewEquip);
			Calculate(warPos);
		}
		msg.newGuid = pNewEquip->GetGuid();
		msg.serialNo = pNewEquip->GetSerialNo();
		m_pHandler->Send( (char*)(&msg), len);
		if(warPos > 0)
		{
			SendOneWarGenInfo(warPos-1);
		}
	}
	else
	{
		msg.nErr = 4;	//	
		m_pHandler->Send( (char*)(&msg), len);
	}
}

void CUser::OnEquipCrack(unsigned int equipGuid, unsigned int serialNo)
{
	printf("CUser::OnEquipCrack\n");
	stS2CMsgEquipCrack msg;
	int len = sizeof(msg);

	CEquip* pEquip = m_ItemMgr.GetEquip(serialNo);
	if(!pEquip)
	{
		msg.nErr = 1;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	//出战不可拆解
	if(pEquip->GetWarFlag() > 0)
	{
		msg.nErr = 2;	//	
		m_pHandler->Send( (char*)(&msg), len);
		return;
	}
	stEquip* pStatic = GetEquipStaticInfo(equipGuid);
	assert(pStatic);
	unsigned short sAdd = pStatic->RandCrackMat();
	m_ItemMgr.Add(pStatic->nCrackItemUID, sAdd);

	msg.nErr = 0;
	msg.oldGuid = pEquip->GetGuid();
	msg.oldSerialNo = pEquip->GetSerialNo();
	msg.itemId = pStatic->nCrackItemUID;
	msg.count = sAdd;

	pEquip->SetDelFlag();

	m_pHandler->Send( (char*)(&msg), len);
}

void CUser::OnGetChallTaskList()
{
	//printf("CUser::OnGetChallTaskList\n");
	char buf[BUF_SIZE10];
	memset(buf, 0, sizeof(buf));
	stS2CMsgGetChallTask* pMsg = (stS2CMsgGetChallTask*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_GetChallTask;
	int len = sizeof(stS2CMsgGetChallTask);
	/*if (!m_ChallTaskMgr.HasTask())
	{
		g_UserMgr->CreateChallTaskList(this);
	}
	len += g_UserMgr->CopyChallTaskList(m_roleInfo.userId, buf+len, &(pMsg->count));*/
	len += m_ChallTaskMgr.OnGetChallTaskList(m_roleInfo.userId, buf+len, &(pMsg->count));
	m_pHandler->Send(buf, len);
}

void CUser::OnFlushChallTaskList()
{
	//printf("CUser::OnFlushChallTaskList\n");
	char buf[BUF_SIZE10];
	memset(buf, 0, sizeof(buf));
	stS2CMsgChallTask* pMsg = (stS2CMsgChallTask*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_ChallTask;
	int len = sizeof(stS2CMsgChallTask);
	//刷新列表
	//g_UserMgr->CreateChallTaskList(this);
	//len += g_UserMgr->CopyChallTaskList(m_roleInfo.userId, buf+len, &(pMsg->count));
	len += m_ChallTaskMgr.OnFlushChallTaskList(m_roleInfo.userId, buf+len, &(pMsg->count));
	m_pHandler->Send(buf, len);
}

void CUser::OnWarUserList(unsigned char type, char* pName, unsigned short count)
{
	if(pName)
	{
		GetChallengeUserInfo(pName);
	}
	else
	{
		GetChallengeList(type, count);
	}
}

void CUser::OnHonorExcList()
{
	printf("CUser::OnHonorExcList\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	stS2CMsgHonorExcList* pMsg = (stS2CMsgHonorExcList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_HonorExcList;
	pMsg->count = (unsigned short)(CExchangeConfigMgr::SharedExchangeMgr()->GetConfigCount());
	int len = sizeof(stS2CMsgHonorExcList);
	//len += CExchangeConfigMgr::SharedExchangeMgr()->FillListInfo(buf+len);
	CExchangeConfigMgr* pMgr = CExchangeConfigMgr::SharedExchangeMgr();
	int n = pMgr->GetConfigCount();
	stExchangeHornorConfig* pExcConf = NULL;
	CShopHistory* pSH = NULL;
	unsigned int curTime = getSecTime();
	for(int i=0; i < n; i++)
	{
		stHonorExc* pInfo = (stHonorExc*)(buf+len);
		pExcConf = pMgr->GetConfigByIndex(i);
		pInfo->id = (unsigned int)(pExcConf->nID);
		pSH = m_ShopHistoryMgr.GetShopHistory(pInfo->id);
		if (pSH && (curTime - pSH->m_info.lastTime < (unsigned int)(pExcConf->nColdTime*60) ))
		{
			pInfo->second = pExcConf->nColdTime*60 - (curTime - pSH->m_info.lastTime);
		}
		else
		{
			pInfo->second = 0;
		}
		len += sizeof(stHonorExc);
	}
	m_pHandler->Send(buf, len);
}

void CUser::OnHonorExchange(unsigned int id)
{
	printf("OnHonorExchange\n");
	//char buf[BUF_SIZE1];
	//memset(buf, 0, BUF_SIZE1);
	stS2CMsgHonorExchange msg;//* pMsg = (stS2CMsgHonorExchange*)buf;
	msg.nErr = 0;
	//pMsg->cSysIdentifer = MSG_GAME_S2C;
	//pMsg->cMsgID = eMsg_S2C_HonorExchange;
	
	int len = sizeof(stS2CMsgHonorExchange);
	stExchangeHornorConfig* pExcConfig = CExchangeConfigMgr::SharedExchangeMgr()->GetConfigByID(id);
	if(!pExcConfig)
	{
		msg.nErr = 1;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	if(m_roleInfo.honor < (unsigned int)(pExcConfig->nConsumeHornor))
	{
		msg.nErr = 2;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	if(m_roleInfo.level < pExcConfig->nLevelLimit)
	{
		msg.nErr = 3;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	int curSec = getSecTime();
	unsigned int diff = curSec - m_roleInfo.lastHonorExcTime;	//单位 秒
	if(diff < (unsigned int)(pExcConfig->nColdTime)*60)
	{
		msg.nErr = 4;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	Lock();
	m_roleInfo.honor -= pExcConfig->nConsumeHornor;
	msg.curHonor = m_roleInfo.honor;
	m_roleInfo.lastHonorExcTime = curSec;
	m_bChangeFlag = true;
	UnLock();
	msg.giftId = (unsigned int)(pExcConfig->nID);
	msg.id = id;
	msg.second = (unsigned int)pExcConfig->nColdTime*60;
	m_ItemMgr.Add(pExcConfig->nItemUID);
	if (pExcConfig->bImmediateUse > 0)
	{//立刻使用
		msg.giftId = 0;
		m_pHandler->Send((char*)(&msg), len);
		UseGift((unsigned int)(pExcConfig->nID));
	}
	else
	{
		m_pHandler->Send((char*)(&msg), len);
	}
}

void CUser::OnShopList()
{
	//printf("CUser::OnShopList\n");
	char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	stS2CMsgShopList* pMsg = (stS2CMsgShopList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_ShopList;
	pMsg->count = (unsigned short)(CShopConfigMgr::SharedConfigMgr()->GetAllCommodityCount());
	int len = sizeof(stS2CMsgShopList);
	len += CShopConfigMgr::SharedConfigMgr()->FillListInfo(buf+len);
	m_pHandler->Send(buf, len);
}

void CUser::OnShopBuy(unsigned int id)
{
	//printf("CUser::OnShopBuy\n");
	stS2CMsgShopBuy msg;
	msg.nErr = 0;
	int len = sizeof(stS2CMsgShopBuy);
	stCommodityConfig* pConfig = CShopConfigMgr::SharedConfigMgr()->GetCommodityConfig(id);
	if(!pConfig)
	{
		msg.nErr = 3;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	if( !pConfig->CanByWithConstraint( m_ShopHistoryMgr.GetBuyCount(id) ) )
	{
		msg.nErr = 4;
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	unsigned int price = m_ItemMgr.GetItemPrice(pConfig);
	unsigned int curCoin = 0;
	unsigned int curGold = 0;
	if (pConfig->IsCoinType())
	{//金币
		Lock();
		if (m_roleInfo.coin < price)
		{
			msg.nErr = 2;
			m_pHandler->Send((char*)(&msg), len);
			UnLock();
			return;
		}
		m_roleInfo.coin -= price;
		curCoin = m_roleInfo.coin;
		curGold = m_roleInfo.gold;
		m_bChangeFlag = true;
		UnLock();
	} 
	else
	{//元宝
		Lock();
		if (m_roleInfo.gold < price)
		{
			msg.nErr = 1;
			m_pHandler->Send((char*)(&msg), len);
			UnLock();
			return;
		}
		m_roleInfo.gold -= price;
		curCoin = m_roleInfo.coin;
		curGold = m_roleInfo.gold;
		m_bChangeFlag = true;
		UnLock();
	}
	IItemBase* pItemBase = m_ItemMgr.Add(pConfig->nItemUID, 1);
	m_ShopHistoryMgr.Add(m_roleInfo.userId, pConfig->nItemUID, price, (unsigned char)(pConfig->nPriceType));
	msg.nErr = 0;
	msg.curCoin = curCoin;
	msg.curGold = curGold;
	msg.itemGuid = pConfig->nItemUID;
	msg.serialNo = pItemBase->GetSerialNo();
	m_pHandler->Send((char*)(&msg), len);
}

void CUser::UseGift(unsigned int giftId)
{
	char buf[BUF_SIZE2];
	memset(buf, 0, sizeof(buf));
	stS2CMsgUseGift* pMsg = (stS2CMsgUseGift*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_UseGift;
	pMsg->giftId = giftId;
	pMsg->itemCount = 0;
	int len = sizeof(stS2CMsgUseGift);

	CCommItem* pGift = m_ItemMgr.GetGiftItem(giftId);
	if (!pGift)
	{
		pMsg->nErr = 2;
		m_pHandler->Send(buf, len);
		return;
	}
	CGiftItem* pGiftInfo = CItemManager::SharedItemMgr()->GetGiftByUID(giftId);
	if(!pGiftInfo)
	{
		pMsg->nErr = 2;
		m_pHandler->Send(buf, len);
		return;
	}
	m_ItemMgr.ReduceCommItem(pGift->GetGuid(), 1); //每次用一个
	if (pGiftInfo->nRewardGroupID > 0)
	{
		stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(pGiftInfo->nRewardGroupID);
		if(!pRewardGroup)
		{
			pMsg->nErr = 3;
			m_pHandler->Send(buf, len);
			return;
		}
		//OnReward(pRewardGroup, 3); //礼包奖励
		{
			stReward* pReward = pRewardGroup->RandReward();
			//更新角色信息
			pMsg->nCoin = pReward->iCoin;
			//pMsg->nExp = pReward->iExp;
			pMsg->nHonour = pReward->iHonour;
			pMsg->nGolden = pReward->iGolden;
			Lock();
			m_roleInfo.coin += pReward->iCoin;
			//m_roleInfo.exp += pReward->iExp;
			m_roleInfo.honor += pReward->iHonour;
			m_roleInfo.gold += pReward->iGolden;
			m_bChangeFlag = true;
			//unsigned int curExp = m_roleInfo.exp;
			UnLock();
			unsigned int rewardItemId = pReward->RandItemId();
			if (rewardItemId > 0)
			{
				t_GiftItemInfo* pSignalItem = (t_GiftItemInfo*)(buf + len);
				IItemBase* pItem = NULL;
				if (1101 == rewardItemId)
				{ //灵力值
					Lock();
					m_roleInfo.psychicPower += 1;
					UnLock();
				} 
				else
				{
					pItem = m_ItemMgr.Add(rewardItemId);
				}
				if(pItem)
				{
					pSignalItem->type = (unsigned char)(pItem->GetType());
					pSignalItem->serialNo = pItem->GetSerialNo();
					pSignalItem->guid = pItem->GetGuid();
					pSignalItem->count = 1;
					len += sizeof(t_GiftItemInfo);
					pMsg->itemCount++;
				}
			}
		}
	}
	if (pGiftInfo->nSingleItemID > 0)
	{
		unsigned short count = pGiftInfo->RandSingleItemCount();
		assert(count > 0);
		t_GiftItemInfo* pSignalItem = (t_GiftItemInfo*)(buf + len);
		IItemBase* pItemBase = NULL;
		if (1101 == pGiftInfo->nSingleItemID)
		{ //灵力值
			Lock();
			m_roleInfo.psychicPower += count;
			UnLock();
		} 
		else
		{
			//不可能配置武器或武将,否则count按"1"处理
			pItemBase = m_ItemMgr.Add(pGiftInfo->nSingleItemID, count); 
		}
		if(pItemBase)
		{
			pSignalItem->type = (unsigned char)(pItemBase->GetType());
			pSignalItem->serialNo = pItemBase->GetSerialNo();
			pSignalItem->guid = pItemBase->GetGuid();
			pSignalItem->count = count;
			len += sizeof(t_GiftItemInfo);
			pMsg->itemCount++;
		}
	}
	int n = pGiftInfo->GetMutItemCount();
	t_GiftItemInfo* pItem = (t_GiftItemInfo*)(buf + len);
	IItemBase* pTemp = NULL; 
	for (int i=0; i < n; i++)
	{
		pItem = (t_GiftItemInfo*)(buf + len);
		unsigned short ItemCount = 0;
		pItem->guid = pGiftInfo->GetGiftItemByIndex(i, &ItemCount);
		bool bFillFlag = false;
		if (1101 == pItem->guid)
		{ //灵力值
			Lock();
			m_roleInfo.psychicPower += ItemCount;
			UnLock();
		} 
		else
		{
			CItem* pItemConf = CItemManager::SharedItemMgr()->GetItemByUID(pItem->guid);
			if (eItem_God == pItemConf->m_eItemType || eItem_Equip == pItemConf->m_eItemType)
			{
				unsigned int guid = pItem->guid;
				for(int i=0; i<ItemCount; i++)
				{
					if(bFillFlag)
					{
						pItem = (t_GiftItemInfo*)(buf + len);
						pItem->guid = guid;
					}
					pTemp = m_ItemMgr.AddNewItem(pItem->guid);
					pItem->type = (unsigned char)(pTemp->GetType());
					pItem->serialNo = pTemp->GetSerialNo();
					pItem->count = 1;
					bFillFlag = true;
					len += sizeof(t_GiftItemInfo);
					pMsg->itemCount++;
				}
			} 
			else
			{
				pTemp = m_ItemMgr.Add(pItem->guid, ItemCount);
				pItem->type = (unsigned char)(pTemp->GetType());
				pItem->serialNo = pTemp->GetSerialNo();
				pItem->count = ItemCount;
				len += sizeof(t_GiftItemInfo);
				pMsg->itemCount++;
			}
		}
	}

	pMsg->nErr = 0;
	m_pHandler->Send(buf, len);
}

void CUser::UseItem(unsigned int itemId, unsigned short itemCount)
{
	stS2CMsgUseItem msg;
	msg.id = itemId;
	CCommItem* pItem = m_ItemMgr.GetResActItem(itemId);
	if (pItem)
	{//回复活力值道具
		if (pItem->GetCount() < itemCount)
		{
			msg.nErr = 2;
		}
		else if(UserResActItem(itemId, itemCount) > 0)
		{
			msg.nErr = 3;
		}
		else
		{
			msg.nErr = 0;
		}
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		SendPropBack();
		return;
	}
	pItem = m_ItemMgr.GetAddChallItem(itemId);
	if (pItem)
	{//增加挑战次数道具
		if (pItem->GetCount() < itemCount)
		{
			msg.nErr = 2;
		}
		else if(UserAddChallItem(itemId, itemCount) > 0)
		{
			msg.nErr = 3;
		}
		else
		{
			msg.nErr = 0;
		}
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		SendPropBack();
	}
	else
	{
		msg.nErr = 1;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
	}
}

int CUser::UserResActItem(unsigned int itemId, unsigned short itemCount)
{
	CActiveRestoreItem* pConfig = CItemManager::SharedItemMgr()->GetResActItemByUID(itemId);
	if (!pConfig)
	{
		return 1;
	}
	unsigned int addValue = 0;
	if(pConfig->m_bIsAbsoluteValue)
	{
		addValue = (unsigned int)pConfig->m_fValue;
	}
	else
	{
		addValue = (unsigned int)(m_curLevelInfo.maxActive * pConfig->m_fValue);
	}
	Lock();
	unsigned int nTmp = m_roleInfo.vitality + addValue;
	m_roleInfo.vitality = nTmp > m_curLevelInfo.maxActive ? m_curLevelInfo.maxActive : nTmp;
	m_bChangeFlag = true;
	UnLock();
	m_ItemMgr.ReduceCommItem(itemId, itemCount);
	return 0;
}

int CUser::UserAddChallItem(unsigned int itemId, unsigned short itemCount)
{
	CAddChallengeItem* pConfig = CItemManager::SharedItemMgr()->GetAddChallItemByUID(itemId);
	if (!pConfig)
	{
		return 1;
	}
	Lock();
	unsigned int nTmp = m_roleInfo.leftChallengeTimes + pConfig->m_nAddChallengeTimes;
	m_roleInfo.leftChallengeTimes = nTmp > MAX_CHALLENGE_SIZE ? MAX_CHALLENGE_SIZE : nTmp;
	UnLock();
	m_ItemMgr.ReduceCommItem(itemId, itemCount);
	return 0;
}

void CUser::GetChallengeUserInfo(char* pName)
{
	printf("CUser::GetChallengeUserInfo(%s)\n", pName);
	CUser* pUser = g_UserMgr->GetUser(pName);
	if(!pUser)
	{
		stS2CMsgWarList msg;
		msg.nErr = 1;
		msg.type = 0;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	stS2CMsgWarList* pMsg = (stS2CMsgWarList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_WarList;
	pMsg->nErr = 0;
	pMsg->count = 1;
	int len = sizeof(stS2CMsgWarList);
	len += pUser->FillSimpInfo(buf+len);
	/*stSimpRoleInfo* pInfo = (stSimpRoleInfo*)(buf+len);
	pInfo->userId = pUser->GetUserId();
	pInfo->godId = pUser->GetFirstGodId();
	pInfo->level = pUser->GetLevel();
	pInfo->ability = pUser->GetAbility();
	char* pTmp = pUser->GetName();
	pInfo->nameLen = strlen(pTmp);
	len += sizeof(stSimpRoleInfo);
	memcpy(buf+len, pTmp, pInfo->nameLen);*/
	m_pHandler->Send(buf, len);
}

int CUser::FillSimpInfo(char* buf)
{
	stSimpRoleInfo* pInfo = (stSimpRoleInfo*)buf;
	Lock();
	pInfo->userId = m_roleInfo.userId;
	pInfo->godId = GetFirstGodId();
	pInfo->level = m_roleInfo.level;
	pInfo->ability = m_roleInfo.ability;
	UnLock();
	//char* pTmp = pUser->GetName();
	pInfo->nameLen = strlen(m_roleInfo.szName);
	int offset = sizeof(stSimpRoleInfo);
	memcpy(buf+offset, m_roleInfo.szName, pInfo->nameLen);
	offset += pInfo->nameLen;
	return offset;
}

int CUser::FillChallRoleInfo(char* buf, unsigned char flag)
{
	stChallRoleInfo* pInfo = (stChallRoleInfo*)buf;
	Lock();
	pInfo->userId = m_roleInfo.userId;
	pInfo->godId = GetFirstGodId();
	pInfo->level = m_roleInfo.level;
	pInfo->ability = m_roleInfo.ability;
	pInfo->flag = flag;
	UnLock();
	//char* pTmp = pUser->GetName();
	pInfo->nameLen = strlen(m_roleInfo.szName);
	int offset = sizeof(stChallRoleInfo);
	memcpy(buf+offset, m_roleInfo.szName, pInfo->nameLen);
	offset += pInfo->nameLen;
	return offset;
}

void CUser::GetChallengeList(unsigned char type, unsigned short count)
{
	printf("CUser::GetChallengeList(%u)\n", type);
	char buf[BUF_SIZE10];
	memset(buf, 0, sizeof(buf));
	stS2CMsgWarList* pMsg = (stS2CMsgWarList*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_WarList;
	pMsg->nErr = 0;
	pMsg->type = type;
	int len = sizeof(stS2CMsgWarList);
	if (type > 0)
	{
		len += m_RelationMgr.CopyRelationList(type, count, buf+len, &(pMsg->count));
	} 
	else
	{
		len += g_UserMgr->CopyUserList(m_roleInfo.userId, type, count, buf+len, &(pMsg->count));
	}
	m_pHandler->Send(buf, len);
	
	/*stSimpRoleInfo* pInfo = (stSimpRoleInfo*)(buf+len);
	pInfo->userId = pUser->GetUserId();
	pInfo->godId = pUser->GetFirstGodId();
	pInfo->level = pUser->GetLevel();
	pInfo->ability = pUser->GetAbility();
	char* pTmp = pUser->GetName();
	pInfo->nameLen = strlen(pTmp);
	
	memcpy(buf+len, pTmp, pInfo->nameLen);
	*/
}

/*void CUser::OnTopUserChallenge(unsigned int userId)
{
	CUser* pDefenceUser = g_UserMgr->GetUser(userId);
	if (!pDefenceUser)
	{
		stS2CMsgChallenge msg;
		msg.nErr = 2;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	Lock();
	if (m_roleInfo.leftChallengeTimes <= 0)
	{
		UnLock();
		stS2CMsgChallenge msg;
		msg.nErr = 3;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
	}
	else
	{
		m_roleInfo.leftChallengeTimes--;
		UnLock();
	}
}*/

void CUser::OnUserChallenge(unsigned int userId, unsigned char type, unsigned int topId)
{
	stS2CMsgChallenge msg;
	msg.leftChallengeTimes = m_roleInfo.leftChallengeTimes;

	if (0 == type && !m_ChallengeMgr.CanChallenge(userId))
	{
		msg.nErr = 7;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	if (1 == type && (topId <=0 || topId > 50))
	{
		msg.nErr = 4;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	CUser* pDefenceUser = g_UserMgr->GetUser(userId);
	if (!pDefenceUser)
	{
		msg.nErr = 2;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	Lock();
	if (m_roleInfo.leftChallengeTimes <= 0)
	{
		UnLock();
		msg.nErr = 3;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		return;
	}
	else
	{
		UnLock();
	}
	CCompeteRankConfig* pTmpConf = CCompeteRankConfig::SharedMgr();
	CTopUser* pAttackTopUser = g_TopInfoMgr->GetTopUser(m_roleInfo.userId);
	CCompeteRankConfig::stRankClearing* pAttackClear = NULL;
	if (pAttackTopUser)
	{//攻方是排行榜内玩家
		pAttackClear = pTmpConf->GetRankClearingForRank(topId, false);
	}
	else
	{//非排行榜内玩家
		pAttackClear = pTmpConf->GetRankClearingForRank(51, false);
	}
	assert(pAttackClear);
	CTopUser* pDefenceTopUser = g_TopInfoMgr->GetTopUser(userId);
	CCompeteRankConfig::stRankClearing* pDefenceClear = NULL;
	if (pDefenceTopUser)
	{//排行榜内玩家
		pDefenceClear = pTmpConf->GetRankClearingForRank(pDefenceTopUser->m_info.topId, true);
	}
	else
	{//非排行榜内玩家
		pDefenceClear = pTmpConf->GetRankClearingForRank(51, true);
	}
	assert(pDefenceClear);
	Lock();
	if (m_roleInfo.honor < (unsigned int)pDefenceClear->nAchieveWin)
	{
		msg.nErr = 1;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
		UnLock();
		return;
	}
	else
	{
		m_roleInfo.leftChallengeTimes--;
		UnLock();
	}
	//attack
	unsigned int attack[WAR_POS_MAX]={0};
	unsigned int attack2[WAR_POS_MAX]={0};
	CGeneral* pAttackGen[WAR_POS_MAX] = {NULL,NULL,NULL,NULL,NULL};
	//defence
	unsigned int defence[WAR_POS_MAX]={0};
	unsigned int defence2[WAR_POS_MAX]={0};
	CGeneral* pDefenceGen[WAR_POS_MAX] = {NULL,NULL,NULL,NULL,NULL};
	
	//出战成员数值
	for (int i=0; i<WAR_POS_MAX; i++)
	{
		attack[i] = GetGeneralAttack(i);
		defence[i] = pDefenceUser->GetGeneralDefence(i);
		
		attack2[i] = attack[i];
		defence2[i] = defence[i];

		m_challengeRes.attack[i] = attack[i];
		m_challengeRes.defence[i] = defence[i];
		m_challengeRes.attackGodGuid[i] = GetGodId(i);
		m_challengeRes.defenceGodGuid[i] = pDefenceUser->GetGodId(i);
	}
	//封神技能
	stConferGodSkll* pAttackSkill[WAR_POS_MAX][SKILL_MAX_PER_GOD] = {{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL}};
	stConferGodSkll* pDefenceSkill[WAR_POS_MAX][SKILL_MAX_PER_GOD] = {{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL},{NULL,NULL,NULL,NULL}};
	stConferGodSkll* pTmpSkill = NULL;
	for (int i=0; i<WAR_POS_MAX; i++)
	{
		pAttackGen[i] = GetWarGeneral(i);
		pDefenceGen[i] = pDefenceUser->GetWarGeneral(i);
		for(int j=0;j<SKILL_MAX_PER_GOD;j++)
		{
			pTmpSkill = (stConferGodSkll*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(GetGodSkillGuid(i,j));
			if(pTmpSkill && pTmpSkill->CanInvoke())
			{
				pAttackSkill[i][j] = pTmpSkill;
				m_challengeRes.attackSkillGuid[i][j] = (unsigned int)pTmpSkill->nUID;
			}
			//defence
			pTmpSkill = (stConferGodSkll*)CSkillMgr::SharedSkillMgr()->GetSkillByUID(pDefenceUser->GetGodSkillGuid(i,j));
			if(pTmpSkill && pTmpSkill->CanInvoke())
			{
				pDefenceSkill[i][j] = pTmpSkill;
				m_challengeRes.defenceSkillGuid[i][j] = (unsigned int)pTmpSkill->nUID;
			}
		}
	}
	//计算攻方封神技
	for(int i=0;i<WAR_POS_MAX;i++)
	{
		for(int j=0;j<SKILL_MAX_PER_GOD;j++)
		{
			if (pAttackSkill[i][j])
			{
				eCalculateType type = pAttackSkill[i][j]->GetCalculateType();
				switch (type)
				{
					case eSelf_Add_RenGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Ren == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}
						
					}
					break;
					case eSelf_Add_ChanGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Chan == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}
					}
					break;
					case eSelf_Add_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Jie == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] += (unsigned int)(attack[k]*pAttackSkill[i][j]->fAttack);
							}
						}
					}
					break;
					case eOther_Reduce_RenGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Ren == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] -= (unsigned int)(defence[k]*pAttackSkill[i][j]->fDefend);
							}
						}
					}
					break;
					case eOther_Reduce_ChanGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Chan == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] -= (unsigned int)(defence[k]*pAttackSkill[i][j]->fDefend);
							}
						}
					}
					break;
					case eOther_Reduce_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Jie == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] -= (unsigned int)(defence[k]*pAttackSkill[i][j]->fDefend);
							}
						}
					}
					break;
					default:
						break;
				}
			}
		}
	}
	//计算守方封神技
	for(int i=0;i<WAR_POS_MAX;i++)
	{
		for(int j=0;j<SKILL_MAX_PER_GOD;j++)
		{
			if (pDefenceSkill[i][j])
			{
				eCalculateType type = pDefenceSkill[i][j]->GetCalculateType();
				switch (type)
				{
					case eSelf_Add_RenGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Ren == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] += (unsigned int)(defence[k]*pDefenceSkill[i][j]->fDefend);
							}
						}
						
					}
					break;
					case eSelf_Add_ChanGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Chan == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] += (unsigned int)(defence[k]*pDefenceSkill[i][j]->fDefend);
							}
						}
					}
					break;
					case eSelf_Add_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pDefenceGen[k] && (eCardGroup_Jie == pDefenceGen[k]->GetGroupType()) )
							{
								defence2[k] += (unsigned int)(defence[k]*pDefenceSkill[i][j]->fDefend);
							}
						}
					}
					break;
					case eOther_Reduce_RenGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Ren == pAttackGen[k]->GetGroupType()) ) 
							{
								attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[i][j]->fAttack);
							}
						}
					}
					break;
					case eOther_Reduce_ChanGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Chan == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[i][j]->fAttack);
							}
						}
					}
					break;
					case eOther_Reduce_JieGroup:
					{
						for (int k=0;k<WAR_POS_MAX;k++)
						{
							if ( pAttackGen[k] && (eCardGroup_Jie == pAttackGen[k]->GetGroupType()) )
							{
								attack2[k] -= (unsigned int)(attack[k]*pDefenceSkill[i][j]->fAttack);
							}
						}
					}
					break;
					default:
						break;
				}
			}
		}
	}
	//攻防总值
	unsigned int attackAll = GetLevelAttack();
	unsigned int defenceAll = pDefenceUser->GetLevelDefence();
	for (int i=0;i<WAR_POS_MAX;i++)
	{
		attackAll += attack2[i];
		defenceAll += defence2[i];
	}

	char * pName = pDefenceUser->GetName();
	memcpy(m_challengeRes.otherName, pName, strlen(pName));
	m_challengeRes.res = attackAll > defenceAll ? 1 : 2; 
	m_challengeRes.defenceLevel = pDefenceUser->GetLevel();
	m_challengeRes.attackAll = attackAll;
	m_challengeRes.defenceAll = defenceAll;

	char buf[BUF_SIZE1];
	memset(buf, 0, sizeof(buf));
	stS2CMsgChallenge* pMsg = (stS2CMsgChallenge*)buf;
	pMsg->cSysIdentifer	= MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_Challenge;
	pMsg->nErr = m_challengeRes.res+4;	//nErr: 5 挑战胜利 6 挑战失败; m_challengeRes.res  1 胜利 2 失败
	pMsg->attackValue = attackAll;
	pMsg->userId = userId;
	pMsg->defenceValue = defenceAll;
	pMsg->defenceLevel = pDefenceUser->GetLevel();
	//清算结果
	if(5 == pMsg->nErr)
	{	//攻方胜利
		pMsg->honor = 10;			//暂时固定
		pMsg->achieve = 0;
		if(m_roleInfo.level >= 20)
		{//20级以上才有战功
			if (1 == type)
			{//挑战排行榜
				//清算攻方
				pMsg->achieve = pAttackClear->nAchieveWin;
				pMsg->honor += pAttackClear->nHonor;
				//清算守方
				pDefenceUser->ReduceDefenceClear(pDefenceClear->nAchieveFail, pDefenceClear->nHonor);
			} 
			else
			{
				pMsg->achieve = 1;			//暂时固定
			}
		}
		Lock();
		m_roleInfo.honor += pMsg->honor;
		m_roleInfo.wonTimes += pMsg->achieve;
		m_bChangeFlag = true;
		UnLock();
		if (pAttackTopUser || pDefenceTopUser)
		{
			g_TopInfoMgr->CheckTopNo(this);
			if (pDefenceTopUser)
			{
				g_TopInfoMgr->CheckTopNo(pDefenceUser);
			}
		}
		if (g_TopInfoMgr->HasEmptyPos())
		{
			g_TopInfoMgr->CheckTopNo(this);
		}
		pDefenceUser->AddFoe(m_roleInfo.userId);
	}
	else
	{
		if(m_roleInfo.level >= 20)
		{//20级以上才有战功
			if (1 == type)
			{//挑战排行榜
				//清算攻方
				pMsg->achieve = pAttackClear->nAchieveFail;
				pMsg->honor = pAttackClear->nHonor;
				//清算守方
				pDefenceUser->AddDefenceClear(pDefenceClear->nAchieveWin, pDefenceClear->nHonor);
			} 
			else
			{
				pMsg->achieve = 1;			//暂时固定
			}
		}
		Lock();
		if(m_roleInfo.honor > pMsg->honor) m_roleInfo.honor -= pMsg->honor;
		else m_roleInfo.honor = 0;
		if(m_roleInfo.wonTimes > pMsg->achieve) m_roleInfo.wonTimes -= pMsg->achieve;
		else m_roleInfo.wonTimes = 0;
		m_bChangeFlag = true;
		UnLock();
		if (pAttackTopUser)
		{
			g_TopInfoMgr->CheckTopNo(this);
		}
		if (pDefenceTopUser)
		{
			g_TopInfoMgr->CheckTopNo(pDefenceUser);
		}
		pDefenceUser->AddFoe(m_roleInfo.userId);
	}
	m_ChallengeMgr.AddHistory(m_roleInfo.userId, userId, type);
	pMsg->wonTimes = m_roleInfo.wonTimes;
	pMsg->leftChallengeTimes = m_roleInfo.leftChallengeTimes;
	pMsg->nameLen = strlen(pName);
	pMsg->pName = NULL;
	int len = sizeof(stS2CMsgChallenge);
	memcpy(buf+len, pName, pMsg->nameLen);
	len += pMsg->nameLen;

	m_pHandler->Send(buf, len);
	//挑战任务
	if (2 == type && 5 == pMsg->nErr && !m_ChallTaskMgr.Finish())
	{
		if(m_ChallTaskMgr.SubTaskFinish(userId) >= 7)
		{
			m_ChallTaskMgr.SetTaskFinish();
		}
	}
}

int CUser::AddFoe(unsigned int userId)
{
	return m_RelationMgr.AddFoe(userId);
}

void CUser::ReduceDefenceClear(int achieve, int honor)
{
	int factor = 1;
	Lock();
	if(m_roleInfo.honor > (unsigned int)honor)
	{
		m_roleInfo.honor -= honor;
	}
	else
	{
		m_roleInfo.honor = 0;
		factor = 2;
	}
	if (m_roleInfo.level >= 20)
	{
		if(m_roleInfo.wonTimes > achieve)
		{//	当排行榜中的玩家荣誉值不足时，防守失败则扣除双倍战功
			m_roleInfo.wonTimes -= achieve * factor;
		}
		else
		{
			m_roleInfo.wonTimes = 0;
		}
	}
	UnLock();
}

void CUser::AddDefenceClear(int achieve, int honor)
{
	Lock();
	if (m_roleInfo.level >= 20)
	{
		m_roleInfo.wonTimes += achieve;
	}
	m_roleInfo.honor += honor;
	UnLock();
}

void CUser::OnUserChallengeRes(unsigned int userId)
{
	if (m_challengeRes.res <= 0)
	{
		stS2CMsgChallengeRes msg;
		int len = sizeof(stS2CMsgChallengeRes);
		memset(&msg, 0, len);
		m_pHandler->Send((char*)(&msg), len);
		return;
	}
	char buf[BUF_SIZE2];
	memset(buf, 0, sizeof(buf));
	stS2CMsgChallengeRes* pMsg = (stS2CMsgChallengeRes*)buf;
	pMsg->cSysIdentifer = MSG_GAME_S2C;
	pMsg->cMsgID = eMsg_S2C_ChallengeRes;
	pMsg->nErr = m_challengeRes.res;
	pMsg->defenceLevel = m_challengeRes.defenceLevel;
	pMsg->attackValue[0] = m_challengeRes.attackAll;
	pMsg->defenceValue[0] = m_challengeRes.defenceAll;
	for (int i=0; i<WAR_POS_MAX; i++)
	{
		pMsg->attackValue[i+1] = m_challengeRes.attack[i];
		pMsg->defenceValue[i+1] = m_challengeRes.defence[i];
		pMsg->attackGodGuid[i] = m_challengeRes.attackGodGuid[i];
		pMsg->defenceGodGuid[i] = m_challengeRes.defenceGodGuid[i];
		for (int j=0; j<SKILL_MAX_PER_GOD; j++)
		{
			pMsg->attackSkillGuid[i][j] = m_challengeRes.attackSkillGuid[i][j];
			pMsg->defenceSkillGuid[i][j] = m_challengeRes.defenceSkillGuid[i][j];
		}
	}
	pMsg->nameLen = strlen(m_challengeRes.otherName);
	int len = sizeof(stS2CMsgChallengeRes);
	memcpy(buf+len, m_challengeRes.otherName, pMsg->nameLen);
	len += pMsg->nameLen;
	m_pHandler->Send(buf, len);
}

void CUser::OnChallTaskReward()
{
	stS2CMsgChallTaskReward msg;
	msg.subTaskCount = m_ChallTaskMgr.GetFinishCount();
	if (m_ChallTaskMgr.Finish())
	{
		if(!m_ChallTaskMgr.IsGetReward())
		{
			msg.nErr = 0;
			m_pHandler->Send((char*)(&msg), sizeof(msg));
			stRewardGroup* pRewardGroup = CRewardMgr::SharedRewardMgr()->GetRewardGroup(10110);
			OnReward(pRewardGroup, 4);//4 每日完成挑战任务奖励
			m_ChallTaskMgr.FinishGetReward();
		}
		else
		{
			msg.nErr = 2;
			m_pHandler->Send((char*)(&msg), sizeof(msg));
		}
	}
	else
	{
		msg.nErr = 1;
		m_pHandler->Send((char*)(&msg), sizeof(msg));
	}
}

bool CUser::GetAdjutantPos(unsigned char warPos, int* mainPos, int* pos)
{
	//warPos 可以优化
	if(warPos>=11 && warPos <=13)
	{
		*mainPos = 1;
		if (11 == warPos) *pos = 1;
		else if(12 == warPos) *pos = 2;
		else *pos = 3;
	}
	else if(warPos>=21 && warPos <=23)
	{
		*mainPos = 2;
		if (21 == warPos) *pos = 1;
		else if(22 == warPos) *pos = 2;
		else *pos = 3;
	}
	else if(warPos>=31 && warPos <=33)
	{
		*mainPos = 3;
		if (31 == warPos) *pos = 1;
		else if(32 == warPos) *pos = 2;
		else *pos = 3;
	}
	else if(warPos>=41 && warPos <=43)
	{
		*mainPos = 4;
		if (41 == warPos) *pos = 1;
		else if(42 == warPos) *pos = 2;
		else *pos = 3;
	}
	else if(warPos>=51 && warPos <=53)
	{
		*mainPos = 5;
		if (51 == warPos) *pos = 1;
		else if(52 == warPos) *pos = 2;
		else *pos = 3;
	}
	else
	{
		return false;
	}
	return true;
}



