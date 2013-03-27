#ifndef   _SERVER_TREATOR__H_
#define   _SERVER_TREATOR__H_

#include "../Base/TreatorBase.h"
#include "../Base/ThreadMod.h"
#include <errno.h>

class  CServerTreator : public ITreatorBase, public CThreadT
{
public:
	static CServerTreator* _instance()
	{
		static CServerTreator* __inst = NULL;
		if(!__inst)
		{
			__inst = new CServerTreator;
		}
		return __inst;
	}
	CServerTreator();
	~CServerTreator();
public:
	void HandleLoginSucess(InternalMsg* pInmsg);
	void HandleLoginErr(InternalMsg* pInmsg);
	
	void __run();
private:
	bool m_bRun;
	
};

#define ServerTreator CServerTreator::_instance()

#endif

