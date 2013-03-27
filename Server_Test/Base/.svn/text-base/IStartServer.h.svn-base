#ifndef _I_START_SERVER__H_
#define _I_START_SERVER__H_
/**************************************************************
* 注意:
*      1.各服务器必须实现IStartServer类,并且定义#define StartServer 
*
**************************************************************/
#include <stdlib.h>
#include <time.h>
#include "../Share/Common.h"

class IStartServer
{
public:
	IStartServer(){};
	virtual ~IStartServer(){};

protected:
	 //读取配置文件
	virtual int ReadConfig() = 0;
	//初始化全局变量
	virtual int InitGlobalData() = 0;
	//初始化服务器
	virtual int InitServer() = 0;
	//初始化所有的服务连接
	virtual int InitAllServiceConnections() = 0;
	//启动所有的服务连接
	virtual int StartAllServiceConnections() = 0;
	 //所有线程都已停止,服务器最后退出时调用的函数,清理内存
	virtual int ClearMemory() = 0;

    //初始化LogManager配置
    virtual int   InitLogManager() = 0;
	//初始化控制台
	virtual int InitConsole();

public:
	virtual int Run();
};

//各服各自定义一个CStartServer类的单体 StartServer
#define StartServerPtrFunction   IStartServer*  StartServerPtr(){return StartServer;}

#endif


