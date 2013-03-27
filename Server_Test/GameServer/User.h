#ifndef _USER__H_
#define _USER__H_

#include <memory.h>
#include "../Share/Common.h"
#include "../Base/InternalMsg.h"
#include <set>
#include "../Base/IUserBase.h"
#include "../Base/Handler.h"
#include "PlrItemMgr.h"
#include "Task.h"
#include "LevelData.h"
#include "General.h"
#include "Equip.h"
#include "CommItem.h"
#include "ItemManager.h"
#include "RewardMgr.h"
#include "GodOperateConfig.h"
#include "RelationMgr.h"
#include "shopHistory.h"
#include "../Share/mutex.h"
#include "Challenge.h"
#include "ChallTask.h"
#include "Battle.h"

struct t_LEVEL_INFO
{
	unsigned int maxActive;			//活力值上限
    unsigned short actvieRestoreSpeed; 	//活力回复速度 1点体力/actvieRestoreSpeed秒
    unsigned short attackLow;			//攻击下限
    unsigned short attackUp;			//攻击上限
    unsigned short defendLow;			//防御下限
    unsigned short defendUp;			//防御上限
};

class CUser : public IUserBase
{
public:
	CUser(uint32_t uid);
	~CUser();
	
	void SendPropBack();
	void Update(uint32_t tim);
	//客户端身份通过随机验证码验证，暂时不用
	void Login(uint32_t checkNo, stRoleInfo roleInfo);
	void Login(t_ROLE_INFO roleInfo);
	
	//关闭连接前保存玩家相应数据等
	void PreShutDownHandle();
	void Logout();
	
	inline void SetStatus(t_USER_STATUS status)
	{
		m_status = status;
	}

	inline t_USER_STATUS GetStatus()
	{
		return m_status;
	}

	inline bool IsBroken()
	{
		return false;//return m_pHandler->IsBroken();
	}
	inline int FillPltInfo(char* pPltInfo);
	inline int Send(const char*  buf, const int len )
	{
		return m_pHandler->Send(buf, len);
	}

	inline void SetHandler(CHandler* pHandler)
	{
		assert(pHandler);
		m_pHandler = pHandler;
	}
	inline CHandler* GetHandler()
	{
		return m_pHandler;
	}
	inline CChallSubTask* GetSubChall(unsigned short subId)
	{
		return m_ChallTaskMgr.GetSubChall(subId);
	}
	inline void AddChallSubTask(CChallSubTask* pSub)
	{
		m_ChallTaskMgr.Add(pSub);
	}

	inline uint32_t GetUserId() 
	{ return m_roleInfo.userId; }
	inline char* GetName()
	{
		return m_roleInfo.szName;
	}
	inline t_ROLE_INFO* GetRoleInfo()
	{
		return &m_roleInfo;
	}
	inline unsigned int GetFirstGodId()
	{
		return m_warGeneral[0].guid;
	}
	inline unsigned int GetGodId(int i)
	{
		assert(i>=0 && i<5);
		return m_warGeneral[i].guid;
	}
	inline unsigned int GetGodSkillGuid(int i, int j)
	{
		return m_warGeneral[i].godSkill[j];
	}
	inline CGeneral* GetWarGeneral(int i)
	{
		return m_ItemMgr.GetGeneral(m_warGeneral[i].serialNo);
	}
	inline unsigned short GetWonTimes()
	{
		return m_roleInfo.wonTimes;
	}
	inline void ClearWonTimes()
	{
		m_roleInfo.wonTimes = 0;
	}
	inline unsigned short GetOldTopId()
	{
		return 0;//return m_roleInfo.oldTopId;
	}
	inline void ExecHonor()
	{
		Lock();
		m_roleInfo.honor += m_roleInfo.wonTimes * 10;
		m_roleInfo.wonTimes = 0;
		UnLock();
	}
	inline unsigned char GetLevel()
	{
		return m_roleInfo.level;
	}
	inline unsigned int GetAbility()
	{
		return m_roleInfo.ability;
	}
	unsigned int GetGeneralAttack(int index);
	unsigned int GetGeneralDefence(int index);
	unsigned int GetLevelAttack();
	unsigned int GetLevelDefence();

	//计算攻防数据
	void CalculateRole();
	void SendRoleInfo();
	void Calculate(CGeneral* pGen, bool bNeedSendFlag = false);
	void Calculate(unsigned char warPos, bool bNeedSendFlag = false);
	void CalculateEquip(CEquip* pEquip, bool bAddFlag = false);
	bool CalculateBasic(unsigned char posIndex);

	void LoadTaskInfo(char* buf, short count);
	void LoadGeneralInfo(char* buf, short count);
	void LoadEquipInfo(char* buf, short count);
	void LoadItemInfo(char* buf, short count);
	void LoadRelationInfo(char* buf, short count);
	void LoadShopHistoryInfo(char* buf, short count);
	void LoadChallengeInfo(char* buf, short count);
	void LoadChallTaskInfo(char* buf, short count);
	void LoadBattleInfo(char* buf, short count);

