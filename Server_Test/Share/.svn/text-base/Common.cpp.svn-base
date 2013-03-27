#include "Common.h"

uint8_t	 BuildUint8(uint8_t* &p)
{
	uint8_t ret = (uint8_t)(*p);
	p += 1;
	return ret;
}

uint16_t BuildUint16(uint8_t* &p)
{
	uint16_t ret = (uint16_t)(BUILD_UINT16(p[0],p[1]));
	p += 2;
	return ret;
}

uint32_t BuildUint32(uint8_t* &p)
{
	uint32_t ret = (uint32_t)BUILD_UINT32( BUILD_UINT16(p[0],p[1]), BUILD_UINT16(p[2],p[3]) );
	p += 4;
	return ret;
}

uint64_t BuildUint64(uint8_t* &p)
{
	uint64_t ret = (uint64_t)(BUILD_UINT64( BUILD_UINT32(BUILD_UINT16(p[0],p[1]),BUILD_UINT16(p[2],p[3])), BUILD_UINT32(BUILD_UINT16(p[4],p[5]),BUILD_UINT16(p[6],p[7])) ));
	p += 8;
	return ret;
}

int	BuildString(uint8_t* &p, char* buf, int bufSize)
{
	if(bufSize <= 0 || !p || !buf)	
		return -1;
	memset(buf, 0, bufSize);
	uint16_t len = BuildUint16(p);
	if(len <= 0 || len > bufSize)	
		return -1;
	memcpy(buf, p, len);
	p += len;
	
	return len;
}

bool PackUint8(uint8_t u, uint8_t* p)
{
	if(!p) return false;
	*p = u;

	return true;
}

bool PackUint16(uint16_t u, uint8_t* p)
{
	if(!p) return false;
	p[0] = HIG_UINT8(u);
	p[1] = LOW_UINT8(u);

	return true;
}

bool PackUint32(uint32_t u, uint8_t* p)
{
	if(!p) return false;
	uint16_t l,h;
	h = HIG_UINT16(u);
	l = LOW_UINT16(u);
	if(PackUint16(h, p+0))
	{
		return PackUint16(l, p+2);
	}
	else
	{
		return false;
	}
}

bool PackUint64(uint64_t u, uint8_t* p)
{
	if(!p) return false;
	uint32_t l,h;
	h = HIG_UINT32(u);
	l = LOW_UINT32(u);
	if(PackUint32(h, p+0))
	{
		return PackUint32(l, p+4);
	}
	else
	{
		return false;
	}
}

int PackString(const char* str, uint8_t* p)
{
	if(!str || !p) return -1;
	uint16_t len = (uint16_t)strlen(str)+1;//末尾加\0
	assert(PackUint16(len, p));
	memcpy(p+sizeof(uint16_t), str, len);

	return len + sizeof(uint16_t);
}

int PackHead(uint8_t cmd, uint8_t subCmd, uint8_t* p)
{
	int i = 4;
	PackUint8(cmd, p+i);
	i++;
	PackUint16(0, p+i);//key1
	i += 2;
	PackUint8(subCmd, p+i);
	i++;
	PackUint16(0, p+i);//key2
	i += 2;
	
	return i;
}

bool PackHeadLen(uint32_t len, uint8_t* p)
{
	return PackUint32(len-4, p);
}

uint64_t getMSTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

uint32_t getSecTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec + (tv.tv_usec / 1000000));
}


