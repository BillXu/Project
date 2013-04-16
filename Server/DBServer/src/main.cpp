#include"main.h"
#include <iostream>
#include "ThreadMod.h"
#include "mutex.h"
#include <my_global.h>
#include "mysql.h"
#include "DataBaseThread.h"
#include "MySqlData.h"
#include "DBRequest.h"
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "ServerNetwork.h"
#include "DBPlayerManager.h"
class A
	:public CThreadT
{
public:
	void __run()
	{
		printf("this function \n") ;
		while ( 1 )
		{
			ShowResult();
			Sleep(30);
		}
	}

	void ShowResult()
	{
		CDBRequestQueue::VEC_DBRESULT vResultOut ;
		CDBRequestQueue::SharedDBRequestQueue()->GetAllResult(vResultOut) ;
		CDBRequestQueue::VEC_DBRESULT::iterator iter = vResultOut.begin() ;
		for ( ; iter != vResultOut.end(); ++iter )
		{
			stDBResult* pRet = *iter ;
			printf( "thie %d line affected\n", pRet->nAffectRow) ;
			stDBResult::VEC_MYSQLROW::iterator iter_Row = pRet->vResultRows.begin() ;
			for ( ; iter_Row != pRet->vResultRows.end(); ++iter_Row )
			{
				CMysqlRow& Row = *(*iter_Row) ;
				printf( "RowInfo ID = %d , name = %s , money = %lld , Rate = %f \n",Row["ID"]->Value.iValue, Row["name"]->Value.pBuffer,Row["money"]->Value.llValue,Row["Rate"]->Value.fValue);
			}
			delete pRet ;
		}
		vResultOut.clear();
	}

	void TestFunc()
	{
	  MYSQL *conn; 
	  MYSQL_RES *result; 
	  MYSQL_ROW row;  
	  int num_fields; 
	  int i;  
	  conn = new MYSQL;
	  conn = mysql_init(NULL);  
	  if ( !mysql_real_connect(conn, "localhost", "root", "123456", "sakila", 3307, NULL, 0) )
	  {
		 fprintf(stderr, "Failed to connect to database: Error: %s\\n",         mysql_error(conn));
		printf("error\n") ;
		return ;
	  }
	  mysql_query(conn, "SELECT * FROM TestTable"); 
	  result = mysql_store_result(conn); 
	  num_fields = mysql_num_fields(result); 
	  while ((row = mysql_fetch_row(result))) 
	  {    
			for(i = 0; i < num_fields; i++)      
			{        
				printf("%s ", row[i] ? row[i] : "NULL");      
			}   
			 printf("\n"); 
	  } 
	  mysql_free_result(result);
	  mysql_close(conn);
	}

	int a ; 
};

class B
:public CServerNetworkDelegate
{
public:
	virtual bool OnMessage( RakNet::Packet* pData )
	{
		unsigned char* pDataNew = pData->data ;
		printf("Recieve a Number = %d \n",*((int*)&pDataNew[1]));
		return false ;
	}

	virtual void OnNewPeerConnected(RakNet::RakNetGUID& nNewPeer, RakNet::Packet* pData )
	{
		printf("a peer connected : %s\n",pData->systemAddress.ToString());
	}
	
	virtual void OnPeerDisconnected(RakNet::RakNetGUID& nPeerDisconnected, RakNet::Packet* pData )
	{
		printf("a peer Disconnected : %s\n",nPeerDisconnected.ToString());
	}
};

BOOL WINAPI ConsoleHandler(DWORD msgType)
{    
	CServerNetwork::SharedNetwork()->ShutDown(); 
	return TRUE;
} 

int main()
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE); 
	CDataBaseThread::SharedDBThread()->InitDataBase("localHost",3307,"root","123456","gamedb");
	CDataBaseThread::SharedDBThread()->Start() ;

	//A b ;
	//b.Start();
	CServerNetwork::SharedNetwork()->StartupNetwork(8000,5);
	CDBPlayerManager DBMgr ;
	CServerNetwork::SharedNetwork()->AddDelegate(&DBMgr);
	while ( true )
	{
		CServerNetwork::SharedNetwork()->RecieveMsg();
		DBMgr.ProcessDBResults();
		Sleep(5);
	}
	return 0 ; 
}