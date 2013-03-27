#ifndef _INTERNAL_MSG__H_
#define _INTERNAL_MSG__H_

#include "../../Common/MessageDefine.h"
#include <stdlib.h>
#include <stdint.h>

//packet第一个字节 标识消息方向
enum
{
	INMSG_BEGIN = ID_USER_PACKET_ENUM+18,//131+18
	INMSG_CLIENT_LOGIN,	//150开始
	INMSG_LOGIN_CLIENT,
	INMSG_CLIENT_GAME,
	INMSG_GAME_CLIENT,
	INMSG_LOGIN_GAME,
	INMSG_GAME_LOGIN,
	INMSG_LOGIN_DB,
	INMSG_DB_LOGIN,
	INMSG_GAME_DB,
	INMSG_DB_GAME,

	INMSG_SYS,
	INMSG_END
};

//所有的系统消息类型枚举
enum
{
	SYS_SAVE = 0,
	
	SYS_END
};
enum
{
	CMD_SERVER_BEGIN = SYS_END+1,
	G2D_SYS_INIT = CMD_SERVER_BEGIN,
	D2G_SYS_INIT,
	D2G_SYS_LOAD_ALL,	//加载所有角色信息
	D2G_SYS_INIT_END,	//加载信息完毕
	L2D_LOGIN,
	D2L_LOGIN,
	D2L_USER_INFO,
	L2G_LOGIN,
	
	G2D_USER_CKECK,
	D2G_USER_CKECK,
	G2D_SAVE_ROLE_BASE,
	G2D_SAVE_ROLE,
	G2D_SAVE_GENERAL,
	D2G_GENERAL,
	G2D_SAVE_EQUIP,
	D2G_EQUIP,
	G2D_SAVE_ITEM,
	D2G_ITEM,
	G2D_SAVE_RELATION,
	D2G_RELATION,
	G2D_SAVE_SHOP_HISTORY,
	D2G_SHOP_HISTORY,
	G2D_SAVE_TOP_INFO,
	D2G_TOP_INFO,
	G2D_SAVE_CHALLENGE_INFO,
	D2G_CHALLENGE_INFO,
	G2D_SAVE_CHALL_TASK_INFO,
	D2G_CHALL_TASK_INFO,
	G2D_SAVE_BATTLE_INFO,
	D2G_BATTLE_INFO,

	L_D_END
};


#pragma pack(1)
class  CHandler;
struct  InternalMsg
{
	InternalMsg()
	{
		pHandler = NULL;
		pMsg = NULL;
	}
	~InternalMsg()
	{
		pHandler = NULL;
		if(pMsg) free(pMsg);
		pMsg = NULL;
	}
	CHandler* 	pHandler;
	//uint64_t 	guid;
	int			nMsgLen;
	stMsg*		pMsg; 
};

enum eCMD_TYPE
{
	SUB_SAVE_DATA = 1,	//定时保存数据
	SUB_TIME1,			//挑战任务每日更新
	SUB_TIME2,			//排行榜每周奖励
	SUB_RESET_BATTLE_TIME, //重置副本时间(每日凌晨三点)

	SUB_END
};
struct stServerMsg :public stMsg
{ 
	int cmdType;		//
	uint64_t guid;
	stServerMsg(int type = SUB_SAVE_DATA)
	{
		cmdType = type;
		guid = 0;
	}
};

/*struct stSysMsg:public stServerMsg
{
	unsigned short subType;	
	stSysMsg(unsigned short sub){subType = sub; cmdType = INMSG_SYS;}
};*/

//LOGIN
struct stL2DLogin
:public stServerMsg
{
    short nSelLen;
	char* select;
    stL2DLogin(){cSysIdentifer = INMSG_LOGIN_DB;}
};
//LOGIN err
struct stD2LLoginErr
:public stServerMsg
{
    unsigned char nErr;
    stD2LLoginErr(){cSysIdentifer = INMSG_DB_LOGIN;}
};
//USER
struct stUserInfo
{
	unsigned int id;
	short nameLen;
	char* name;
	short pwdLen;
	char* pwd;
	unsigned char onlineFlag;
	short emailLen;
	char* email;
	short mobileLen;
	char* mobile;
};
struct stRoleInfo
{
	unsigned int userId;	//玩家id
	short roleNameLen;		//角色名长度
	char* pRoleName;		//角色名
	unsigned int exp;		//当前拥有经验
	unsigned char level;	//人物的当前等级
	unsigned int coin;		//当前拥有的金币
	unsigned int gold;		//当前拥有的元宝
	unsigned short vitality;	//当前拥有的活力值
	unsigned int ability;	//能力值
	unsigned int psychicPower;//灵力值
	unsigned int honor;	//玩家拥有的荣誉值
	unsigned short curTask;	//当前任务id
	unsigned char leftChallengeTimes;	//剩余的挑战次数
	unsigned short wonTimes;//竞技场中胜利次数(战功 可兑换成荣誉值)
	unsigned short challengeWonTimes;	//挑战任务中胜利次数
	//unsigned short compDoubleWonTimes;	//竞技场连胜次数
	//unsigned short competitionTimes;//累积的竞技场竞技次数
	//unsigned short attackTimes;		//主动攻击的次数
	//unsigned short defenceTimes;	//被动防御的次数
	//unsigned short attackWonTimes;	//主动攻击胜利次数
	//unsigned short defenceWonTimes;	//被动防御胜利次数
	unsigned int lastHonorExcTime;
	unsigned int lastActiveTime;	//最新活跃时间
	unsigned int oldTopId;
	unsigned int newTopId;
	
};
struct stTaskInfo
{
	unsigned int userId;
	unsigned short mainId;
	unsigned short leafId;
	unsigned char leftTimes;
	unsigned char changeFlag;
};

