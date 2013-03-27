#ifndef _USER__H_
#define _USER__H_

#include <memory.h>
#include "../Share/Common.h"
#include "../Base/InternalMsg.h"
#include <set>

struct t_ROLE_INFO
{
	unsigned int userId;	//玩家id
	char szName[MAX_NAME_SIZE];	//角色名
	unsigned int exp;		//人物的当前等级
	unsigned char level;	//当前拥有经验
	unsigned int coin;		//当前拥有的金币
	unsigned int gold;		//当前拥有的元宝
	unsigned char vitality;	//当前拥有的活力值
	unsigned int ability;	//能力值
	unsigned int psychicPower;//灵力值
	unsigned int honor;	//玩家拥有的荣誉值
	unsigned short curTask;	//当前任务id
	unsigned char leftChallengeTimes;	//剩余的挑战次数
	unsigned short wonTimes;//doubleWonTimes;	//连胜的次数
	unsigned short challengeWonTimes;	//挑战任务中胜利次数
	unsigned int lastHonorExcTime;	//最新荣誉兑换时间
	unsigned int lastActiveTime;	//最新活跃时间
	unsigned int oldTopId;
	unsigned int newTopId;

	unsigned short ltAttack;	//出战队列总攻击下限值
	unsigned short utAttack;	//出战队列总攻击上限值
	unsigned short ltDefence;	//出战队列总防御下限值
	unsigned short utDefence;	//出战队列总防御上限值
};

class CUser
{
public:
	CUser(uint32_t uid);
	~CUser();
	
	uint32_t Login(stUserInfo userInfo, stRoleInfo roleInfo);
	void CreateCheckNo();
	uint32_t GetCheckNo();
	
	//关闭连接前保存玩家相应数据等
	void PreShutDownHandle();
	void Logout();
	
	bool IsBroken()
	{
		return false;//return m_pHandler->IsBroken();
	}
	int FillPltInfo(char* pPltInfo);
	int Send(const uint8_t *  buf, int len );

	//bool SaveDB();
	//bool LoadDB();
public:
	uint32_t GetUserId() { return m_nUserId; }
private:
	uint32_t	m_nUserId;
	char		m_szName[MAX_NAME_SIZE];
	char		m_szPwd[MAX_PASS_SIZE];
	uint8_t		m_nOnlineFlag;
	char		m_szEmail[MAX_EMAIL_SIZE];
	char		m_szMobile[MAX_MOBILE_LEN];
	t_ROLE_INFO	m_roleInfo;
	
	uint32_t	m_nCheckNo;
};

#endif

