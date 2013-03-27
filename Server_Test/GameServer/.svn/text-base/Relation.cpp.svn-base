
#include <assert.h>
#include "Relation.h"

CRelation::CRelation()
{
	otherUserId = 0;
	m_type = eNone;
	changeFlag = 0;
}


CRelation::~CRelation()
{

}

void CRelation::SetType(unsigned char type)
{
	if (1 == type)
	{
		m_type = eFoe;
	} 
	else if(2 == type)
	{
		m_type = eFriend;
	}
	else
	{
		assert(0);
	}
}

void CRelation::SetTime(unsigned int tim)
{
	addTime = tim;
}

void CRelation::SetUserId(unsigned int userId)
{
	otherUserId = userId;
}