struct stGeneralInfo
{
	unsigned char changeFlag;
	unsigned int serialNo;
	unsigned int roleId;
	unsigned int itemId;
	unsigned int guid;
	unsigned char warFlag;
	unsigned char group;
	unsigned char curStar;
	unsigned char curLevel;
	unsigned int godSkill;	//封神技能
};

struct stEquipInfo
{
	unsigned char changeFlag;
	unsigned int serialNo;
	unsigned char type;
	unsigned int roleId;
	unsigned int guid;
	unsigned int itemId;
	unsigned char warFlag;
	unsigned char curStar;
	unsigned char curLevel;
};

struct stItemInfo
{
	unsigned char changeFlag;
	unsigned int roleId;
	unsigned char type;		// 3 材料; 4 回复活力值道具; 5 增加挑战次数道具; 6 礼包;
	unsigned int itemId;
	unsigned int count;
};

struct stRelationInfo
{
	unsigned char changeFlag;
	unsigned char type;
	unsigned int roleId;
	unsigned int addTime;
};

struct stShopHistoryInfo
{
	unsigned char changeFlag;
	unsigned int roleId;
	unsigned int guid;
	unsigned short count;
	unsigned char priceType;
	unsigned int price;
	unsigned int lastTime;	//最近更新时间
};
struct stTopInfo
{
	unsigned char changeFlag;
	unsigned int topId;
	unsigned int roleId;
	unsigned int lastTime;	//最近更新时间
};
struct stChallengeInfo
{
	unsigned char changeFlag;
	unsigned int myRoleId;
	unsigned int roleId;
	unsigned short times;
	unsigned char type;
	unsigned int lastTime;	//最近更新时间
};
struct stChallTaskInfo
{
	unsigned char changeFlag;
	unsigned int roleId;
	unsigned short subTaskId;//0~9
	unsigned int roleId2;	//挑战对象
	unsigned char flag;		//1 完成 0 未完成
};

struct stBattleInfo
{
	unsigned char changeFlag;
	unsigned int roleId;
	unsigned short mainId;
	unsigned short curLeafId;
};

//服务器启动时信息
struct stG2DSys:public stServerMsg
{
	unsigned int maxSerialNo;		//当前最大流水号
	stG2DSys(){cSysIdentifer = INMSG_GAME_DB; cMsgID = G2D_SYS_INIT;};
};
struct stD2GSys:public stServerMsg
{
	unsigned int maxSerialNo;		//当前最大流水号
	stD2GSys(){cSysIdentifer = INMSG_DB_GAME; cMsgID = D2G_SYS_INIT;};
};
struct stD2GLoadAllUser:public stServerMsg
{
	unsigned short count;		//
	unsigned int* pList;
	stD2GLoadAllUser(){cSysIdentifer = INMSG_DB_GAME; cMsgID = D2G_SYS_LOAD_ALL;};
};

struct stD2LUser:public stServerMsg
{
	//int count;
	stUserInfo user;
	stRoleInfo role;
	stD2LUser(){cSysIdentifer = INMSG_DB_LOGIN;}
};

struct stL2G_Login:public stServerMsg
{
	unsigned int checkNo;
	stRoleInfo role;
	stL2G_Login(){cSysIdentifer = INMSG_LOGIN_GAME;}
};

//select
struct stG2D_Select
:public stServerMsg
{
    short nSelLen;
	char* select;
    stG2D_Select(){cSysIdentifer = INMSG_GAME_DB;}
};
struct stD2G_Role:public stServerMsg
{
	stRoleInfo role;
	stD2G_Role(){cSysIdentifer = INMSG_DB_GAME;}
};

