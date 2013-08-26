//
//  EventHandleQueue.cpp
//  God
//
//  Created by Xu BILL on 12-10-19.
//
//

#include "EventHandleQueue.h"
CEventInvocation::CEventInvocation(CCObject* pTarget , SEL_CCEventHandler pSelector)
{
    this->pTarget = pTarget ;
    this->pHandle = pSelector ;
}

void CEventInvocation::Invoke(stEventArg *pArg)
{
    (pTarget->*pHandle)(pArg);
}

bool CEventInvocation::IsEqual(cocos2d::CCObject *pTarget, SEL_CCEventHandler pSelector)
{
    return pTarget == this->pTarget && pSelector == pHandle ;
}

// event handle queue
CEventHandleQueue* CEventHandleQueue::SharedEventHandleQueue()
{
    static CEventHandleQueue g_sEventHandle ;
    return &g_sEventHandle ;
}

CEventHandleQueue::CEventHandleQueue()
{
    mMapHandles.clear() ;
}

CEventHandleQueue::~CEventHandleQueue()
{
    MAP_EVENT_HANDLES::iterator iterMap = mMapHandles.begin() ;
    for ( ; iterMap != mMapHandles.end() ; ++iterMap )
    {
        LIST_INVOCATION& listInvocation = iterMap->second ;
        LIST_INVOCATION::iterator iter = listInvocation.begin() ;
        for ( ; iter != listInvocation.end() ; ++iter )
        {
            delete *iter ;
            *iter = NULL ;
        }
        listInvocation.clear() ;
    }
    mMapHandles.clear() ;
}

void CEventHandleQueue::RegisterEventHandle(eEventType eEvent, cocos2d::CCObject *pTarget, SEL_CCEventHandler pSelector)
{
    if ( IsHasEventHandle(eEvent, pTarget, pSelector))
        return ;
    CEventInvocation* pEventInoke = new CEventInvocation(pTarget, pSelector) ;
    MAP_EVENT_HANDLES::iterator iter = mMapHandles.find(eEvent) ;
    if ( iter == mMapHandles.end() )
    {
        LIST_INVOCATION listInvok ;
        listInvok.push_back(pEventInoke) ;
        mMapHandles.insert(MAP_EVENT_HANDLES::value_type(eEvent,listInvok));
    }
    else
    {
        LIST_INVOCATION& listInok = iter->second ;
        listInok.push_back(pEventInoke) ;
    }
}

void CEventHandleQueue::RemoveEventHandle(eEventType eEvent, cocos2d::CCObject *pTarget, SEL_CCEventHandler pSelector)
{
    if ( !IsHasEventHandle(eEvent, pTarget, pSelector))
        return ;
    MAP_EVENT_HANDLES::iterator iter = mMapHandles.find(eEvent) ;
    if ( iter == mMapHandles.end() )
        return ;
    LIST_INVOCATION& listInvoke = iter->second ;
    LIST_INVOCATION::iterator iterInvoke = listInvoke.begin() ;
    for ( ; iterInvoke != listInvoke.end() ; ++iterInvoke )
    {
        CEventInvocation* pEventIn = *iterInvoke ;
        if ( pEventIn->IsEqual(pTarget, pSelector) )
        {
            delete pEventIn ;
            listInvoke.erase(iterInvoke) ;
            return ;
        }
    }
    return ;
}

void CEventHandleQueue::RemoveEventHandle(eEventType eEvent , CCObject* pTarget)
{
    MAP_EVENT_HANDLES::iterator iter = mMapHandles.find(eEvent) ;
    if ( iter == mMapHandles.end() )
        return ;
    LIST_INVOCATION& listInvoke = iter->second ;
    LIST_INVOCATION::iterator iterInvoke = listInvoke.begin() ;
    LIST_INVOCATION listWillRemove ;
    for ( ; iterInvoke != listInvoke.end() ; ++iterInvoke )
    {
        CEventInvocation* pEventIn = *iterInvoke ;
        if ( pEventIn->IsEquilTarget(pTarget) )
        {
            listWillRemove.push_back(pEventIn) ;
        }
    }
    
    LIST_INVOCATION::iterator iter_remove = listWillRemove.begin() ;
    for ( ; iter_remove != listWillRemove.end(); ++iter_remove )
    {
        LIST_INVOCATION::iterator iter_r = listInvoke.begin() ;
        for ( ; iter_r != listInvoke.end(); ++iter_r )
        {
            if ( *iter_remove == *iter_r )
            {
                delete *iter_r ;
                listInvoke.erase(iter_r) ;
                break ;
            }
        }
    }
    listWillRemove.clear() ;
    return ;
}

void CEventHandleQueue::SendEvent(eEventType eEvent, stEventArg *pArg)
{
    MAP_EVENT_HANDLES::iterator iter = mMapHandles.find(eEvent) ;
    if ( iter == mMapHandles.end() )
        return ;
    pArg->eEventInvoked = eEvent ;
    LIST_INVOCATION& vInokcation = iter->second ;
    LIST_INVOCATION::iterator iterInvoke = vInokcation.begin() ;
    for ( ; iterInvoke != vInokcation.end() ; ++iterInvoke )
    {
        CEventInvocation* pEventInvoke = *iterInvoke ;
        pEventInvoke->Invoke(pArg) ;
    }
}

bool CEventHandleQueue::IsHasEventHandle(eEventType eEvent, cocos2d::CCObject *pTarget, SEL_CCEventHandler pSelector)
{
    MAP_EVENT_HANDLES::iterator iter = mMapHandles.find(eEvent) ;
    if ( iter == mMapHandles.end() )
        return false;
    LIST_INVOCATION& vInokcation = iter->second ;
    LIST_INVOCATION::iterator iter_inoke = vInokcation.begin() ;
    for ( ; iter_inoke != vInokcation.end() ; ++iter_inoke )
    {
        if ( (*iter_inoke)->IsEqual(pTarget, pSelector) )
            return true ;
    }
    return false ;
}

