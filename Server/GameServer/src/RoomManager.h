#pragma once
#include "CommonDefine.h"
#include <map>
#include "Room.h"
class CRoomManager
{
public:
	CRoomManager(){m_pRoom = NULL ;}
	~CRoomManager(){if (m_pRoom ) delete m_pRoom ;}
	void Init(){ m_pRoom = new CRoom ;m_pRoom->Init(++CRoom::s_RoomID , 5 ) ; }
	CRoom* GetRoom(char cRoomType , char cRoomLevel ){ return m_pRoom ;}
protected:
	CRoom* m_pRoom ;
};