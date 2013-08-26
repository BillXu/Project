//
//  EventHandleQueue.h
//  God
//
//  Created by Xu BILL on 12-10-19.
//
//

#ifndef God_EventHandleQueue_h
#define God_EventHandleQueue_h
#include "cocos2d.h"
#include "EventDefine.h"
#include <list>
#include <map>
USING_NS_CC ;

typedef void (CCObject::*SEL_CCEventHandler)( stEventArg* pArg );

#define ccevent_selector(_SELECTOR) (SEL_CCEventHandler)(&_SELECTOR)

class CEventInvocation
{
public:
    CEventInvocation( CCObject* pTarget , SEL_CCEventHandler pSelector);
    void Invoke( stEventArg* pArg );
    bool IsEqual(CCObject* pTarget , SEL_CCEventHandler pSelector);
    bool IsEquilTarget(CCObject* pTarget){ return pTarget == this->pTarget ;}
protected:
    CCObject* pTarget ;
    SEL_CCEventHandler pHandle ;
};

class CEventHandleQueue
{
public:
    typedef std::list<CEventInvocation*> LIST_INVOCATION ;
    typedef std::map<eEventType, LIST_INVOCATION> MAP_EVENT_HANDLES ;
public:
    static CEventHandleQueue* SharedEventHandleQueue();
    CEventHandleQueue();
    ~CEventHandleQueue();
    void RegisterEventHandle( eEventType eEvent , CCObject* pTarget , SEL_CCEventHandler pSelector ) ;
    void RemoveEventHandle( eEventType eEvent , CCObject* pTarget , SEL_CCEventHandler pSelector  );
    void RemoveEventHandle(eEventType eEvent , CCObject* pTarget);
    void SendEvent( eEventType eEvent , stEventArg* pArg );
protected:
    bool IsHasEventHandle( eEventType eEvent , CCObject* pTarget , SEL_CCEventHandler pSelector );
protected:
    MAP_EVENT_HANDLES mMapHandles ;
};

#endif
