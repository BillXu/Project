#include <errno.h>
#ifndef __USE_UNIX98
#define __USE_UNIX98
#endif
//#include <zlib.h>

#include "MysqlClient.h"
//#include "LogManager.h"

char dbname[MAX_NAME_SIZE] = "Ares";
char username[MAX_NAME_SIZE] = "Ares";
char password[MAX_PASS_SIZE] = "Ares";
//mysqlConnectInfo mysqlConnectInfos[MAX_MYSQL_CONNECT] = {{"mysql://Ares:Ares@192.168.10.2:3306/Titan",0,0}};
uint16_t mysqlConnectCount = 0;

//mysql连接池
#define MAX_CONNECT_HANDLE	MAX_MYSQL_CONNECT*16

#define log do_log


time_t StrtoDataTime(const char* pStrTime)
{
	struct tm tm;
	strptime(pStrTime, "%Y-%m-%d %H:%M:%S", &tm);

	return mktime(&tm);
}

char* DateTimetoStr(time_t time)
{
	static char szTime[20];
	struct tm *tmp;

	tmp = localtime(&time);
	if (NULL == tmp)
		return NULL;

	if (strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", tmp) == 0)
		return NULL;

	return szTime;
}

bool CMysqlClientHandler::ParseMysqlURLString(const char *connString)
{
	char host[MAX_NAME_SIZE] = "";
	char user[MAX_NAME_SIZE] = "";
	char passwd[MAX_PASS_SIZE] ="";
	char port[16] = "";
	char dbName[MAX_NAME_SIZE] = "";
	unsigned int i, j, k;
	if (strncmp(connString, "mysql://", strlen("mysql://"))) 
	{
		return false;
	}
	i = 0; j = 0; k = 0;
	for(i = strlen("mysql://"); i < strlen(connString) + 1; i++) 
	{
		switch(j) 
		{
			case 0:
				if (connString[i] == ':') 
				{
					user[k] = '\0';
					j++;
					k = 0;
				}
				else 
				{
					user[k++] = connString[i];
				}
				break;
			case 1:
				if (connString[i] == '@') 
				{
					passwd[k] = '\0';
					j++;
					k = 0;
				}
				else 
				{
					passwd[k++] = connString[i];
				}
				break;
			case 2:
				if (connString[i] == ':') 
				{
					host[k] = '\0';
					j++;
					k = 0;
				}
				else 
				{
					host[k++] = connString[i];
				}
				break;
			case 3:
				if (connString[i] == '/') 
				{
					port[k] = '\0';
					j++;
					k = 0;
				}
				else 
				{
					port[k++] = connString[i];
				}
				break;
			case 4:
				if (connString[i] == '\0') 
				{
					dbName[k] = '\0';
					j++;
					k = 0;
				}
				else 
				{
					dbName[k++] = connString[i];
				}
				break;
			default:
				break;
		}
	}

	if (j != 5) 
	{
		return FALSE;
	}

	strncpy(m_pstrHost, host, MAX_NAME_SIZE);
	strncpy(m_pstrUser, user, MAX_NAME_SIZE);
	strncpy(m_pstrPasswd, passwd, MAX_PASS_SIZE);
	m_wdPort = atoi(port);
	strncpy(m_pstrDbName, dbName, MAX_NAME_SIZE);

	return TRUE;

}

int CMysqlClientHandler::FetchSelectSql(const char * tableName, const mysqlCol * column, const char *where,const char * order ,char * sql)
{
	unsigned int retsize=0;//查询结果大小
	const mysqlCol * temp;
	int len, sqllen = 0;;
	//生成SQL,计算返回结果大小
	bool first= true;
	strcat(sql,"SELECT "); sqllen += 7/*strlen("SELECT ")*/;
	temp = column;
	while(temp->name)
	{
		if(temp->dataSize == 0)
		{
			//log_error("invalid column data size.");
			return -1;//retval;//字段大小为零，错误
		}
		retsize += temp->dataSize;//计算返回值的大小
		len = strlen(temp->name);
		if(len > 0)//有效column
		{
			if(first) 
			{
				first = false;
			}
			else 
			{
				strcat(sql,", "); 
				sqllen += 2/*strlen(", ")*/;
			}
			strcat(sql, "`");
			strcat(sql,temp->name); 
			strcat(sql, "`");
			sqllen += (len+2);
		}
		temp++;
	}
	len = strlen(tableName);
	if(len > 0)
	{
		strcat(sql," FROM "); 
		sqllen += 6/*strlen(" FROM ")*/;
		strcat(sql,tableName); 
		sqllen += len;
	}
	if(where != NULL)
	{
		strcat(sql," WHERE "); 
		sqllen += 7/*strlen(" WHERE ")*/;
		strcat(sql,where); 
		sqllen += strlen(where);
	}
	if(order != NULL)
	{
		strcat(sql," ORDER BY ");
		sqllen += 10/*strlen(" ORDER BY ")*/;
		strcat(sql,order); 
		sqllen += strlen(order);
	}

	if(ExecSql(sql, sqllen) != 0)
		return -1;//retval;
	else
		return retsize;
}

//用结果row填充Select的数据
bool CMysqlClientHandler::FullSelectDataByRow(MYSQL_ROW row, unsigned long * lengths, const mysqlCol * temp,uint8_t * tempData)
{
	int offset = 0;//column数据偏移量
	int i = 0;
	while(temp->name)
	{
		if(strlen(temp->name) != 0)
		{
			switch(temp->mysqlType)
			{
				case MYSQL_CHAR:
					if (row[i])
						*(char *)(tempData+offset)=strtoul(row[i],(char **)NULL,10);
					else
						*(char *)(tempData+offset)=0;
				case MYSQL_BYTE:
					if (row[i])
						*(uint8_t *)(tempData+offset)=strtoul(row[i],(char **)NULL,10);
					else
						*(uint8_t *)(tempData+offset)=0;
					break;
				case MYSQL_WORD:
					if (row[i])
						*(uint16_t *)(tempData+offset)=strtoul(row[i],(char **)NULL,10);
					else
						*(uint16_t *)(tempData+offset)=0;
					break;
				case MYSQL_DWORD:
					if (row[i])
						*(uint32_t *)(tempData+offset)=strtoul(row[i],(char **)NULL,10);
					else
						*(uint32_t *)(tempData+offset)=0L;
					break;
				case MYSQL_QWORD:
					if (row[i])
						*(uint64_t *)(tempData+offset)=strtoull(row[i],(char **)NULL,10);
					else
						*(uint64_t *)(tempData+offset)=0LL;
					break;
				case MYSQL_INT64:
					if (row[i])
						*(int64_t *)(tempData+offset)=strtoll(row[i],(char **)NULL,10);
					else
						*(int64_t *)(tempData+offset)=0LL;
					break;
				case MYSQL_INT:
					if (row[i])
						*(int *)(tempData+offset)=strtol(row[i],(char **)NULL,10);
					else
						*(int *)(tempData+offset)=0LL;
					break;
				case MYSQL_FLOAT:
					if (row[i])
						*(float *)(tempData+offset)=atof(row[i]);
					else
						*(float *)(tempData+offset)=0.0;
					break;
				case MYSQL_SHORT:
					if (row[i])
						*(short *)(tempData+offset)=strtol(row[i],(char **)NULL,10);
					else
						*(short *)(tempData+offset)=0;
					break;
				
				case MYSQL_STR:
				case MYSQL_BIN:
					bzero(tempData+offset,temp->dataSize);
					if (row[i])
					{
						bcopy(row[i],tempData+offset,temp->dataSize>lengths[i]?lengths[i]:temp->dataSize);
					}
					break;
				case MYSQL_DATETIME:
					if (row[i])
						*(uint32_t *)(tempData+offset)= StrtoDataTime(row[i]);
					else
						*(uint32_t *)(tempData+offset)=0L;
					break;

					break;
				/*case MYSQL_ZIP:
					{
						bzero(tempData+offset,temp->dataSize);
						if (row[i]) 
						{
							int retcode;
							uLong destLen = temp->dataSize;
							retcode = uncompress(tempData+offset, &destLen, (Bytef *)row[i], lengths[i])
								;
							switch(retcode) 
							{
								case Z_OK:
									break;
								case Z_MEM_ERROR:
									bzero(tempData+offset,temp->dataSize);
									//log_error( "(%s, %d)Z_MEM_ERROR, zero value instead.", __FUNCTION__, __LINE__);
									break;
								case Z_BUF_ERROR:
									bzero(tempData+offset,temp->dataSize);
									//log_error( "(%s, %d)Z_BUF_ERROR, zero value instead.", __FUNCTION__, __LINE__);
									break;
								case Z_DATA_ERROR:
									bzero(tempData+offset,temp->dataSize);
									//log_error( "(%s, %d)Z_DATA_ERROR, zero value instead.", __FUNCTION__, __LINE__);
									break;
							}
						}
					}
					break;*/
				default:
					//log_error("invalid titan mysql type(%d). ->%s",temp->mysqlType,temp->name);
					return FALSE;
			}
			i++;
		}
		offset+=temp->dataSize;
		temp++;
	}
	return TRUE;
}

int CMysqlClientHandler::SelectDataByCol(const char* tableName, const mysqlCol *column, const char *where, const char * order) 
{
	char sql[4096] = "";
	int retval = -1;//返回值
	unsigned int retsize = 0;//查询结果大小

	if(tableName == NULL || column == NULL || mysql == NULL)
	{
		//log_error("null pointer error. ---- %s",__FUNCTION__);
		return retval;
	}       
	//生成Sql，并执行
	retsize = FetchSelectSql(tableName, column, where, order, sql);
	if(retsize < 1) 
		return retval;

	//得到结果集
	unsigned int retCount = 0;//查询结果个数
	//MYSQL_RES *result=NULL;
	res = mysql_store_result(mysql);
	if(res == NULL)
	{
		//log_error(mysql_error(mysql));
		return retval;
	}

	retCount = mysql_num_rows(res);
	if(retCount == 0)
	{
		mysql_free_result(res);
		return retCount;
	}

	if(m_pResultData != NULL)
	{
		delete[] m_pResultData;
		m_pResultData = NULL;
		m_dwResultSize = 0;
	}

	m_pResultData = new uint8_t[retCount * retsize / sizeof(uint8_t)];
	m_dwResultSize = retCount * retsize;

	if(m_pResultData == NULL)
	{
		//log_error("malloc mem error .");
		mysql_free_result(res);
		return retval;
	}

	bzero(m_pResultData, m_dwResultSize);

	//填充返回值
	MYSQL_ROW row;
	uint8_t * tempData = m_pResultData;
	while ((row = mysql_fetch_row(res)))
	{
		unsigned long * lengths= mysql_fetch_lengths(res);
		if(!FullSelectDataByRow(row, lengths, column, tempData))
		{
			delete[] m_pResultData;
			m_dwResultSize = 0;
			mysql_free_result(res);
			return retval;
		}
		tempData += retsize;
	}

	mysql_free_result(res);
	return retCount;
} 

uint32_t CMysqlClientHandler::InsertDataByCol(const char * tableName, const mysqlCol * column,const uint8_t * data)
{
	char sql[1024*1024] = "";
	const mysqlCol * temp;
	int len, sqllen = 0;
	if(tableName == NULL || data == NULL || column == NULL || mysql == NULL)
	{
		//log_error("null pointer error. ---- %s",__FUNCTION__);
		return false;
	}

	//生成SQL
	strcat(sql,"INSERT INTO "); 
	sqllen += 12/*strlen("INSERT INTO ")*/;
	strcat(sql,tableName); 
	sqllen += strlen(tableName);
	strcat(sql," ( "); 
	sqllen += 3/*strlen(" ( ")*/;
	temp = column;
	bool first = true;
	while(temp->name)
	{
		len = strlen(temp->name);
		if(len > 0)
		{
			if(first) 
			{
				first = false;
			}
			else 
			{
				strcat(sql,", "); 
				sqllen += 2/*strlen(", ")*/;
			}
			strcat(sql,temp->name); 
			sqllen += len;
		}
		if(temp->dataSize == 0)
		{
			//log_error("invalid column data size.");
			return false;
		}
		temp++;
	}
	strcat(sql,") VALUES( "); 
	sqllen += 10/*strlen(") VALUES( ")*/;

	first = true;
	temp = column;
	int offset = 0;
	while(temp->name)
	{
		if(strlen(temp->name)!=0)
		{
			if(first) 
			{
				first = false;
			}
			else 
			{
				strcat(sql,", "); 
				sqllen += 2/*strlen(", ")*/;
			}
			//values内容
			switch(temp->mysqlType)
			{

				case MYSQL_CHAR:
					{
						sqllen += sprintf(&sql[sqllen], "%d", *(char *)(data+offset));
					}
					break;
				case MYSQL_BYTE:
					{
						sqllen += sprintf(&sql[sqllen], "%u", *(uint8_t *)(data+offset));
					}
					break;
				case MYSQL_WORD:
					{
						sqllen += sprintf(&sql[sqllen], "%u", *(uint16_t *)(data+offset));
					}
					break;
				case MYSQL_DWORD:
					{
						sqllen += sprintf(&sql[sqllen], "%u", *(uint32_t *)(data+offset));
					}
					break;
				case MYSQL_QWORD:
					{
						sqllen += sprintf(&sql[sqllen], "%llu", *(uint64_t *)(data+offset));
					}
					break;
				case MYSQL_FLOAT:
					{
						sqllen += sprintf(&sql[sqllen], "%f", *(float *)(data+offset));
					}
					break;
				case MYSQL_SHORT:
					{
						sqllen += sprintf(&sql[sqllen], "%d", *(short *)(data+offset));
					}
					break;
				case MYSQL_STR:
					{
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
						len=strlen((char *)(data+offset));
						if((uint32_t)len > temp->dataSize)
							len = temp->dataSize;
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen],(char *)(data+offset),len);
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
					}
					break;
				case MYSQL_DATETIME:
					{
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
						char* pszTime = DateTimetoStr(*(uint32_t *)(data+offset));
						len=strlen(pszTime);
						/*if((uint32_t)len > temp->dataSize)
							len = temp->dataSize;
						*/
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen], pszTime, len);
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;

					}
					break;
				case MYSQL_BIN:
					{
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen],(char *)(data+offset),temp->dataSize);
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;	
					}
					break;
				/*case MYSQL_ZIP:
					{
						//计算压缩目的缓冲长度,must be at least 0.1% larger than sourceLen plus 12 bytes
						uLong destLen = temp->dataSize * 120 / 100 + 12;
						Bytef * destBuffer = new Bytef[destLen];
						strcat(sql,"\'"); 
						sqllen++;
						if (destBuffer) 
						{
							int retcode = compress(destBuffer, &destLen, (Bytef *)(data+offset),temp->dataSize);
							switch(retcode) 
							{
								case Z_OK:
									sqllen += mysql_real_escape_string(mysql,&sql[sqllen],(char *)destBuffer,destLen);
									break;
								case Z_MEM_ERROR:
									//log_fatal( "(%s, %d)Z_MEM_ERROR, NULL value instead.", __FUNCTION__, __LINE__);
									break;
								case Z_BUF_ERROR:
									//log_fatal( "(%s, %d)Z_BUF_ERROR, NULL value instead.", __FUNCTION__, __LINE__);
									break;
							}
							delete[] destBuffer;
						}
						else 
						{
							//log_fatal( "(%s, %d)Not enough memory, NULL value instead.", __FUNCTION__, __LINE__);
						}
						strcat(sql,"\'"); 
						sqllen++;
					}
					break;*/
				default:
					//log_error("invalid titan mysql type.");
					return false;
			}
		}
		offset += temp->dataSize;
		temp++;
	}
	strcat(sql,")"); 
	sqllen++;

	//执行SQL ,原子操作
	uint32_t ret = 0;
	//pthread_mutex_lock(&insertLock);
	if(!ExecSql(sql, sqllen/*strlen(sql)*/))
	{
		ret = mysql_insert_id(mysql);
		///! if not generate auto_id,maybe error [chib]
		/*
		if(ret == 0) 
			ret = 1;
			*/
	}
	//pthread_mutex_unlock(&insertLock);
	return ret;
}

