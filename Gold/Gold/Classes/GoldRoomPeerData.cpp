//
//  GoldRoomPeerData.cpp
//  Gold
//
//  Created by Xu BILL on 13-8-26.
//
//

#include "GoldRoomPeerData.h"
#include "PlayerData.h"
#include "ClientApp.h"
void stGoldRoomPeerData::SetData(stRoomPeerBrifInfo *pdata)
{
    setCoin(pdata->nCoin) ;
    setUserDefinePhotoID(pdata->nUserDefinePhotoID) ;
    setBetCoin(pdata->nBetCoin) ;
    setIdx(pdata->nIdx) ;
    setPeerState((eRoomPeerState)pdata->ePeerState) ;
    strTitle = "defaultTitle" ;
    setName(pdata->pName) ;
    setDefaulPhotoID(pdata->nDefaulPhotoID) ;
    setSessionID(pdata->nSessionID) ;
    setIsRecievedUserDefinePhoto(false) ;
}

bool stGoldRoomPeerData::IsActive()
{
    return (getPeerState() == eRoomPeer_Look || getPeerState() == eRoomPeer_Unlook );
}

unsigned int stGoldRoomPeerData::getCoin()
{
    return nCoin ;
}

void stGoldRoomPeerData::setCoin(unsigned int var)
{
    nCoin = var ;
}

void stGoldRoomPeerData::AddBetCoin(int nCoin )
{
    setBetCoin(getBetCoin() + nCoin) ;
    setCoin(getCoin()-nCoin) ;
}

// self gold room data
stGoldRoomSelfData::stGoldRoomSelfData()
{
    CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    setUserDefinePhotoID(pbasedata->getUserDefinePhotoID()) ;
    setBetCoin(0) ;
    setPeerState(eRoomPeer_None);
    strTitle = pbasedata->getTitle() ;
    setName(pbasedata->getName()) ;
    setDefaulPhotoID(pbasedata->getDefaulPhotoID()) ;
    setSessionID(pbasedata->getSessionID()) ;
    setIsRecievedUserDefinePhoto(true) ;
}

unsigned int stGoldRoomSelfData::getCoin()
{
    CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    return pbasedata->getCoin() ;
}

void stGoldRoomSelfData::setCoin(unsigned int var)
{
    CPlayerBaseData* pbasedata = CClientApp::SharedClientApp()->GetPlayerData()->GetBaseData() ;
    pbasedata->setCoin(var) ;
}
