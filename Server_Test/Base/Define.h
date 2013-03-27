#ifndef _DEFINE__H_
#define _DEFINE__H_

#include "../Share/Common.h"
#include "Item.h"
#include "GodCard.h"

#define MAX_CHALLENGE_SIZE		20
#define LEVLE_BIG_BEGIN			20
#define MAX_LEVLE_SIZE			100
#define CHALL_TASK_SIZE			10

enum t_USER_STATUS
{
	STATUS_IDLE,
	STATUS_WAR,
	STATUS_FUBEN,
	
	STATUS_END
};
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
	//unsigned short compDoubleWonTimes;	//竞技场连胜次数
	//unsigned short attackTimes;		//主动攻击的次数
	//unsigned short defenceTimes;	//被动防御的次数
	//unsigned short attackWonTimes;	//主动攻击胜利次数
	//unsigned short defenceWonTimes;	//被动防御胜利次数
	unsigned int lastHonorExcTime;	//最新荣誉兑换时间
	unsigned int lastActiveTime;	//最新活跃时间
	unsigned int oldTopId;
	unsigned int newTopId;
	
	unsigned short ltAttack;	//出战队列总攻击下限值
	unsigned short utAttack;	//出战队列总攻击上限值
	unsigned short ltDefence;	//出战队列总防御下限值
	unsigned short utDefence;	//出战队列总防御上限值
};

struct t_TaskInfo
{
	unsigned short mainId;
	unsigned short leafId;
	unsigned char leftTimes;
	unsigned char changeFlag;	//1 增 2 删 3 改 0 无改变
	t_TaskInfo()
	{
		mainId = 0;
		leafId = 0;
		leftTimes = 0;
		changeFlag = 0;
	}
	void SetFlag(unsigned char flag = 3)
	{
		changeFlag = flag;
	}
	void ResetFlag()
	{
		changeFlag = 0;
	}
};

struct t_GeneralInfo
{
	unsigned int serialNo;	//武将流水号
	unsigned int roleId;
	unsigned int itemId;
	//unsigned int guid;
	unsigned char warFlag;
    eCardGroupType group;
	unsigned char curStar;
    unsigned char curLevel;
	unsigned int godSkill;	//封神技能
};

struct t_ShopHistoryInfo
{
	unsigned int roleId;
	unsigned int guid;
	unsigned short count;
	unsigned char priceType;
	unsigned int price;
	unsigned int lastTime;	//最近更新时间
};

struct t_TopInfo
{
	unsigned int topId;
	unsigned int roleId;
	unsigned int lastTime;
};

struct t_ChallengeInfo
{
	unsigned int myRoleId;
	unsigned int roleId;
	unsigned short times;
	unsigned char type;
	unsigned int lastTime;	
};

struct t_ChallTask
{
	unsigned int roleId;
	unsigned short subTaskId;//0~9
	unsigned int roleId2;	//挑战对象
	unsigned char flag;		//1 完成 0 未完成
};

#define WAR_POS_MAX			5
#define SKILL_MAX_PER_GOD	4
struct t_WarGeneralInfo
{
	unsigned int serialNo;	//武将流水号
	unsigned int roleId;
	unsigned char warPos;
	unsigned int guid;
	unsigned int ability;	//能力值
	unsigned int adjutantGuid[MAX_AJUTANT];
	unsigned int adjutantNo[MAX_AJUTANT];
	unsigned int weaponGuid;
	unsigned int weaponNo;
	unsigned int armorGuid;
	unsigned int armorNo;
	unsigned int treasureGuid;
	unsigned int treasureNo;
	unsigned char spEquipFlag[MAX_SPECIAL_EQUIP];
	unsigned char combSkillFlag[MAX_COMB_SKILL + WAR_POS_MAX];
	unsigned int godSkill[SKILL_MAX_PER_GOD];		//封神技能
	
	unsigned int ltAttack;	//攻击下限值
	unsigned int utAttack;	//攻击上限值
	unsigned int ltDefence;	//防御下限值
	unsigned int utDefence;	//防御上限值

	unsigned int ltBasicAttack;	//武将+副将+装备攻击下限值
	unsigned int utBasicAttack;	//武将+副将+装备攻击上限值
	unsigned int ltBasicDefence;	//武将+副将+装备防御下限值
	unsigned int utBasicDefence;	//武将+副将+装备防御上限值
};

struct t_CommItemInfo
{
	unsigned int roleId;
	unsigned int type;
	//unsigned int itemId;
	//unsigned short count;
};
struct t_EQUIPINFO
{
	unsigned int serialNo;	//装备流水号
	unsigned char type;
	unsigned int roleId;
	unsigned int itemId;
	//unsigned int guid;
	unsigned char warFlag;
	unsigned char curStar;
	unsigned char curLevel;
};

struct t_ChallengeRes
{
	//unsigned int otherUserId;
	char otherName[MAX_NAME_SIZE]; 
	unsigned char res;			//0 无挑战 1 胜利 2 失败 
	unsigned char defenceLevel;
	unsigned int attackAll;
	unsigned int attack[WAR_POS_MAX];
	unsigned int attackGodGuid[WAR_POS_MAX];
	unsigned int attackSkillGuid[WAR_POS_MAX][SKILL_MAX_PER_GOD];
	unsigned int defenceAll;
	unsigned int defence[WAR_POS_MAX];
	unsigned int defenceGodGuid[WAR_POS_MAX];
	unsigned int defenceSkillGuid[WAR_POS_MAX][SKILL_MAX_PER_GOD];
};

struct t_BattleInfo
{
	unsigned int roleId;
	unsigned short mainId;
	unsigned short curLeafId;
};

#endif