/*struct stString
{
	int strLen;
	char* pStr;
};*/
//G2D_SAVE_ROLE_BASE
struct stG2D_SaveRoleBase : public stServerMsg
{
	stRoleInfo role;
	stG2D_SaveRoleBase(){cSysIdentifer = INMSG_GAME_DB;cMsgID = G2D_SAVE_ROLE_BASE;}
};
//G2D_SAVE_ROLE
struct stG2D_SaveRole : public stServerMsg
{
	stRoleInfo role;
	short taskCount;
	stTaskInfo* pTask;
	stG2D_SaveRole(){cSysIdentifer = INMSG_GAME_DB;}
};
//G2D_SAVE_GENERAL
struct stG2D_SaveGeneral : public stServerMsg
{
	short count;
	stGeneralInfo* pGeneral;
	stG2D_SaveGeneral(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_GENERAL
struct stD2G_General : public stServerMsg
{
	unsigned int userId;
	short count;
	stGeneralInfo* pGeneral;
	stD2G_General(){cSysIdentifer = INMSG_GAME_DB;}
};
//G2D_SAVE_EQUIP
struct stG2D_SaveEquip : public stServerMsg
{
	short count;
	stEquipInfo* pList;
	stG2D_SaveEquip(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_EQUIP
struct stD2G_Equip : public stServerMsg
{
	unsigned int userId;
	short count;
	stEquipInfo* pEquip;
	stD2G_Equip(){cSysIdentifer = INMSG_GAME_DB;}
};
//G2D_SAVE_ITEM
struct stG2D_SaveItem : public stServerMsg
{
	short count;
	stItemInfo* pList;
	stG2D_SaveItem(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_ITEM
struct stD2G_Item : public stServerMsg
{
	unsigned int userId;
	short count;
	stItemInfo* pItem;
	stD2G_Item(){cSysIdentifer = INMSG_GAME_DB;}
};
//G2D_SAVE_RELATION
struct stG2D_SaveRelation : public stServerMsg
{
	unsigned int userId;
	short count;
	stRelationInfo* pList;
	stG2D_SaveRelation(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_RELATION
struct stD2G_RELATION : public stServerMsg
{
	unsigned int userId;
	short count;
	stRelationInfo* pList;
	stD2G_RELATION(){cSysIdentifer = INMSG_GAME_DB;}
};

//G2D_SAVE_SHOP_HISTORY
struct stG2D_SaveShopHistory : public stServerMsg
{
	unsigned int userId;
	short count;
	stShopHistoryInfo* pList;
	stG2D_SaveShopHistory(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_SHOP_HISTORY
struct stD2G_ShopHistory : public stServerMsg
{
	unsigned int userId;
	short count;
	stShopHistoryInfo* pList;
	stD2G_ShopHistory(){cSysIdentifer = INMSG_GAME_DB;}
};
//G2D_SAVE_TOP_INFO
struct stG2D_SaveTopInfo : public stServerMsg
{
	short count;
	stTopInfo* pList;
	stG2D_SaveTopInfo(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_TOP_INFO
struct stD2G_TopInfo : public stServerMsg
{
	unsigned int userId;
	short count;
	stTopInfo* pList;
	stD2G_TopInfo(){cSysIdentifer = INMSG_GAME_DB;}
};

//G2D_SAVE_CHALLENGE_INFO
struct stG2D_SaveChallenge : public stServerMsg
{
	unsigned int userId;
	short count;
	stChallengeInfo* pList;
	stG2D_SaveChallenge(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_CHALLENGE_INFO
struct stD2G_Challenge : public stServerMsg
{
	unsigned int userId;
	short count;
	stChallengeInfo* pList;
	stD2G_Challenge(){cSysIdentifer = INMSG_GAME_DB;}
};

//G2D_SAVE_CHALL_TASK_INFO
struct stG2D_SaveChallTask : public stServerMsg
{
	unsigned int userId;
	short count;
	stChallTaskInfo* pList;
	stG2D_SaveChallTask(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_CHALL_TASK_INFO
struct stD2G_ChallTask : public stServerMsg
{
	unsigned int userId;
	short count;
	stChallTaskInfo* pList;
	stD2G_ChallTask(){cSysIdentifer = INMSG_GAME_DB;}
};

//G2D_SAVE_BATTLE_INFO
struct stG2D_SaveBattle : public stServerMsg
{
	unsigned int userId;
	short count;
	stBattleInfo* pList;
	stG2D_SaveBattle(){cSysIdentifer = INMSG_GAME_DB;}
};
//D2G_BATTLE_INFO
struct stD2G_Battle : public stServerMsg
{
	unsigned int userId;
	short count;
	stBattleInfo* pList;
	stD2G_Battle(){cSysIdentifer = INMSG_GAME_DB;}
};

#pragma pack()
#endif

