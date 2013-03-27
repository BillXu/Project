#ifndef _RELATION_MGR__H_
#define _RELATION_MGR__H_

#include "Relation.h"
#include "../Share/Common.h"
#include "../Share/mutex.h"
#include <map>
using namespace std;

class CUser;
class CRelationMgr
{
public:
	int Add(CRelation* rel);
	int Del(CRelation* rel);
	int AddFoe(unsigned int userId);
	CRelation* GetFoe(unsigned int userId);
	int AddFoe(CRelation* rel);
	int DelFoe(CRelation* rel);
	int AddFriend(CRelation* rel);
	int DelFrined(CRelation* rel);

	short CopyRelationInfo(unsigned int roleId, char* buf, short bufLen);
	int CopyRelationList(unsigned char type, unsigned short count, char* buf, unsigned short* cnt);
public:
	void SetUser(CUser* pUser)
	{
		assert(pUser);
		m_pUser = pUser;
	}
	CRelationMgr();
	~CRelationMgr();
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
	typedef std::map<uint32_t, CRelation*> MAP_RELATION;
	MAP_RELATION m_vFoe;
	MAP_RELATION m_vFriend;
};

#endif

