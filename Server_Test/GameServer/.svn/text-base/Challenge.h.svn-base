
#ifndef _CHALLENGE__H_
#define _CHALLENGE__H_

#include "../Base/Define.h"
#include "../Base/InternalMsg.h"
#include "../Share/mutex.h"

class CUser;
class CChallenge
{
public:
	CChallenge(unsigned int myRoleId, unsigned int roleId);
	CChallenge(stChallengeInfo* pInfo);
	~CChallenge();
	
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline bool IsDelete()
	{
		return (changeFlag == 2);
	}
	inline void SetChangeFlag()
	{
		changeFlag = 1;
	}
	inline void SetDelFlag()
	{
		changeFlag = 2;
	}
	inline void SetAddFlag()
	{
		changeFlag = 3;
	}
	inline void ResetFlag()
	{
		changeFlag = 0;
	}
public:
	t_ChallengeInfo m_info;
protected:
	unsigned char changeFlag;	//3Ôö ; 2 É¾ ;1 ¸Ä
};

class CChallengeMgr
{
public:
	bool CanChallenge(unsigned int roleId);
	void AddHistory(unsigned int myRoleId, unsigned int roleId, unsigned char type);
	int Add(unsigned int myRoleId, unsigned int roleId, unsigned char type);
	int Add(CChallenge* rel);
	int Del(CChallenge* rel);
	CChallenge* GetChallenge(unsigned int roleId);

	short CopyChallengeInfo(unsigned int roleId, char* buf, short bufLen);
public:
	void SetUser(CUser* pUser)
	{
		assert(pUser);
		m_pUser = pUser;
	}
	CChallengeMgr();
	~CChallengeMgr();
private:
	inline void Lock()
	{
		m_mutex.Acquire();
	}
	inline void UnLock()
	{
		m_mutex.Release();
	}
private:
	Mutex m_mutex;
	CUser* m_pUser;
public:
	typedef std::map<uint32_t, CChallenge*> MAP_CHALLENGE;
	MAP_CHALLENGE m_mapChallenge;
};

#endif
