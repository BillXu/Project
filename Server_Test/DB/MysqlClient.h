#ifndef _MYSQL_CLIENT__H_
#define _MYSQL_CLIENT__H_

#include <pthread.h>
#include <mysql.h>
#include <stdio.h>
#include <vector>
#include "../Share/Common.h"


//#include "Types.h"
//#include "LogManager.h"
#define _MULTI_THREAD_USE_MYSQL_

#define MANUALCOMMIT "SET AUTOCOMMIT = 0"
#define AUTOCOMMIT "SET AUTOCOMMIT = 1"

#define MYSQL_BYTE			0
#define MYSQL_CHAR			1
#define MYSQL_WORD			2
#define MYSQL_DWORD			3
#define MYSQL_QWORD			4
#define MYSQL_STR			5
#define MYSQL_BIN			6
#define MYSQL_DATETIME		7
#define MYSQL_ZIP			8
#define MYSQL_FLOAT			9
#define MYSQL_SHORT			10
#define MYSQL_INT64			11
#define MYSQL_INT			12

#define MAX_MYSQL_CONNECT 		DB_CONN_COUNT//64
#define MAX_CONNECT_URL			128

#ifndef BOOL
	#define BOOL bool
#endif
#ifndef TRUE
	#define TRUE true
#endif
#ifndef FALSE
	#define FALSE false
#endif

typedef struct _MysqlConnParam
{
	char conn[MAX_CONNECT_URL];
	uint32_t hash;
}MysqlConnParam;

typedef struct _mysqlCol
{
	const char * name;		//字段名字
	int mysqlType;			//MYSQL数据类型
	unsigned int dataSize;		//数据大小
	//const char *comment;		//字段描述
}
mysqlCol;

typedef struct mysqlConnectInfo
{
	char url[MAX_CONNECT_URL];
	uint32_t charidmin;
	uint32_t charidmax;
}
mysqlConnectInfo;

typedef uint8_t mysqlDateTime[20];
typedef  uint32_t(* hashCodeFunc)(uint32_t dwCharID);

//mysqlClient handle
#define MYSQLCLIENT_HANDLE_VALID	1	//有效的
#define MYSQLCLIENT_HANDLE_USED		2	//被使用


class CMysqlClientHandler
{
	private:
		uint8_t m_byState;
		int  m_iHashCode;

		char m_pstrHost[MAX_NAME_SIZE];
		char m_pstrUser[MAX_NAME_SIZE];
		char m_pstrPasswd[MAX_PASS_SIZE];
		uint16_t m_wdPort;
		char m_pstrDbName[MAX_NAME_SIZE];

		MYSQL * mysql;
		MYSQL_RES * res;
		uint16_t m_dwGetCount;

		uint32_t m_dwLifeTimeStamp;
		uint32_t m_dwTimeStamp;

		uint8_t * m_pResultData;
		uint32_t m_dwResultSize;
		int	  m_dwEscapedOffset; 
#ifdef _MULTI_THREAD_USE_MYSQL_
		pthread_t m_OwnerThread;	
#endif

		char m_pstrSql[4096];	
		char m_pstrEscapedString[1024];
	public:
		CMysqlClientHandler()
			:m_byState(0),
			mysql(NULL),	
			res(NULL),
			m_dwGetCount(0),
			m_pResultData(NULL),
			m_dwResultSize(0),
			m_dwEscapedOffset(0)
	{
		memset(m_pstrEscapedString, 0 , sizeof(m_pstrEscapedString));
	}
		~CMysqlClientHandler(){};
	public:

		bool Init(const char * connString, int iHashCode, bool bAutoTransactions)
		{
			if(ParseMysqlURLString(connString) == false)
				return false;
			if(InitMysql() == false)
				return false;
			m_byState |= MYSQLCLIENT_HANDLE_VALID;
			m_iHashCode = iHashCode;
			m_dwLifeTimeStamp = time(NULL);
			return !SetTransactions(bAutoTransactions);
		}
		uint32_t GetSerialID()
		{
			return mysql_insert_id(mysql);
		}
		void Free()
		{
			ResetResult();
			Close();
		}

		bool SetUsed()
		{
			if(m_dwGetCount > 1800 ||time(NULL) - m_dwLifeTimeStamp > 1800 || mysql_ping(mysql) != 0)
			{
				Close();
				if(InitMysql() == false)
					return false;
				m_dwLifeTimeStamp = time(NULL);
				m_dwGetCount = 0;
			}

			m_byState |= MYSQLCLIENT_HANDLE_USED;
			m_dwGetCount++;
			m_dwTimeStamp = time(NULL);
#ifdef _MULTI_THREAD_USE_MYSQL_
			m_OwnerThread = pthread_self();	
#endif
			return true;
		}

		void ClearUsed()
		{
			m_byState ^= MYSQLCLIENT_HANDLE_USED;
			m_dwTimeStamp = time(NULL); 
#ifdef _MULTI_THREAD_USE_MYSQL_
			m_OwnerThread = 0;	
#endif
			ResetResult();
		}

