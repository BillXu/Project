#ifndef   _LOGIN_TREATOR__H_
#define   _LOGIN_TREATOR__H_

#include "../Base/TreatorBase.h"
#include "../Base/ThreadMod.h"
#include <errno.h>

class  CLoginTreator : public ITreatorBase, public CThreadT
{
public:
	static CLoginTreator* _instance()
	{
		static CLoginTreator* __inst = NULL;
		if(!__inst)
		{
			__inst = new CLoginTreator;
		}
		return __inst;
	}
	CLoginTreator();
	~CLoginTreator();
public:
	void OnLogin(stC2SMsgLogin* pMsg, uint64_t guid);
	//void Push(InternalMsg    * pInMsg);
	
	void __run();
private:
	bool m_bRun;
	
};

#define LoginTreator CLoginTreator::_instance()

#endif

