//global.h

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "printk.h"


#ifndef VERSION
#define VERSION   4
#define PATCHLEVEL 1
#endif

#ifndef MAXPOINT
#define MAXPOINT 8
#endif


#define HBTK_NAME          __stringify(HNAME)

#define CAN_ENABLE		(1<<0)
#define SOUND_ENABLE	(1<<1)

#define MAX_POINT_COUNT			MAXPOINT	//一个控制器最大可带点个数定义
#define MAX_DELAY				30




#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif



#endif
