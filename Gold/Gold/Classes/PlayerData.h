//
//  PlayerData.h
//  Gold
//
//  Created by Xu BILL on 13-8-2.
//
//

#ifndef __Gold__PlayerData__
#define __Gold__PlayerData__

#include <iostream>
#include "IPlayerData.h"
#include "PlayerBaseData.h"
class CPlayerData
{
public:
    CPlayerData();
    ~CPlayerData();
    void Init();
    void PreLoading();
    void Loading();
    void Save();
    bool OnMessage( stMsg* pMessage );
    IPlayerDataBase* GetPlayerData(IPlayerDataBase::ePlayerDataType ePlayerDataType );
    CPlayerBaseData* GetBaseData(){ return (CPlayerBaseData*)GetPlayerData(IPlayerDataBase::ePlayerData_BaseData) ;}
protected:
    IPlayerDataBase* m_vPlayerData[IPlayerDataBase::ePlayerData_Max];
};

#endif /* defined(__Gold__PlayerData__) */