		void ResetEscapedString()
		{
			m_dwEscapedOffset = 0;
		}

		uint8_t State()
		{
			return m_byState;
		}

		int HashCode()
		{
			return m_iHashCode;
		}

		uint32_t TimeStamp()
		{
			return m_dwTimeStamp;
		}

		const char * Sql()
		{
			return m_pstrSql;
		}
	public:
		bool Commit()
		{
			return !ExecSql("commit",strlen("commit"));
		}

		bool Rollback()
		{
			return !ExecSql("rollback",strlen("rollback"));
		}

		int SelectDataByCol(const char* tableName, const mysqlCol *column, const char *where, const char * order);

		const uint8_t * GetResultData()
		{
			return m_pResultData;
		}

		uint32_t ResultSize()
		{
			return m_dwResultSize;
		}

		uint32_t InsertDataByCol(const char * tableName, const mysqlCol * column,const uint8_t * data);
		int DeleteData(const char * tableName, const char * where)
		{
			char sql[4096] = "";
			if(tableName == NULL || mysql == NULL)
			{
				//log_error("null pointer error. ---- %s",__FUNCTION__);
				return 0;
			}
			strcat(sql, "DELETE FROM ");
			strcat(sql, tableName);
			if(where)
			{
				strcat(sql, " WHERE ");
				strcat(sql, where);
			}
			ExecSql(sql, strlen(sql));
			unsigned long ret= (unsigned long)mysql_affected_rows(mysql);
			return ret;
		}

		int UpdateDataByCol(const char * tableName, const mysqlCol *column, const uint8_t *data, const char * where);

		const char* EscapeString(const char * data, size_t len = 0)
		{
			int offset = m_dwEscapedOffset;
			if (len == 0)
			{
				m_dwEscapedOffset += (mysql_real_escape_string(mysql, m_pstrEscapedString + offset, data, strlen(data)) + 1);
			}
			else
			{
				m_dwEscapedOffset += (mysql_real_escape_string(mysql, m_pstrEscapedString + offset, data, len) + 1);

			}
			return m_pstrEscapedString + offset;
		}

		int UpdateDataBySql(const char * sql)
		{
			ExecSql(sql, strlen(sql));
			unsigned long ret = (unsigned long)mysql_affected_rows(mysql);
			return ret;

		}
		
		bool UseResult()
		{       
			//得到结果集
			res = mysql_use_result(mysql);
			if(res == NULL)
			{
				//pthread_mutex_unlock(&resultITLock);
				//log_error(mysql_error(mysql));
				return false;
			}
			else
				return true;
		}

		bool SelectDataByColIT(const char * tableName, const mysqlCol * column, const char * where, const char * order)
		{       
			char sql[4096] = "";

			if(tableName == NULL || column == NULL || mysql == NULL)
			{
				//log_error("null pointer error. ---- %s",__FUNCTION__);
				return false;
			}
			if(res != NULL)
			{
				//log_error("resultIT is not null.you must call selectDataByColNext until return FALSE!");
				return false;
			}
			//生成Sql，并执行
			//pthread_mutex_lock(&resultITLock);
			int retsize = FetchSelectSql(tableName, column, where, order, sql);
			if(retsize < 1)
			{               
				//pthread_mutex_unlock(&resultITLock);
				return false;
			}

			//得到结果集
			res = mysql_use_result(mysql);
			if(res == NULL)
			{
				//pthread_mutex_unlock(&resultITLock);
				//log_error(mysql_error(mysql));
				return false;
			}
			else
				return true;
		}

		bool SelectDataByColNext(const mysqlCol * column, uint8_t * data)
		{
			//填充返回值
			if(column == NULL || data == NULL)
			{
				//log_error("null pointer error. ---- %s",__FUNCTION__);
				ResetResult();	
				return false;
			}
			if(res == NULL)
			{
				//log_error("You must successfully call selectUnionDataByColIT or selectDataByColIT before calling selectDataByColNext.");
				return false;
			}
			MYSQL_ROW row = mysql_fetch_row(res);
			if (row != NULL)
			{
				unsigned long * lengths = mysql_fetch_lengths(res);
				if(!FullSelectDataByRow(row, lengths, column, data))
				{
					ResetResult();	
					return false;
				}
				return true;
			}
			else
			{
				ResetResult();	
				return false;
			}
		}

