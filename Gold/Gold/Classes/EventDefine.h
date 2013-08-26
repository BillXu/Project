//
//  EventDefine.h
//  God
//
//  Created by Xu BILL on 12-10-19.
//
//

#ifndef God_EventDefine_h
#define God_EventDefine_h
#include "MessageDefine.h"
enum eEventType
{
    eEvent_None,
    eEvent_RecieveBaseData,
    eEvent_Max,
};

// event arg ;
struct stEventArg
{
    unsigned int nID ;
    eEventType eEventInvoked ;
    void* pUserData ;
public:
    stEventArg(){ nID = 0 ; eEventInvoked = eEvent_None ; pUserData = NULL ;}
    virtual ~stEventArg(){};
} ;
#endif
