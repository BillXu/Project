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
	unsigned int nRequestFlag ; // һ����msg,��Ҫ������ʾ�������ڶ��������Ĵ���. just ;
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
	void PushReserveRequest(VEC_DBREQUEST& request );  // stDBRequest ����ʹ���ꡡ��Ҫdelete ������push������thread safe ;
	stDBRequest* GetReserveRequest(); //  stDBReuest ����ֱ��new�� Ҫͨ���˺�����ȡ�� thread safe ;
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