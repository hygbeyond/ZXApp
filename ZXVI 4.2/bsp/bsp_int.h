
#ifndef __BSP_INT_H__
#define __BSP_INT_H__

#define __SVC_FUNC( func, SVC_cnt )  \
    extern void func(void);  \
    void func(void) { \
        asm volatile ("SVC " __stringify(SVC_cnt) "\n"); }

#endif //__BSP_INT_H__


