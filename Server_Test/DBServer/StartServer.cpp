#include <stdio.h>
#include "../Share/XMLScanner.h"
#include "../Share/Common.h"
#include "../DB/MysqlClient.h"
#include "StartServer.h"
#include "DBTreator.h"
//#include "MsgParser.h"

static uint32_t MysqlHashCode(uint32_t wdHashCode)
{ 
	return wdHashCode;
}

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
	char* p1, *p2;
	if(1 != xmlLS.scan(0, "/BIGCUP/DBServer/ListenServerPort\0value\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	m_ListenServerPort = atoi(p1);

	if(2 != xmlLS.scan(0, "/BIGCUP/DBServer/Mysql\0hash\0conn\0", 0, &p1, &p2))
	{
		assert(0);
	}
	m_Conn.hash = atoi(p1);
	strcpy(m_Conn.conn, p2);
	if(2 != xmlLS.scan(0, "/BIGCUP/DBServer/Mysql2\0hash\0conn\0", 0, &p1, &p2))
	{
		assert(0);
	}
	m_Conn2.hash = atoi(p1);
	strcpy(m_Conn2.conn, p2);
	
	if(2 != xmlLS.scan(0, "/BIGCUP/LoginServer\0ServerId\0IP\0", 0, &p1, &p2))
	{
		assert(0);
		return -1;
	}
	strcpy(m_szLoginIP, p2);
	if(1 != xmlLS.scan(0, "/BIGCUP/LoginServer/ClientPort\0DBPort\0", 0, &p1))
	{
		assert(0);
		return -1;
	}
	m_LoginClientPort = atoi(p1);
	
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
	m_GameClientPort = atoi(p2);

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
	if( MysqlPool->SetConnectString(m_Conn.hash, m_Conn.conn) )
	{
		printf("Info: Set Mysql %d conn : %s\n", m_Conn.hash, m_Conn.conn);
	}
	if( MysqlPool->SetConnectString(m_Conn2.hash, m_Conn2.conn) )
	{
		printf("Info: Set Mysql %d conn : %s\n", m_Conn2.hash, m_Conn2.conn);
	}
	//初始化数据库连接池 不支持事务
	if(!MysqlPool->InitByHashCodeFunc(false, MysqlHashCode))
	{
		printf("Error:Initialize link to database failed\n");
		return -1;
	}
	//开启处理器
	DBTreator->Start();
	//开启监听线程
	m_ListenServerThread.SetListenPort(m_ListenServerPort);
	m_ListenServerThread.RegisterTreator(DBTreator);
	m_ListenServerThread.Start();
	
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


