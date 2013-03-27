#ifndef _BASE_HEAD__H_
#define _BASE_HEAD__H_

#include "../Share/condition.h"

Mutex g_mut;
Condition g_cond(&g_mut);

#endif


