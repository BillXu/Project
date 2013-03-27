#ifndef _COMMON__H_
#define _COMMON__H_

#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <stdint.h>

//#include "../Base/Log.h"

#define SERVER_END_TIME_OUT		3//3seconds

//服务器配置文件
#define SERVER_CONFIG_FILE 			"../Config/config.xml"
#define LEVEL_CONFIG_FILE			"../Config/level.txt"
#define REWARD_CONFIG_FILE			"../Config/reward.txt"
#define TASK_CONFIG_FILE			"../Config/task.txt"
#define GENERAL_CONFIG_FILE			"../Config/general.txt"
#define EQUIP_CONFIG_FILE			"../Config/equip.txt"
#define ITEM1_CONFIG_FILE			"../Config/item1.txt"
#define ITEM2_CONFIG_FILE			"../Config/item2.txt"
#define ITEM3_CONFIG_FILE			"../Config/item3.txt"
#define ITEM4_CONFIG_FILE			"../Config/item4.txt"
#define SKILL_CONFIG_FILE			"../Config/skill.txt"
#define CONFER_CONFIG_FILE			"../Config/confer.txt"
#define EXCHANGE_CONFIG_FILE		"../Config/exchange.txt"
#define SHOP_CONFIG_FILE			"../Config/shop.txt"
#define TOP50_CONFIG_FILE			"../Config/top50.txt"
//#define TOP50_HONOR_CONFIG_FILE		"../Config/top50honor.txt"

#ifndef WEGHT_SUM
#define WEIGHT_SUM				10000
#endif
#define MAX_NAME_SIZE			128

#define BUF_SIZE1				1024
#define BUF_SIZE2				2048
#define BUF_SIZE10				10240
#define BUF_SIZE_1M				512*1024
#define MAX_PASS_SIZE			256
#define MAX_EMAIL_SIZE			256
#define MAX_MOBILE_LEN 			12
#define MAX_IP_LENGTH 			16

#define	HANDLE_THREAD_SIZE 		2

#define MAX_PATH_SIZE 			256
#define MAX_URL_SIZE			256

#define MAX_PROPPASS_SIZE		10
#define STRING_BUF_SIZE			2048

#define MAX_TOKEN_LEN 			20
#define MAX_EMAIL_LEN 			20
#define MAX_IDCARD_LEN 			20

#define BUILD_UINT16(h,l)	( (((uint16_t)(h)) << 8) | (((uint16_t)(l)) & 0xff) )
#define BUILD_UINT32(h,l)	( (((uint32_t)(h)) << 16) | (((uint32_t)(l)) & 0xffff) )
#define BUILD_UINT64(h,l)	( (((uint64_t)(h)) << 32) | (((uint64_t)(l)) & 0xffffffff) )

#define LOW_UINT8(u) 	( (uint8_t)(u) )
#define HIG_UINT8(u)	( (uint8_t)(((uint16_t)(u) & 0xff00 ) >> 8) )
#define LOW_UINT16(u)	( (uint16_t)(u) )
#define HIG_UINT16(u)	( (uint16_t)((((uint32_t)(u)) & 0xffff0000) >> 16 ) )
#define LOW_UINT32(u)	( (uint32_t)(u) )
#define HIG_UINT32(u)	( (uint32_t)((((uint64_t)(u)) & 0xffffffff00000000) >> 32) )

uint8_t	 BuildUint8(uint8_t* &p);
uint16_t BuildUint16(uint8_t* &p);
uint32_t BuildUint32(uint8_t* &p);
uint64_t BuildUint64(uint8_t* &p);
int		BuildString(uint8_t* &p, char* buf, int bufSize);

bool	PackUint8(uint8_t u, uint8_t* p);
bool	PackUint16(uint16_t u, uint8_t* p);
bool    PackUint32(uint32_t u, uint8_t* p);
bool    PackUint64(uint64_t u, uint8_t* p);
int		PackString(const char* str, uint8_t* p);
int		PackHead(uint8_t cmd, uint8_t subCmd, uint8_t* p);
bool	PackHeadLen(uint32_t len, uint8_t* p);

uint64_t getMSTime();

uint32_t getSecTime();

enum _DB_HASH_TABLE
{
	DB_READ = 0,
	DB_WRITE,
	
	DB_CONN_COUNT
};

/*typedef struct _ServerAddrParam
{
	char ip[16];
	int port;
}ServerAddrParam;*/


#endif