		void ResetResult()
		{
			if(m_pResultData)
			{
				delete[] m_pResultData;
				m_pResultData = NULL;
			}
			m_dwResultSize = 0;
			if(res != NULL)
			{
				mysql_free_result(res);
				res = NULL;
			}
		}
		int ExecSql(const char * sql, unsigned long sqllen)
		{
			if(sql == NULL || sqllen == 0 || mysql == NULL)
			{
				//log_error("invalid mysql handle or sql statement.");
				return -1;
			}

            //struct timeval tvStart, tvStop; 
            //gettimeofday(&tvStart,NULL);
			//strncpy(m_pstrSql, sql, sizeof(m_pstrSql));
			int ret = mysql_real_query(mysql, sql, sqllen);
			/*if(ret)
			{
				log_error(mysql_error(mysql));
				log_error(sql);
			}       
            else 
            {
                gettimeofday(&tvStop,NULL);
                if(tvStop.tv_sec - tvStart.tv_sec >= 1)
                    log_info("%s execsql use (%lu/%lu) sql=%.*s .%s"
                            , Red
                            , tvStop.tv_sec - tvStart.tv_sec
                            , (tvStop.tv_usec -tvStart.tv_usec)/1000
                            , sizeof(m_pstrSql)
                            , m_pstrSql
                            , None);
            }*/
			return ret;
		}                     
		unsigned int GetLastError()
		{
			return mysql_errno(this->mysql);
		}
	private:
		bool ParseMysqlURLString(const char *connString);
		int SetTransactions(bool supportTransactions)
		{       
			if(supportTransactions)
				return ExecSql(MANUALCOMMIT, strlen(MANUALCOMMIT));
			else    
				return ExecSql(AUTOCOMMIT, strlen(AUTOCOMMIT));
		} 

		bool InitMysql()
		{
			mysql=mysql_init(mysql);
			if(mysql==NULL)
			{                       
				//log_error("Initiate mysql error...");
				return false;           
			}                       

			if(mysql_real_connect(mysql, m_pstrHost, m_pstrUser, m_pstrPasswd, m_pstrDbName, m_wdPort, NULL, CLIENT_COMPRESS|CLIENT_INTERACTIVE) == NULL)
			{                               
				//log_error("connect mysql failed...");
				//log_error("reason: %s",mysql_error(mysql));
				mysql_close(mysql); 
				mysql=NULL;
				return false;   
			}                       
			//log_info("connect mysql successful...");
			return true;                    
		}

		void Close()
		{
			if(mysql!=NULL)
			{
				mysql_close(mysql);
				mysql = NULL;
			}
		}

		int FetchSelectSql(const char * tableName, const mysqlCol * column, const char * where,const char * order ,char * sql);	
		bool FullSelectDataByRow(MYSQL_ROW row, unsigned long * lengths, const mysqlCol * temp, uint8_t * tempData);
};

#define MAX_CONNECT_HANDLE      MAX_MYSQL_CONNECT*16
class CMysqlPool
{
	private:
		std::vector<CMysqlClientHandler *> m_HandlerPool;
		mysqlConnectInfo m_tConnectInfo[MAX_MYSQL_CONNECT];
		bool m_bAutoTransactions;
		hashCodeFunc m_HashFunc;
#ifdef _MULTI_THREAD_USE_MYSQL_
		pthread_mutex_t m_tPoolLock;
	public:
		bool ThreadInit()
		{
			return !mysql_thread_init();
		}       

		void ThreadEnd()
		{
			mysql_thread_end();
		}
#endif
	public:
		CMysqlPool();
		~CMysqlPool();
	private:
		static uint32_t DefaultHashCode(uint32_t dwCharID)
		{ 
			//UNUSED_ARG(dwCharID);                              
			return 0;                       
		}                
	public:
		static CMysqlPool* instance()
		{
			static CMysqlPool* _instance = NULL;
			if(NULL == _instance)
			{
				_instance = new CMysqlPool();
			}
			return _instance;
			//return *ACE_Singleton<CMysqlPool, ACE_Thread_Mutex>::instance();
		}
	public:
		bool SetConnectString(uint16_t wdHashCode, const char * pstrConnectString)
		{
			if(wdHashCode < MAX_MYSQL_CONNECT)
			{
				strncpy(m_tConnectInfo[wdHashCode].url, pstrConnectString, MAX_CONNECT_URL);
				return true;
			}
			return false;
		}	

		CMysqlClientHandler * GetHandlerByHashcode(uint16_t wdHashCode);
		void PutHandler(CMysqlClientHandler * pHandler)
		{
#ifdef _MULTI_THREAD_USE_MYSQL_
			pthread_mutex_lock(&m_tPoolLock);
#endif
			if(pHandler)
			{

				pHandler->ClearUsed();
				pHandler->ResetEscapedString();
			}
#ifdef _MULTI_THREAD_USE_MYSQL_ 
			pthread_mutex_unlock(&m_tPoolLock);
#endif
		}

		CMysqlClientHandler * GetNextHandler(CMysqlClientHandler * pHandler)
		{
			int hashcode=0;
			if(pHandler != NULL)
			{
				hashcode = pHandler->HashCode() + 1;//下一个连接
				PutHandler(pHandler); //收会上一个
			}
			return GetHandlerByHashcode(hashcode);
		}

		bool InitByHashCodeFunc(bool supportTransactions,  hashCodeFunc hashfunc);
	public:
		CMysqlClientHandler * GetHandlerByCharID(uint32_t dwCharID)
		{

			uint16_t hashcode = m_HashFunc(dwCharID);
			return GetHandlerByHashcode(hashcode);
		}		
};


#define MysqlPool CMysqlPool::instance()
#endif