int CMysqlClientHandler::UpdateDataByCol(const char * tableName, const mysqlCol *column, const uint8_t *data, const char * where)
{
	char sql[1024*1024] = "";
	int len, sqllen = 0;
	const mysqlCol * temp;
	if(tableName == NULL || column == NULL || data == NULL || mysql == NULL)
	{
		//log_error("null pointer error. ---- %s",__FUNCTION__);
		return 0;
	}

	//生成SQL
	strcat(sql,"UPDATE "); 
	sqllen += 7/*strlen("UPDATE ")*/;
	strcat(sql, tableName); 
	sqllen += strlen(tableName);
	strcat(sql, " SET "); 
	sqllen += 5/*strlen(" SET ")*/;
	temp = column;
	bool first = true;
	int offset = 0;
	while(temp->name)
	{
		if(temp->dataSize == 0)
		{
			//log_error("invalid column data size.");
			return 0;
		}
		len = strlen(temp->name);
		if(len > 0)
		{
			if(first) 
			{
				first = false;
			}
			else 
			{
				strcat(sql,", "); 
				sqllen += 2/*strlen(", ")*/;
			}
			strcat(sql, temp->name); 
			sqllen += len;
			strcat(sql, "="); 
			sqllen++/*sqllen += strlen("=")*/;
			//set内容
			switch(temp->mysqlType)
			{
				case MYSQL_CHAR:
					{
						sqllen += sprintf(&sql[sqllen],"%d",*(char *)(data+offset));
					}
					break;
				case MYSQL_BYTE:
					{
						sqllen += sprintf(&sql[sqllen],"%u",*(uint8_t *)(data+offset));
					}
					break;
				case MYSQL_WORD:
					{
						sqllen += sprintf(&sql[sqllen],"%u",*(uint16_t *)(data+offset));
					}
					break;
				case MYSQL_DWORD:
					{
						sqllen += sprintf(&sql[sqllen],"%u",*(uint32_t *)(data+offset));
					}
					break;
				case MYSQL_QWORD:
					{
						sqllen += sprintf(&sql[sqllen],"%llu",*(uint64_t *)(data+offset));
					}
					break;
				case MYSQL_FLOAT:
					{
						sqllen += sprintf(&sql[sqllen], "%f", *(float *)(data+offset));
					}
					break;
				case MYSQL_SHORT:
					{
						sqllen += sprintf(&sql[sqllen],"%d",*(short *)(data+offset));
					}
					break;
				case MYSQL_STR:
					{
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
						len=strlen((char *)(data+offset));
						if((uint32_t)len > temp->dataSize) 
							len = temp->dataSize;
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen],(char *)(data+offset),len);
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
					}
					break;
				case MYSQL_DATETIME:
					{
						strcat(sql,"\'");
						sqllen++/*sqllen += strlen("\'")*/;
						char* pszTime = DateTimetoStr(*(uint32_t *)(data+offset));
						len=strlen(pszTime);
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen], pszTime, len);
						strcat(sql,"\'");
						sqllen++/*sqllen += strlen("\'")*/;

					}
					break;

				case MYSQL_BIN:
					{
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
						sqllen += mysql_real_escape_string(mysql, &sql[sqllen],(char *)(data+offset),temp->dataSize);
						strcat(sql,"\'"); 
						sqllen++/*sqllen += strlen("\'")*/;
					}
					break;
				/*case MYSQL_ZIP:
					{
						//计算压缩目的缓冲长度,must be at least 0.1% larger than sourceLen plus 12 bytes
						uLong destLen = temp->dataSize * 120 / 100 + 12;
						Bytef * destBuffer = new Bytef[destLen];
						strcat(sql,"\'"); 
						sqllen++;
						if (destBuffer) 
						{
							int retcode = compress(destBuffer, &destLen, (Bytef *)(data+offset),temp->dataSize);
							switch(retcode) 
							{
								case Z_OK:
									sqllen += mysql_real_escape_string(mysql,&sql[sqllen],(char *)destBuffer,destLen);
									break;
								case Z_MEM_ERROR:
									//log_fatal( "(%s, %d)Z_MEM_ERROR, NULL value instead.", __FUNCTION__, __LINE__);
									break;
								case Z_BUF_ERROR:
									//log_fatal( "(%s, %d)Z_BUF_ERROR, NULL value instead.", __FUNCTION__, __LINE__);
									break;
							}
							delete[] destBuffer;
						}
						else 
						{
							//log_fatal( "(%s, %d)Not enough memory, NULL value instead.", __FUNCTION__, __LINE__);
						}
						strcat(sql,"\'"); 
						sqllen++;
					}
					break;*/
				default:
					//log_error("invalid ares mysql type.");
					return FALSE;
			}
		}
		offset += temp->dataSize;
		temp++;
	}
	if(where != NULL)
	{
		strcat(sql," WHERE "); 
		sqllen += 7/*strlen(" WHERE ")*/;
		strcat(sql,where); 
		strcat(sql, ";");
		sqllen += strlen(where);
		sqllen += 1;
	}
	else 
	{
		strcat(sql, ";");
		sqllen += 1;
	}

	//pthread_mutex_lock(&affectedLock);
	ExecSql(sql, sqllen/*strlen(sql)*/);
	unsigned long ret = (unsigned long)mysql_affected_rows(mysql);
	//pthread_mutex_unlock(&affectedLock);
	return ret;
} 

