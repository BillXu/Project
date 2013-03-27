#ifndef _START_DB_SERVER__H_
#define _START_DB_SERVER__H_

#include "../Base/IStartServer.h"
#include "../Base/ListenThread.h"

class CStartServer :public IStartServer
{
public:
	CStartServer()
	{
		m_ListenServerPort = 0;
		memset(&m_Conn, 0, sizeof(m_Conn));
		memset(&m_Conn2, 0, sizeof(m_Conn2));
		m_LoginClientPort = 0;
		m_GameClientPort = 0;
		memset(m_szLoginIP, 0, sizeof(m_szLoginIP));
		memset(m_szGameIP, 0, sizeof(m_szGameIP));
	}
	
	int SendToLogin(char* pData, int len)
	{
		RakNet::SystemAddress loginID(m_szLoginIP, m_LoginClientPort);
		return m_ListenServerThread.GetHandler(loginID)->Send(pData, len);
	}
	int SendToGame(char* pData, int len)
	{
		RakNet::SystemAddress gameID(m_szGameIP, m_GameClientPort);
		unsigned int nRet = m_ListenServerThread.GetHandler(gameID)->Send(pData, len);
		//printf("SendToGame %d bytes return %u\n", len, nRet);
		return nRet;
	}

	~CStartServer()
	{}

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

private:
	CListenThread m_ListenServerThread;
	unsigned short m_ListenServerPort;
	
	char m_szLoginIP[MAX_IP_LENGTH];
	unsigned short m_LoginClientPort;
	char m_szGameIP[MAX_IP_LENGTH];
	unsigned short m_GameClientPort;
	
	MysqlConnParam m_Conn;
	MysqlConnParam m_Conn2;
};

#define StartServer CStartServer::_instance()

#endif