	stGodCard* GetGodStaticInfo(unsigned int guid);
	stConferGod* GetConferGodStaticInfo(unsigned int guid);
	stUnconferGod* GetUnconferGodStaticInfo(unsigned int guid);
	stEquip* GetEquipStaticInfo(unsigned int guid);
	void LoadWarGeneral();
	void LoadWarEquip();
	void SendWarGenInfo();
	bool GetAdjutantPos(unsigned char warPos, int* mainPos, int* pos);
	bool OnReward(stRewardGroup* pRewardGroup, unsigned char type);	//升级返回true
	void OnDailyReward();
	void OnGodList();
	void OnEquipList();
	void OnItemList();
	void FillWarGodInfo(unsigned char warPos, t_WarGodInfo* pWarGod);
	void GenWar(CGeneral* pGen);
	void EquipWar(CEquip* pEquip);
	void AdjutantWar(CGeneral* pGen, int mainPos, int pos);
	void GenBack(CGeneral* pGen);
	//void GenBack(int pos);
	void EquipBack(CEquip* pEquip);
	//void EquipBack(int pos);
	void OnAdjutantPlay(unsigned int serialNo, unsigned char warPos);
	void AdjutantPlay(unsigned int serialNo, int mainPos, int pos);
	void OnGeneralPlay(unsigned int serialNo, unsigned char warPos);
	void OnEquipPlay(unsigned int serialNo, unsigned char warPos);
	void OnGeneralBack(unsigned int serialNo, unsigned char warPos);
	void OnEquipBack(unsigned int serialNo, unsigned char warPos);
	void OnAdjutantBack(unsigned int serialNo, int mainPos, int pos);
	void OnGeneralUpLev(unsigned int godGuid, unsigned int serialNo, unsigned int serialNo2);
	void OnGeneralUpStar(unsigned int godGuid, unsigned int serialNo, unsigned int serialNo2);
	void OnGeneralChangeSkill(unsigned int godGuid, unsigned int serialNo);
	void OnEquipUpLev(unsigned int equipGuid, unsigned int serialNo);

	void OnGodCrack(unsigned int godGuid, unsigned int serialNo);
	void OnEquipCrack(unsigned int equipGuid, unsigned int serialNo);

	void OnGetChallTaskList();
	void OnFlushChallTaskList();
	void OnWarUserList(unsigned char type, char* pName, unsigned short count);
	int FillSimpInfo(char* buf);
	int FillChallRoleInfo(char* buf, unsigned char flag);
	void GetChallengeUserInfo(char* pName);
	void GetChallengeList(unsigned char type, unsigned short count);
	//void OnTopUserChallenge(unsigned int userId);
	void OnUserChallenge(unsigned int userId, unsigned char type, unsigned int topId);
	void OnUserChallengeRes(unsigned int userId);
	void OnChallTaskReward();

	int AddFoe(unsigned int userId);
	void ReduceDefenceClear(int achieve, int honor);
	void AddDefenceClear(int achieve, int honor);

	void OnHonorExcList();
	void OnHonorExchange(unsigned int id);
	void UseGift(unsigned int giftId);
	void UseItem(unsigned int itemId, unsigned short itemCount);

	void OnShopList();
	void OnShopBuy(unsigned int id);

	int UserResActItem(unsigned int itemId, unsigned short itemCount);
	int UserAddChallItem(unsigned int itemId, unsigned short itemCount);
public:
	void OnTaskInfo(unsigned int nMainId);
	void OnTask(unsigned int nMainId, unsigned int nLeafId);
	void OnBattleList();
	void OnDoBattle(unsigned short mainId, unsigned short curLeafId);
	void OnBattleReset(unsigned short mainId);
	void OnBattleReset();

	bool CheckLevelUp(unsigned int exp);
	void OnLevelUp(stLevelData* pNextLevel);
	
	short CopyGeneralInfo(unsigned int roleId, char* buf, short bufLen);
	void SaveRoleBase();
	void SaveOneGenInfo(unsigned int godNo);

	void SaveGeneralInfo();
	void SaveEquipInfo();
	void SaveCommItem();
	void SaveRelationInfo();
	void SaveShopHistory();
	void SaveChallengeHistory();
	void SaveChallTask();
	void SaveBattleInfo();

	bool SendOneWarGenInfo(int index);
	void SaveData();
	void ChallTaskUpdate();
	void OnDisconnect();
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
private:
	Mutex m_mutex;
	CHandler* 			m_pHandler;
	uint32_t			m_nUserId;
	char				m_szName[MAX_NAME_SIZE];
	char				m_szPwd[MAX_PASS_SIZE];
	uint8_t				m_nOnlineFlag;
	char				m_szEmail[MAX_EMAIL_SIZE];
	char				m_szMobile[MAX_MOBILE_LEN];
	t_ROLE_INFO			m_roleInfo;
	bool				m_bChangeFlag;
	t_LEVEL_INFO		m_curLevelInfo;
	//t_TASK_INFO			m_curTaskInfo;
	uint32_t			m_nCheckNo;//暂时不要
	CTask				m_task;
	CBattleMgr			m_battleMgr;
	CPlrItemMgr			m_ItemMgr;
	CRelationMgr		m_RelationMgr;
	CShopHistoryMgr		m_ShopHistoryMgr;
	CChallengeMgr		m_ChallengeMgr;
	CChallTaskMgr		m_ChallTaskMgr;
	t_WarGeneralInfo	m_warGeneral[WAR_POS_MAX];
    uint64_t			m_msLastChangeVitality;		//体力值更新时间
	uint64_t			m_msLastChangeChallTime;	//挑战次数更新时间
	t_USER_STATUS		m_status;
	t_ChallengeRes		m_challengeRes;
};

#endif

