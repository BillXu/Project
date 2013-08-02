//
//  PlayerData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#include "PlayerData.h"
CPlayerData::CPlayerData()
{
    memset(m_vPlayerData, 0, sizeof(m_vPlayerData)) ;
}

CPlayerData::~CPlayerData()
{
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i] )
        {
            delete m_vPlayerData[i] ;
            m_vPlayerData[i] = NULL ;
        }
    }
}

void CPlayerData::Init()
{
    // set up 
    m_vPlayerData[IPlayerDataBase::ePlayerData_BaseData] = new CPlayerBaseData ;
    
    // init all data ;
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i] )
        {
            m_vPlayerData[i]->Init() ;
        }
    }
}

void CPlayerData::PreLoading()
{
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i] )
        {
            m_vPlayerData[i]->PreLoading() ;
        }
    }
}

void CPlayerData::Loading()
{
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i] )
        {
            m_vPlayerData[i]->Loading() ;
        }
    }
}

void CPlayerData::Save()
{
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i] )
        {
            m_vPlayerData[i]->Save() ;
        }
    }
}

bool CPlayerData::OnMessage( stMsg* pMessage )
{
    for ( int i = IPlayerDataBase::ePlayerData_None ; i < IPlayerDataBase::ePlayerData_Max ; ++i )
    {
        if ( m_vPlayerData[i]->OnMessage(pMessage) )
        {
            return true ;
        }
    }
    return false ;
}

IPlayerDataBase* CPlayerData::GetPlayerData(IPlayerDataBase::ePlayerDataType ePlayerDataType )
{
    if ( ePlayerDataType >= IPlayerDataBase::ePlayerData_None && ePlayerDataType < IPlayerDataBase::ePlayerData_Max )
    {
        return m_vPlayerData[ePlayerDataType] ;
    }
    return NULL ;
}

