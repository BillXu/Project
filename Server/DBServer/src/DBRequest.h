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
	unsigned int nRequestFlag ; // 一般是msg,主要用来表示请求，用于对请求结果的处理. just ;
	char pSqlBuffer[Max_Sql_String];
	int nSqlBufferLen ;
protected:
	stDBRequest(){}
	friend class CDBRequestQueue;
};

struct stDBResult
{
public:
	typedef std::vector<CMysqlRow*> VEC_MYSQLROW ;
	~stDBResult();
public:
	unsigned int nRequestFlag ;
	// network form address ;
	VEC_MYSQLROW vResultRows ;  
	unsigned int nAffectRow ;
};

class CDBRequestQueue
{
public:
	typedef std::vector<stDBRequest*> VEC_DBREQUEST ;
	typedef std::vector<stDBResult*> VEC_DBRESULT ;
public:
	static CDBRequestQueue* SharedDBRequestQueue();
	CDBRequestQueue();
	~CDBRequestQueue();
	void PushRequest(stDBRequest* request );
	void PushResult(VEC_DBRESULT& result );
	void GetAllResult(VEC_DBRESULT& vAllReslutOut ) ; // delete stDBreslut after used ;
	void GetAllRequest(VEC_DBREQUEST& vAllRequestOut ); // push reserver after used DBRequest ;
	void PushReserveRequest(VEC_DBREQUEST& request );  // stDBRequest 对象使用完　不要delete ，而是push进来。thread safe ;
	stDBRequest* GetReserveRequest(); //  stDBReuest 不能直接new， 要通过此函数获取； thread safe ;
protected:
	void ClearAllRequest();
	void ClearAllResult();
	void ClearAllReserveRequest();
protected:
	Mutex mRequestLock ;
	Mutex mResultLock ;
	Mutex mReserveQuestLock ;
	VEC_DBREQUEST m_vReserveRequest ;
	VEC_DBREQUEST m_vAllRequest ;
	VEC_DBRESULT m_vAllResult ;
};