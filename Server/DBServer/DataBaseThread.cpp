#include "DataBaseThread.h"
bool CDataBaseThread::InitDataBase( const char* pIP,unsigned pPort , const char* pUserName,const char* pPassword, const char* pDBName )
{
	// connect to data base ;
	// init my_sql ;
	m_pMySql = mysql_init(NULL);
	if ( !mysql_real_connect(NULL,pIP,pUserName,pPassword,pDBName,pPort,NULL,0) )
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\\n",  mysql_error(m_pMySql));
		m_bRunning = false ;
		mysql_close(m_pMySql) ;
		m_pMySql = NULL ;
		return false ;
	}
	m_bRunning = true ;
	return true ;
}

void CDataBaseThread::StopWork()
{
	m_bRunning = false ;
}

void CDataBaseThread::__run()
{
	while ( m_bRunning )
	{
		if ( ProcessRequest() )
			break ;
	}

	if ( m_pMySql )
	{
		mysql_close(m_pMySql) ;
	}
}

bool CDataBaseThread::ProcessRequest()
{
	// process request here ;
	return false ;
}
