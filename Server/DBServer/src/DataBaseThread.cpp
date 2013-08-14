#include "DataBaseThread.h"
#include "DBRequest.h"
#include "LogManager.h"

CDataBaseThread* CDataBaseThread::SharedDBThread()
{
	static CDataBaseThread g_sDBThread ;
	return &g_sDBThread;
}

bool CDataBaseThread::InitDataBase( const char* pIP,unsigned pPort , const char* pUserName,const char* pPassword, const char* pDBName )
{
	// connect to data base ;
	// init my_sql ;
	m_pMySql = mysql_init(NULL);
	if ( !mysql_real_connect(m_pMySql,pIP,pUserName,pPassword,pDBName,pPort,NULL,0) )
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
	while ( true )
	{
		if ( ProcessRequest() )
			break ;
		if ( !m_bRunning )
		{
			ProcessRequest(); 
			break;
		}
		Sleep(2);
	}

	if ( m_pMySql )
	{
		mysql_close(m_pMySql) ;
	}
}

int CDataBaseThread::EscapeString(char *to, const char *from, unsigned long length )
{
	if ( m_pMySql == NULL )
		return 0 ;
	return mysql_real_escape_string(m_pMySql, to,from,length );
}

bool CDataBaseThread::ProcessRequest()
{
	// process request here ;
	CDBRequestQueue* pRequestQueue = CDBRequestQueue::SharedDBRequestQueue();
	CDBRequestQueue::VEC_DBREQUEST vRequestOut ;
	CDBRequestQueue::VEC_DBRESULT vProcessedResult ;
	pRequestQueue->GetAllRequest(vRequestOut);
	CDBRequestQueue::VEC_DBREQUEST::iterator iter = vRequestOut.begin() ;
	stDBRequest* pRequest = NULL ;
	stDBResult* pResult = NULL ;
	MYSQL_RES *msqlResult = NULL ;
	MYSQL_ROW msqlrow;
	MYSQL_FIELD *msqlfield;
	for ( ; iter != vRequestOut.end(); ++iter )
	{
		pRequest = *iter ;
		pResult = new stDBResult;  // will be deleted after processed in the main thread .
		vProcessedResult.push_back(pResult);
		pResult->nRequestUID = pRequest->nRequestUID ;
		pResult->pUserData = pRequest->pUserData ;
		if ( mysql_real_query(m_pMySql,pRequest->pSqlBuffer,pRequest->nSqlBufferLen) )
		{
			CLogMgr::SharedLogMgr()->ErrorLog("query DB Error Info , Operate UID = %d : %s \n", pRequest->nRequestUID, mysql_error(m_pMySql));
			pResult->nAffectRow = 0 ;
			continue; 
		}
		//int iAffectRow = mysql_affected_rows(m_pMySql) ;
		
		switch ( pRequest->eType )
		{
		case eRequestType_Add:
			//{
			//	pResult->nAffectRow = mysql_affected_rows(m_pMySql);
			//}
			//break; 
		case eRequestType_Delete:
			//{
			//	pResult->nAffectRow = mysql_affected_rows(m_pMySql);
			//}
			//break;
		case eRequestType_Update:
			{
				pResult->nAffectRow = (unsigned int)mysql_affected_rows(m_pMySql);
			}
			break;
		case eRequestType_Select:
			{
				msqlResult = mysql_store_result(m_pMySql);
				pResult->nAffectRow = (unsigned int)mysql_num_rows(msqlResult);
				// process row ;
				int nNumFiled = mysql_num_fields(msqlResult);
				while ( msqlrow = mysql_fetch_row(msqlResult))
				{
					CMysqlRow* rowData = new CMysqlRow ;
					unsigned long* pLengths = mysql_fetch_lengths(msqlResult);
					for ( int i = 0 ; i < nNumFiled ; ++i )
					{
						msqlfield = mysql_fetch_field(msqlResult);
						stMysqlField* pField = new stMysqlField(msqlfield->name,msqlfield->name_length) ;
						pField->nBufferLen = pLengths[i] ;
						bool bValide = true ;
						switch (msqlfield->type)
						{
						case MYSQL_TYPE_TINY: // char
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_TINY", pField->strFieldName.c_str()) ;
							}
							break;
						case MYSQL_TYPE_SHORT: // short 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_SHORT", pField->strFieldName.c_str()) ;
							}
							break;
						case MYSQL_TYPE_LONG: // int
							{
								 CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_LONG", pField->strFieldName.c_str()) ;
							}
							break;
						case MYSQL_TYPE_LONGLONG: // 64 bit int 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_LONGLONG", pField->strFieldName.c_str()) ;
							}
							break;
						case MYSQL_TYPE_FLOAT: // float 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_FLOAT", pField->strFieldName.c_str()) ;
							}
							break; 
						case MYSQL_TYPE_DOUBLE: // double 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_DOUBLE", pField->strFieldName.c_str()) ;
							}
							break;
						case MYSQL_TYPE_BLOB: // binary 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_BLOB", pField->strFieldName.c_str()) ;
							}
							break; 
						case MYSQL_TYPE_VAR_STRING:  // string 
							{
								CLogMgr::SharedLogMgr()->PrintLog("DB request field Type : Type = %s : field Name: %s ","MYSQL_TYPE_VAR_STRING", pField->strFieldName.c_str()) ;
							}
							break;
						default:
							{
								bValide = false ;
								CLogMgr::SharedLogMgr()->ErrorLog("error DB request unsupport field Type : Type = %d : field Name: %s ",msqlfield->type, pField->strFieldName.c_str()) ;
							}
						}

						if ( bValide )
						{
							pField->pBuffer = new char [pField->nBufferLen + 1 ] ;
							memset(pField->pBuffer,0,pField->nBufferLen + 1 );
							memcpy(pField->pBuffer,msqlrow[i],pLengths[i]);
						}
						else
						{
							delete pField ;
							assert(0&&"why support type not !");
							continue; 
						}
						rowData->PushFiled(pField);
					}
					pResult->vResultRows.push_back(rowData);
				}
				mysql_free_result(msqlResult);
			}
			break; 
		default:
			{
				CLogMgr::SharedLogMgr()->ErrorLog("error DB request type, DB request UID = %d , Type = %d",pRequest->nRequestUID,pRequest->eType) ;
				continue; ;
			}
		}
	}
	pRequestQueue->PushReserveRequest(vRequestOut);
	pRequestQueue->PushResult(vProcessedResult);
	return false ;
}
