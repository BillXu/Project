#pragma once 
#ifndef Max_Sql_String
#define Max_Sql_String 1024*2 
#endif 
#include "MySqlData.h"
#include <vector>
#include "mutex.h"
enum eDBRequestType
{
	eRequestType_Add,
	eRequestType_Delete,
	eRequestType_Update,
	eRequestType_Select,
	eRequestType_Max,
};

struct stDBRequest
{
	eDBRequestType eType ;
	// network form address ;
	unsigned int nRequestFlag ; // 一般是msg,主要用来表示请求，用于对请求结果的处理.
	char pSqlBuffer[Max_Sql_String];
	int nSqlBufferLen ;
};

struct stDBResult
{
public:
	typedef std::vector<CMysqlRow> VEC_MYSQLROW ;
public:
	unsigned int nRequestFlag ;
	// network form address ;
	VEC_MYSQLROW vResultRows ;  
};

class CDBRequestQueue
{
public:
	typedef std::vector<stDBRequest*> VEC_DBREQUEST ;
	typedef std::vector<stDBResult*> VEC_DBRESULT ;
public:
	CDBRequestQueue();
	~CDBRequestQueue();
	void PushRequest(stDBRequest* request );
	void PushResult(stDBResult* result );
	void GetAllResult(VEC_DBRESULT& vAllReslut ) ;
	void GetAllRequest(VEC_DBREQUEST& vAllRequest );
protected:
	void ClearAllRequest();
	void ClearAllResult();
protected:
	Mutex mRequest ;
	Mutex mResult ;
	VEC_DBREQUEST vAllRequest ;
	VEC_DBRESULT vAllResult ;
};