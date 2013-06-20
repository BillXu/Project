//
//  LogManager.cpp
//  God
//
//  Created by Xu BILL on 12-10-25.
//
//

#include "LogManager.h"
#include <stdarg.h>
#ifdef GOD_CLIENT
#include <string>
#else
#include <string.h>
#endif
CLogMgr* CLogMgr::SharedLogMgr()
{
    static CLogMgr g_sLogMgr ;
    return &g_sLogMgr ;
}

CLogMgr::CLogMgr()
{
    pFile = NULL ;
    bOutPutToFile = false ;
    bEnable = true ;
}

CLogMgr::~CLogMgr()
{
    if ( pFile )
    {
        fclose(pFile) ;
    }
}

void CLogMgr::SetDisablePrint(bool bEnable)
{
    this->bEnable = bEnable ;
}

void CLogMgr::PrintLog(const char *sformate, ...)
{
    va_list va ;
    va_start(va,sformate);
    Print(sformate, va,eLogState_Noraml);
    va_end(va) ;
}

void CLogMgr::ErrorLog(const char *sformate, ...)
{
	va_list va ;
	va_start(va,sformate);
	Print(sformate, va,eLogState_Error);
	va_end(va) ;
}

void CLogMgr::SystemLog(const char* sformate , ...)
{
	va_list va ;
	va_start(va,sformate);
	Print(sformate, va,eLogState_System);
	va_end(va) ;
}

void CLogMgr::SetOutputFile(const char *pFilename)
{
    pFile = fopen(pFilename, "w");
    if ( pFile == NULL )
    {
        bOutPutToFile = false ;
        return ;
    }
    bOutPutToFile = true ;
}

void CLogMgr::Print(const char *sFormate, va_list va , eLogState eSate )
{
    if ( bEnable == false )
        return ;
    
    static char pBuffer[512] = { 0 } ;
    memset(pBuffer,0,sizeof(pBuffer));
    if ( eSate == eLogState_Error )
    {
        sprintf(pBuffer, "Error: [%s , %s] %s \n",__DATE__,__TIME__,sFormate);
    }
    else if ( eSate == eLogState_Noraml )
    {
        sprintf(pBuffer, "Log: [%s, %s] %s \n",__DATE__,__TIME__,sFormate);
    }
    else if ( eSate == eLogState_Warnning )
    {
        sprintf(pBuffer, "Warnning: [%s, %s] %s \n",__DATE__,__TIME__,sFormate);
    }
	else if ( eSate == eLogState_System )
	{
		sprintf(pBuffer, "System: [%s, %s] %s \n",__DATE__,__TIME__,sFormate);
	}
    
    if ( bOutPutToFile && pFile )
    {
        vfprintf(pFile, pBuffer, va) ;
    }
    else
    {
        vprintf(pBuffer, va) ;
    }
}

void CLogMgr::CloseFile()
{
    if ( bOutPutToFile && pFile )
    {
        fclose(pFile) ;
        pFile = NULL ;
        bOutPutToFile = false ;
    }
}
