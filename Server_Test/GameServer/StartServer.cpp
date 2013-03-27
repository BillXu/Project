#include <stdio.h>
#include "../Share/XMLScanner.h"
#include "../Share/Common.h"
#include "StartServer.h"
#include "ServerTreator.h"
#include "GameTreator.h"
#include "LevelData.h"
#include "RewardMgr.h"
#include "MissionMgr.h"
#include "ItemManager.h"
#include "SkillManager.h"
#include "GodOperateConfig.h"
#include "TimeUpdater.h"
#include "ExchangeConfig.h"
#include "ShopConfig.h"
#include "CompeteRankConfig.h"

StartServerPtrFunction

unsigned int g_GodSerialNoBegin;
//unsigned int g_uMiniVersion;
//unsigned int g_uMaxVersion;

//关闭服务器监听端口
int  CloseAcceptConnections();
//关闭服务器网络通讯
int ShutDownNet();

int CStartServer::ReadConfig()
{
	CXMLScanner xmlLS(SERVER_CONFIG_FILE);
	char *p1, *p2;
	if(1 != xmlLS.scan(0, "/BIGCUP/Global\0godSerialNoBegin\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	g_GodSerialNoBegin = atoi(p1);
	if(1 != xmlLS.scan(0, "/BIGCUP/GameServer/ListenClientPort\0value\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	m_ListenClientPort = atoi(p1);
	if(2 != xmlLS.scan(0, "/BIGCUP/DBServer\0ServerId\0IP\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	strcpy(m_dbIP, p2);
	if(1 != xmlLS.scan(0, "/BIGCUP/DBServer/ListenServerPort\0value\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	m_dbPort = atoi(p1);
	
	if(1 != xmlLS.scan(0, "/BIGCUP/LoginServer/ListenServerPort\0value\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	m_LoginPort = atoi(p1);
	if(2 != xmlLS.scan(0, "/BIGCUP/LoginServer\0ServerId\0IP\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	strcpy(m_LoginIP, p2);
	
	if(2 != xmlLS.scan(0, "/BIGCUP/GameServer/ClientPort\0LoginPort\0DBPort\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	m_ClientPortForLogin = atoi(p1);
	m_ClientPortForDB = atoi(p2);

	return 0;
}

int CStartServer::InitGlobalData()
{
	assert( CGodOperateConfigMgr::ShareGodOperateConfigMgr()->LoadFromFile(CONFER_CONFIG_FILE) );
	assert( CSkillMgr::SharedSkillMgr()->LoadFile(SKILL_CONFIG_FILE) );
	assert( CLevelDataManager::ShardLevelDataMgr()->LoadConfig(LEVEL_CONFIG_FILE) );
	assert( CRewardMgr::SharedRewardMgr()->ReadFromFile(REWARD_CONFIG_FILE) );
	assert( CMissionMgr::SharedMissionMgr()->ReadFromFile(TASK_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(GENERAL_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(EQUIP_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(ITEM1_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(ITEM2_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(ITEM3_CONFIG_FILE) );
	assert( CItemManager::SharedItemMgr()->LoadConfig(ITEM4_CONFIG_FILE) );
	assert( CExchangeConfigMgr::SharedExchangeMgr()->LoadFromFile(EXCHANGE_CONFIG_FILE) );
	assert( CShopConfigMgr::SharedConfigMgr()->LoadFromFile(SHOP_CONFIG_FILE) );
	//assert( CCompeteRankConfig::SharedMgr()->LoadConfigFile(TOP50_HONOR_CONFIG_FILE, true) );
	assert( CCompeteRankConfig::SharedMgr()->LoadConfigFile(TOP50_CONFIG_FILE, false) );
	
	return 0;
}

int CStartServer::InitAllServiceConnections()
{
	return 0;
}

int CStartServer::StartAllServiceConnections()
{
	return 0;
}

int CStartServer::InitLogManager()
{
	return 0;
}

int CStartServer::ClearMemory()
{
	return 0;
}

int CStartServer::InitServer()
{
	//开启处理器
	ServerTreator->Start();
	GameTreator->Start();
	GameTreator->InitPacketHandlerTable();
	sleep(1);
	//连接数据库
	m_ConnectDBThread.SetServerAddr(m_dbIP, m_dbPort);
	m_ConnectDBThread.SetClientPort(m_ClientPortForDB);
	m_ConnectDBThread.RegisterTreator(ServerTreator);
	m_ConnectDBThread.Start();
	sleep(1);
	//连接Login
	m_ConnectLoginThread.SetServerAddr(m_LoginIP, m_LoginPort);
	m_ConnectLoginThread.SetClientPort(m_ClientPortForLogin);
	m_ConnectLoginThread.RegisterTreator(ServerTreator);
	m_ConnectLoginThread.Start();
	sleep(1);
	//开启监听线程
	m_ListenClientThread.SetListenPort(m_ListenClientPort);
	m_ListenClientThread.RegisterTreator(GameTreator);
	m_ListenClientThread.Start();
	sleep(1);
	ServerTreator->InitSysData();
	//n毫秒更新一次
	TimeUpdater->SetUserUpdateTime(600);
	//n秒保存一次数据
	TimeUpdater->SetUserSaveTime(60);
	TimeUpdater->Start();
	
	return 0;
}

int CloseAcceptConnections()
{
	return 0;
}

int ShutDownNet()
{
	return 0;
}