CMysqlClientHandler * CMysqlPool::GetHandlerByHashcode(uint16_t wdHashCode)
{
	unsigned int i;
	int timeoutCount = 0;
    //time_t timeStart = time(NULL);
	while(true)
	{
		int invalidHandleID = -1;
#ifdef _MULTI_THREAD_USE_MYSQL_
		pthread_mutex_lock(&m_tPoolLock);
#endif
		for(i = 0; i < m_HandlerPool.size(); i++)
		{
			if(m_HandlerPool[i]->State() & MYSQLCLIENT_HANDLE_VALID)
			{
				if(m_HandlerPool[i]->HashCode() == wdHashCode)
				{
					//句柄未被使用
					if ((m_HandlerPool[i]->State() & MYSQLCLIENT_HANDLE_USED) ==0)
					{
						if(m_HandlerPool[i]->SetUsed())
                        {
#ifdef _MULTI_THREAD_USE_MYSQL_
                            pthread_mutex_unlock(&m_tPoolLock);
#endif
                            //if(timeoutCount) 
                                //log_special("timeout handle total %d",timeoutCount);
                            m_HandlerPool[i]->ResetEscapedString();
                            
                            /*time_t timeStop = time(NULL);
                            if(timeStop - timeStart  > 0)
                            {
                                log_info("%s mysql get handler use(%d) second.%s"
                                        , Red
                                        , timeStop - timeStart
                                        , None);
                            }*/
                            return m_HandlerPool[i];
                        }
						else
							m_HandlerPool[i]->Free();
					}
					//句柄超时10秒,回收此句柄
					else if(time(NULL) - m_HandlerPool[i]->TimeStamp() > 10)
					{
						timeoutCount++;
						//log_error("The handle(%d) timeout %lds", i, time(NULL) - m_HandlerPool[i]->TimeStamp());
						//log_error("The handle sql is : %s", m_HandlerPool[i]->Sql());
					}
				}
			}
			else if(invalidHandleID == -1)
			{
				invalidHandleID = i;
				break;
			}
		}
		if(invalidHandleID != -1)//有空闲句柄,生成一个新句柄
		{
			if(strnlen(m_tConnectInfo[wdHashCode].url, MAX_CONNECT_URL) == 0)//没有连接URL直接返回NULL
            {
#ifdef _MULTI_THREAD_USE_MYSQL_ 
                pthread_mutex_unlock(&m_tPoolLock);
#endif
                //log_error("%s mysql get handler failed.%s", Red, None);
                return NULL;
            }
			if(m_HandlerPool[invalidHandleID]->Init(m_tConnectInfo[wdHashCode].url, wdHashCode, m_bAutoTransactions))
			{
				if(m_HandlerPool[invalidHandleID]->SetUsed())
                {
#ifdef _MULTI_THREAD_USE_MYSQL_ 
                    pthread_mutex_unlock(&m_tPoolLock);
#endif
                    //if(timeoutCount) 
                       // log_special("timeout handle total %d",timeoutCount);
                    m_HandlerPool[invalidHandleID]->ResetEscapedString();

                    /*time_t timeStop = time(NULL);
                    if(timeStop - timeStart  > 0)
                    {
                        log_info("%s mysql create handler use(%d) second.%s"
                                , Red
                                , timeStop - timeStart
                                , None);
                    }*/
                    return m_HandlerPool[invalidHandleID];
                }
				else
					m_HandlerPool[invalidHandleID]->Free();
			}
		}
#ifdef _MULTI_THREAD_USE_MYSQL_ 
		pthread_mutex_unlock(&m_tPoolLock);
#endif
		usleep(10000);//没有得到句柄,休眠0.01秒
	}
}

