


#ifndef __MYLIST_H__
#define __MYLIST_H__


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a )             (sizeof( a) / sizeof( a[0] ))
#endif


#ifndef LIST_BUF_SIZE
#define LIST_BUF_SIZE               (128)               // serial buffer in bytes (power 2)
#endif

#define LIST_BUF_MASK               (LIST_BUF_SIZE-1ul)  // buffer size mask

/* Buffer read / write macros */
#define LIST_BUF_RESET(serBuf)      (serBuf.rd = serBuf.cnt = serBuf.wr = 0)
#define LIST_BUF_COUNT(serBuf)      (serBuf.cnt) 
#define LIST_BUF_FULL(serBuf)       (serBuf.cnt >= ARRAY_SIZE( serBuf.data) )


#define LIST_BUF_GET( serBuf, pdata) \
    if( !LIST_BUF_FULL( serBuf ) ) { \
        serBuf.cnt++; pdata = &serBuf.data[serBuf.wr++]; \
        if( serBuf.wr >= ARRAY_SIZE( serBuf.data )) serBuf.wr=0; \
    }else pdata = NULL;

#define LIST_BUF_RD(serBuf, dataOut)                                      \
		{ (dataOut) = serBuf.data[serBuf.rd++]; if( serBuf.rd >= ARRAY_SIZE(serBuf.data)) serBuf.rd=0;  \
			serBuf.cnt--;                                                 \
		}
			 
#define LIST_BUF_WR(serBuf, dataIn)                                       \
		if( serBuf.cnt < ARRAY_SIZE( serBuf.data ) ){                     \
			serBuf.cnt++;                                                 \
			serBuf.data[serBuf.wr++] = (dataIn);                       \
			if( serBuf.wr >= ARRAY_SIZE(serBuf.data)) serBuf.wr=0; }

#define LIST_TYPE( __name, __type )     \
    typedef struct __ ## __name {       \
        __type data[LIST_BUF_SIZE];     \
        uint16 wr;                      \
        uint16 rd;                      \
        uint16 cnt;                     \
    } __name;


#define LIST_TYPE_EXT( __name, __type, size )     \
    typedef struct __ ## __name {       \
        __type data[size];        \
        uint16 wr;             \
        uint16 rd;             \
        uint16 cnt;               \
    } __name;


#endif //__MYLIST_H__



