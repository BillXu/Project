#ifndef _RECORD_STRUCT_DEFINE__H_
#define _RECORD_STRUCT_DEFINE__H_

#include "../DB/MysqlClient.h"
//#include "../Share/Common.h"

#pragma pack(1)
//L2D_LOGIN
static mysqlCol User_Define[] =
{
	{"id", MYSQL_DWORD, sizeof(uint32_t)},
	{"name", MYSQL_STR, MAX_NAME_SIZE},
	{"password", MYSQL_STR, MAX_PASS_SIZE},
	{"onlineFlag", MYSQL_BYTE, sizeof(uint8_t)},
	{"email", MYSQL_STR, MAX_EMAIL_SIZE},
	{"mobile", MYSQL_STR, MAX_MOBILE_LEN},
	{NULL, 0, 0}
};

struct t_SysInitRecord
{
	uint32_t maxSerialNo;
};
static mysqlCol SysInit_Define[] =
{
	{"maxSerialNo", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};

struct t_UserIdRecord
{
	uint32_t userId;
};
static mysqlCol UserId_Define[] =
{
	{"userId", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};

struct t_UserRecord
{
	uint32_t id;
	char name[MAX_NAME_SIZE];
	char password[MAX_PASS_SIZE];
	unsigned char onlineFlag;
	char email[MAX_EMAIL_SIZE];
	char mobile[MAX_MOBILE_LEN];
};
static mysqlCol Role_Define[] =
{
	{"userId", MYSQL_DWORD, sizeof(uint32_t)},
	{"roleName", MYSQL_STR, MAX_NAME_SIZE},
	{"exp", MYSQL_DWORD, sizeof(uint32_t)},
	{"level", MYSQL_WORD, sizeof(uint16_t)},
	{"coin", MYSQL_DWORD, sizeof(uint32_t)},
	{"gold", MYSQL_DWORD, sizeof(uint32_t)},
	{"vitality", MYSQL_DWORD, sizeof(uint32_t)},
	{"ability", MYSQL_DWORD, sizeof(uint32_t)},
	{"psychicPower", MYSQL_DWORD, sizeof(uint32_t)},
	{"honor", MYSQL_DWORD, sizeof(uint32_t)},
	{"curTask", MYSQL_WORD, sizeof(uint16_t)},
	{"leftChallengeTimes", MYSQL_BYTE, sizeof(uint8_t)},
	//{"competitionTimes", MYSQL_WORD, sizeof(uint16_t)},
	{"wonTimes", MYSQL_WORD, sizeof(uint16_t)},
	{"challengeWonTimes", MYSQL_DWORD, sizeof(uint32_t)},
	//{"compDoubleWonTimes", MYSQL_DWORD, sizeof(uint32_t)},
	//{"attackTimes", MYSQL_WORD, sizeof(uint16_t)},
	//{"defenceTimes", MYSQL_WORD, sizeof(uint16_t)},
	//{"attackWonTimes", MYSQL_WORD, sizeof(uint16_t)},
	//{"defenceWonTimes", MYSQL_WORD, sizeof(uint16_t)},
	{"lastHonorExcTime", MYSQL_DWORD, sizeof(uint32_t)},
	{"lastActiveTime", MYSQL_DWORD, sizeof(uint32_t)},
	{"oldTopId", MYSQL_DWORD, sizeof(uint32_t)},
	{"newTopId", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_RoleRecord
{
	unsigned int userId;			//玩家id
	char roleName[MAX_NAME_SIZE];	//角色名
	unsigned int exp;				//人物的当前等级
	unsigned short level;			//当前拥有经验
	unsigned int coin;				//当前拥有的金币
	unsigned int gold;				//当前拥有的元宝
	unsigned int vitality;			//当前拥有的活力值
	unsigned int ability;			//能力值
	unsigned int psychicPower;		//灵力值
	unsigned int honor;				//玩家拥有的荣誉值
	unsigned short curTask;			//当前任务id
	unsigned char leftChallengeTimes;	//剩余的挑战次数
	//unsigned short competitionTimes;//累积的竞技场竞技次数
	unsigned short wonTimes;	//胜的次数
	unsigned short challengeWonTimes;	//挑战任务中胜利次数
	//unsigned short compDoubleWonTimes;	//竞技场连胜次数
	//unsigned short attackTimes;		//主动攻击的次数
	//unsigned short defenceTimes;	//被动防御的次数
	//unsigned short attackWonTimes;	//主动攻击胜利次数
	//unsigned short defenceWonTimes;	//被动防御胜利次数
	unsigned int lastHonorExcTime;		//最新荣誉兑换时间
	unsigned int lastActiveTime;
	unsigned int oldTopId;
	unsigned int newTopId;
};

static mysqlCol Task_Define[] =
{
	{"userId", MYSQL_DWORD, sizeof(uint32_t)},
	{"mainId", MYSQL_WORD, sizeof(uint16_t)},
	{"leafId", MYSQL_WORD, sizeof(uint16_t)},
	{"leftTimes", MYSQL_BYTE, sizeof(uint8_t)},
	{NULL, 0, 0}
};

struct t_TaskRecord
{
	unsigned int userId;	//玩家id
	unsigned short mainId;	//主任务id
	unsigned short leafId;	//子任务id
	unsigned char leftTimes;//剩余可执行次数
};

static mysqlCol General_Define[] =
{
	{"serialNo", MYSQL_DWORD, sizeof(uint32_t)},
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"itemId", MYSQL_DWORD, sizeof(uint32_t)},
	{"guid", MYSQL_DWORD, sizeof(uint32_t)},
	{"warFlag", MYSQL_BYTE, sizeof(uint8_t)},
	{"group", MYSQL_BYTE, sizeof(uint8_t)},
	{"curStar", MYSQL_BYTE, sizeof(uint8_t)},
	{"curLevel", MYSQL_BYTE, sizeof(uint8_t)},
	{"godSkill", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_GeneralRecord
{
	unsigned int serialNo;	//流水号
	unsigned int roleId;	//玩家id
	unsigned int itemId;	//武将id
	unsigned int guid;		//武将guid
	unsigned char warFlag;	//是否出战
	unsigned char group;	//阵营
	unsigned char curStar;	//当前星级
	unsigned char curLevel;	//星级等级
	unsigned int godSkill;//封神技
};

static mysqlCol Equip_Define[] =
{
	{"serialNo", MYSQL_DWORD, sizeof(uint32_t)},
	{"type", MYSQL_BYTE, sizeof(uint8_t)},
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"guid", MYSQL_DWORD, sizeof(uint32_t)},
	{"itemId", MYSQL_DWORD, sizeof(uint32_t)},
	{"warFlag", MYSQL_BYTE, sizeof(uint8_t)},
	{"curStar", MYSQL_BYTE, sizeof(uint8_t)},
	{"curLevel", MYSQL_BYTE, sizeof(uint8_t)},
	{NULL, 0, 0}
};
struct t_EquipRecord
{
	unsigned int serialNo;	//流水号
	unsigned char type;		//类型
	unsigned int roleId;	//玩家id
	unsigned int guid;		//guid
	unsigned int itemId;	//id
	unsigned char warFlag;	//是否出战
	unsigned char curStar;	//当前星级
	unsigned char curLevel;	//星级等级
};

static mysqlCol Item_Define[] =
{
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"type", MYSQL_BYTE, sizeof(uint8_t)},
	{"itemId", MYSQL_DWORD, sizeof(uint32_t)},
	{"count", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_ItemRecord
{
	unsigned int roleId;	//玩家id
	unsigned char type;		//类型
	unsigned int itemId;	//id
	unsigned int count;		//
};

static mysqlCol Relation_Define[] =
{
	{"roleId1", MYSQL_DWORD, sizeof(uint32_t)},
	{"roleId2", MYSQL_DWORD, sizeof(uint32_t)},
	{"type", MYSQL_BYTE, sizeof(uint8_t)},
	{"addTime", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_RelationRecord
{
	unsigned int roleId1;	//
	unsigned int roleId2;	//
	unsigned char type;		//
	unsigned int addTime;
};

static mysqlCol ShopHistory_Define[] =
{
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"guid", MYSQL_DWORD, sizeof(uint32_t)},
	{"count", MYSQL_WORD, sizeof(uint16_t)},
	{"priceType", MYSQL_BYTE, sizeof(uint8_t)},
	{"price", MYSQL_DWORD, sizeof(uint32_t)},
	{"lastTime", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_ShopHistory
{
	unsigned int roleId;	//
	unsigned int guid;		//
	unsigned short count;
	unsigned char priceType;
	unsigned int price;		//
	unsigned int lastTime;
};

static mysqlCol TopInfo_Define[] =
{
	{"topId", MYSQL_DWORD, sizeof(uint32_t)},
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"lastTime", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_TopInfo
{
	unsigned int topId;		//
	unsigned int roleId;	//
	unsigned int lastTime;
};

static mysqlCol ChallengeInfo_Define[] =
{
	{"myRoleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"times", MYSQL_WORD, sizeof(uint16_t)},
	{"type", MYSQL_BYTE, sizeof(uint8_t)},
	{"lastTime", MYSQL_DWORD, sizeof(uint32_t)},
	{NULL, 0, 0}
};
struct t_ChallengeInfo
{
	unsigned int myRoleId;
	unsigned int roleId;
	unsigned short times;
	unsigned char type;
	unsigned int lastTime;	//最近更新时间
};

static mysqlCol ChallTaskInfo_Define[] =
{
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"subTaskId", MYSQL_WORD, sizeof(uint16_t)},
	{"roleId2", MYSQL_DWORD, sizeof(uint32_t)},
	{"flag", MYSQL_BYTE, sizeof(uint8_t)},
	{NULL, 0, 0}
};
struct t_ChallTaskInfo
{
	unsigned int roleId;
	unsigned short subTaskId;
	unsigned int roleId2;
	unsigned char flag;
};

static mysqlCol BattleInfo_Define[] =
{
	{"roleId", MYSQL_DWORD, sizeof(uint32_t)},
	{"mainId", MYSQL_WORD, sizeof(uint16_t)},
	{"curLeafId", MYSQL_WORD, sizeof(uint16_t)},
	{NULL, 0, 0}
};
struct t_BattleInfo
{
	unsigned int roleId;
	unsigned short mainId;
	unsigned short curLeafId;
};

#pragma pack()
#endif