bool CMysqlPool::InitByHashCodeFunc(bool supportTransactions,  hashCodeFunc hashfunc) 
{
	m_bAutoTransactions = supportTransactions;
	//log_info("Version of the mysql libs is %s", mysql_get_client_info());

#ifdef _MULTI_THREAD_USE_MYSQL_ 
	/*if(!mysql_thread_safe())
	{
		log_error("The mysql libs is not thread safe ... %d", mysql_thread_safe());
	}*/
	pthread_mutex_init(&m_tPoolLock, NULL);
#endif
	//初始化hashcode函数
	if(hashfunc) 
		m_HashFunc = hashfunc;

	//初始化第一个句柄
	int count=0;
	int i;
	for(i = 0; i < MAX_MYSQL_CONNECT; i++)
	{
		if(strnlen(m_tConnectInfo[i].url, MAX_CONNECT_URL) > 0)
		{
			if(!m_HandlerPool[count]->Init(m_tConnectInfo[i].url, i, m_bAutoTransactions))
			{
				int j;
				for(j = count-1; j >= 0; j--)
					m_HandlerPool[j]->Free();
				return false;
			}
			count++;
		}
	}
	return true;
} 

CMysqlPool::CMysqlPool()
{
	m_HashFunc = CMysqlPool::DefaultHashCode;
	for(unsigned int i = 0; i < MAX_CONNECT_HANDLE; i++)
	{
		CMysqlClientHandler * pHandler = new CMysqlClientHandler();
		m_HandlerPool.push_back(pHandler);
	}
	bzero(m_tConnectInfo, sizeof(mysqlConnectInfo) * MAX_MYSQL_CONNECT);
}

CMysqlPool::~CMysqlPool()
{
	unsigned int i;
	for(i = 0; i < m_HandlerPool.size(); i++)
	{
		if(m_HandlerPool[i]->State() & MYSQLCLIENT_HANDLE_USED)//MYSQLCLIENT_HANDLE_VALID)
			m_HandlerPool[i]->Free();
	}
#ifdef _MULTI_THREAD_USE_MYSQL_
	pthread_mutex_destroy(&m_tPoolLock);
#endif
	return;
} 

