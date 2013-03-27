
#ifndef _RELATION__H_
#define _RELATION__H_

#include "../Base/InternalMsg.h"
#include "../Share/Common.h"

enum eRelation
{
	eNone,
	eFoe,		//宿敌
	eFriend,	//好友
	eEND
};
class CRelation
{
public:
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
	inline unsigned char GetChangeFlag()
	{
		return changeFlag;
	}
	inline eRelation GetType()
	{
		return m_type;
	}
	inline unsigned int GetUserId()
	{
		return otherUserId;
	}
	inline unsigned int GetTime()
	{
		return addTime;
	}
	void SetTime(unsigned int tim);
	void SetType(unsigned char type);
	void SetUserId(unsigned int userId);

    CRelation();
	~CRelation();
private:
	unsigned int otherUserId;		//
	eRelation m_type;				// 
	unsigned char changeFlag;		//3增 ; 2 删 ;3 改
	unsigned int addTime;
};

#endif
