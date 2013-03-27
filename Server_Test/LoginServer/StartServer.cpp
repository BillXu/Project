#include <stdio.h>
#include "../Share/XMLScanner.h"
#include "../Share/Common.h"
#include "StartServer.h"
#include "LoginTreator.h"
#include "ServerTreator.h"
//#include "MsgParser.h"

StartServerPtrFunction

unsigned int g_uMiniVersion;
unsigned int g_uMaxVersion;

//关闭服务器监听端口
int  CloseAcceptConnections();
//关闭服务器网络通讯
int ShutDownNet();

int CStartServer::ReadConfig()
{
	CXMLScanner xmlLS(SERVER_CONFIG_FILE);
	char* p;
	if(1 != xmlLS.scan(0, "/BIGCUP/LoginServer/ListenClientPort\0value\0", 0, &p))
	{
		assert(0);
		return -1;
	}
	m_ListenClientPort = atoi(p);
	if(1 != xmlLS.scan(0, "/BIGCUP/LoginServer/ListenServerPort\0value\0", 0, &p))
	{
		assert(0);
		return -1;
	}	
	m_ListenServerPort = atoi(p);
	if(1 != xmlLS.scan(0, "/BIGCUP/LoginServer/ClientPort\0DBPort\0", 0, &p))
	{
		assert(0);
		return -1;
	}	
	m_ClientPortForDB = atoi(p);
	
	if(1 != xmlLS.scan(0, "/BIGCUP/DBServer/ListenServerPort\0value\0", 0, &p))
	{
		assert(0);
		return -1;
	}
	m_dbPort = atoi(p);
	if(1 != xmlLS.scan(0, "/BIGCUP/DBServer\0IP\0", 0, &p))
	{
		assert(0);
		return -1;
	}
	strcpy(m_dbIP, p);
	
	char* p1, *p2;
	if(2 != xmlLS.scan(0, "/BIGCUP/GameServer\0ServerId\0IP\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	strcpy(m_szGameIP, p2);
	if(2 != xmlLS.scan(0, "/BIGCUP/GameServer/ClientPort\0LoginPort\0DBPort\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	m_GameClientPort = atoi(p1);
	
	return 0;
}

int CStartServer::InitGlobalData()
{
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
	LoginTreator->Start();
	ServerTreator->Start();
	sleep(1);
	//开启监听线程
	m_ListenClientThread.SetListenPort(m_ListenClientPort);
	m_ListenClientThread.RegisterTreator(LoginTreator);
	m_ListenClientThread.Start();
	sleep(1);
	m_ListenGameThread.SetListenPort(m_ListenServerPort);
	m_ListenGameThread.RegisterTreator(ServerTreator);
	m_ListenGameThread.Start();
	sleep(1);
	//连接数据库
	m_ConnectDBThread.SetServerAddr(m_dbIP, m_dbPort);
	m_ConnectDBThread.SetClientPort(m_ClientPortForDB);
	m_ConnectDBThread.RegisterTreator(ServerTreator);
	m_ConnectDBThread.Start();
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


