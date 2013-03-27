#include "TimeUpdater.h"
#include "../Base/InternalMsg.h"
#include "UserMgr.h"
#include "ServerTreator.h"

CTimeUpdater::CTimeUpdater()
{
	m_lastUpdateTime = getMSTime();
	m_lastSaveTime = getMSTime(); 
	m_msUpdateTime = 0;
	m_msSaveTime = 0;
	m_lastDailyTime = 0;
	m_lastWeeklyTime = 0;
}

CTimeUpdater::~CTimeUpdater()
{
}

void CTimeUpdater::__run()
{
	//uint32_t curTime = getMSTime();
	MainLoop();
}

void CTimeUpdater::SetUserUpdateTime(uint32_t tim)
{
	m_msUpdateTime = tim;
}

void CTimeUpdater::SetUserSaveTime(uint32_t tim)
{
	m_msSaveTime = tim*1000;
}

void CTimeUpdater::PushSysMsg(int sub)
{
	printf("CTimeUpdater::PushSysMsg(cmd=%d)\n", sub);
	InternalMsg* pInMsg	= new InternalMsg;
	stServerMsg* pmsg = new stServerMsg(sub);
	pInMsg->pMsg		= (stMsg*)pmsg;
	pInMsg->pMsg->cSysIdentifer = INMSG_SYS;
	pInMsg->nMsgLen		= sizeof(stServerMsg);
	ServerTreator->Push(pInMsg);
}

bool CTimeUpdater::IsWeeklyTime(uint32_t tim)
{
	tm* pTm = gmtime((const time_t*)(&tim));
	if (0 == pTm->tm_wday && 0 == pTm->tm_hour && 0 == pTm->tm_min && 0 == pTm->tm_sec)
	{
		if(tim - m_lastWeeklyTime > 3600)
		{
			m_lastWeeklyTime = tim;
			return true;
		}
	} 
	return false;
}

bool CTimeUpdater::IsDailyTime(uint32_t tim)
{
	tm* pTm = gmtime((const time_t*)(&tim));
	if (0 == pTm->tm_hour && 0 == pTm->tm_min && 0 == pTm->tm_sec)
	{
		if (tim - m_lastDailyTime > 3600)
		{
			m_lastDailyTime = tim;
			return true;
		}
	} 
	return false;
}

bool CTimeUpdater::IsResetBattleTime(uint32_t tim)
{
	tm* pTm = gmtime((const time_t*)(&tim));
	if (19 == pTm->tm_hour && 0 == pTm->tm_min && 0 == pTm->tm_sec)	//GMT 19：00 及北京时间03：00
	{
		if (tim - m_lastResetBattleTime > 3600)
		{
			m_lastResetBattleTime = tim;
			return true;
		}
	} 
	return false;
}

int CTimeUpdater::MainLoop()
{
	//int msCircle = 500;
	while(1)
	{
		uint64_t tim = getMSTime();
		int diff1 = tim - m_lastUpdateTime;
		if (diff1 >= (int)m_msUpdateTime)
		{
			g_UserMgr->Update(tim);//ms
			m_lastUpdateTime = tim;
		}
		int diff2 = tim - m_lastSaveTime;
		if (diff2 >= (int)m_msSaveTime)
		{
			PushSysMsg(SUB_SAVE_DATA);
			m_lastSaveTime = tim;
		}
		if (IsDailyTime(tim/1000))
		{
			PushSysMsg(SUB_TIME1);
		}
		if (IsWeeklyTime(tim/1000))
		{
			PushSysMsg(SUB_TIME2);
		}
		if(IsResetBattleTime(tim/1000))
		{
			PushSysMsg(SUB_RESET_BATTLE_TIME);
		}
		usleep(200);
	}
	return 0;
}

