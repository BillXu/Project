#include "IStartServer.h"
//#include "helpFuncs.h"
//#include "LogManager.h"

extern void ConsoleProc();

/*int IStartServer::InitLogManager()
{
	return 0;
}*/

int IStartServer::InitConsole()
{
	return 0;
}

int IStartServer:: Run()
{
	int    nRet = -1;

	//初始化随机数种子
	//rand()不适合多线程
	srand( (unsigned)time(0));

	//1.初始化全局变量
	nRet = InitGlobalData();
	if(nRet)	return -1;
	//2.读取配置文件(包含服务器IP列表)
	nRet = ReadConfig();
	if(nRet)	return -1;
	//3.初始化LogManager配置 
	nRet=InitLogManager();
	//启动Log记录线程
	//MainLog->Start("Log");
	if(nRet)	return -1;
	//4.初始化服务器
	nRet=InitServer();
	if(nRet)	return -1;
	//5.初始化控制台
	nRet=InitConsole();
	if(nRet)	return -1;
	//6.启动所有的服务连接,通常为最后一个函数
	nRet=StartAllServiceConnections();
	if(!nRet)
	{
		//7.等待所有线程退出，处理控制台命令
		ConsoleProc();
		//8.所有线程都已停止，服务器最后退出时调用的函数，清理内存
		ClearMemory();
		//9.暂停一会
		sleep(SERVER_END_TIME_OUT);
	}

	return 0;
}

