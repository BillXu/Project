//
//  RoomData.h
//  Gold
//
//  Created by Xu BILL on 13-8-1.
//
//

#ifndef __Gold__RoomData__
#define __Gold__RoomData__
#include "NetWorkManager.h"
#include "MessageDefine.h"
#include "CommonDefine.h"
#include <string>
struct stRoomPeerData
:public stRoomPeerBrifInfo
{
    bool bRecievedUserDefinePhoto ;
    bool IsActive();
};

class CRoomLayer ;
class CRoomData
:public CNetMessageDelegate
{
public:
    void Init(CRoomLayer* player );
    bool OnMessage( RakNet::Packet* pMsg );
    stRoomPeerData* GetRoomPeerDataBySessionID( unsigned int nSessionID ) ;
    char GetPlayerCount();
    char GetActiveCount();
    // client index : the players sequnced is the same as the server , but in the client , selft must the last idx , as the UI need ;
    stRoomPeerData* GetRoomPeerDataByClientIdx( char nClientIdx ); 
protected:
    char ConvertoServerIdx(char nClientIdx);
    char ConvertToClientIdx(char nServerIdx ) ;
    friend class CRoomLayer ;
protected:
    CRoomLayer* m_pRoomLayer ;
    stRoomPeerData m_vRoomPeers[MAX_ROOM_PEER] ;  // server idx ;
    int m_nSingleBetCoin;
    int m_nTotalBetCoin;
    unsigned short m_nRound ;
    unsigned char m_eRoomSate ;  // eRoomState
};
#endif /* defined(__Gold__RoomData__) */
