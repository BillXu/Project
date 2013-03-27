#ifndef _START_SERVER__H_
#define _START_SERVER__H_

#include "../Base/IStartServer.h"
#include "../Base/ListenThread.h"
#include "../Base/ConnectThread.h"

class CStartServer :public IStartServer
{
public:
	CStartServer()
	{
		m_ListenClientPort = 0;
		m_ListenServerPort = 0;
		m_dbPort = 0;
		m_ClientPortForDB = 0;
		m_GameClientPort = 0;
		memset(&m_dbIP, 0, sizeof(m_dbIP));
		memset(m_szGameIP, 0, sizeof(m_szGameIP));
	}
	
	int SendToDB(char* pData, int len)
	{
		return m_ConnectDBThread.GetPeerHandler()->Send(pData, len);
	}
	
	int SendToGame(char* pData, int len)
	{
		RakNet::SystemAddress gameID(m_szGameIP, m_GameClientPort);
		return m_ListenGameThread.GetHandler(gameID)->Send(pData, len);
	}
	
	int SendToClient(uint64_t g, char* pData, int len)
	{
		return m_ListenClientThread.GetHandler(g)->Send(pData, len);
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

public:
	CListenThread m_ListenClientThread;
	CListenThread m_ListenGameThread;
private:
	CConnectThread m_ConnectDBThread;
	
	//ServerAddrParam m_AddrForClient;
	int m_ListenClientPort;
	int m_ListenServerPort;
	char m_dbIP[16];
	int m_dbPort;
	int m_ClientPortForDB;
	//GAME SERVER INFO
	char m_szGameIP[MAX_IP_LENGTH];
	unsigned short m_GameClientPort;
};

#define StartServer CStartServer::_instance()

#endif


