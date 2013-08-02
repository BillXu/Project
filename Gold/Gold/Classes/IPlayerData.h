//
//  IPlayerData.h
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#ifndef __Gold__IPlayerData__
#define __Gold__IPlayerData__

#include <iostream>
struct stMsg ;
class IPlayerDataBase
{
public:
    enum ePlayerDataType
    {
        ePlayerData_None,
        ePlayerData_BaseData ,
        ePlayerData_Max,
    } ;
public:
    virtual ~IPlayerDataBase(){ m_nVersionID=0; }
    virtual void Init(){}
    virtual void PreLoading(){}
    virtual void Loading(){}
    virtual void Save(){}
    virtual bool OnMessage( stMsg* pMessage ){ return  false ; }
protected:
    unsigned int  m_nVersionID ;  // different version , may different ready serail and different meathed ;
};

#endif /* defined(__Gold__IPlayerData__) */
