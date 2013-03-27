#ifndef _START_DB_SERVER__H_
#define _START_DB_SERVER__H_

#include "../Base/IStartServer.h"
#include "../Base/ListenThread.h"
#include "../Base/ConnectThread.h"

extern unsigned int g_GodSerialNoBegin;

class CStartServer :public IStartServer
{
public:
	CStartServer()
	{
		m_ListenClientPort = 0;
		m_LoginPort = 0;
		m_dbPort = 0;
		m_ClientPortForLogin = 0;
		m_ClientPortForDB = 0;
		memset(&m_LoginIP, 0, sizeof(m_LoginIP));
		memset(&m_dbIP, 0, sizeof(m_dbIP));
	}

	~CStartServer()
	{}
	
	int SendToDB(char* pData, int len)
	{
		return m_ConnectDBThread.GetPeerHandler()->Send(pData, len);
	}
	
	int SendToLogin(char* pData, int len)
	{
		return m_ConnectLoginThread.GetPeerHandler()->Send(pData, len);
	}
	
	int SendToClient(uint64_t g, char* pData, int len)
	{
		return m_ListenClientThread.GetHandler(g)->Send(pData, len);
	}

	//实例化本服务器的一个单体
	static CStartServer* _instance()
	{
		static CStartServer* __inst = NULL;
		if(!__inst)
		{
			__inst = new CStartServer;
		}
		return __inst;
	}
protected:
	//读取配置信息
	int ReadConfig();
	//初始化全局变量
	int InitGlobalData();
	//初始化log
	int InitLogManager();
	//初始化服务器
	int InitServer();
	//初始化服务器连接
	int InitAllServiceConnections();
	//启动服务器连接
	int StartAllServiceConnections();
	//服务器退出时,清理内存
	int ClearMemory();

public:
	CListenThread m_ListenClientThread;
private:
	CConnectThread m_ConnectLoginThread;
	CConnectThread m_ConnectDBThread;
	
	int m_ListenClientPort;
	char m_dbIP[16];
	int m_dbPort;
	int m_ClientPortForDB;
	char m_LoginIP[16];
	int m_LoginPort;
	int m_ClientPortForLogin;
};

#define StartServer CStartServer::_instance()

#endif


