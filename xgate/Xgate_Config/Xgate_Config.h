#ifndef XGATE_CONFIG_H
#define XGATE_CONFIG_H


/* Module Enable swith define */
#define XGATE_ENABLE_ACCP
#define XGATE_ENABLE_EXTFLASH
#define XGATE_ENABLE_REALTIME
#define XGATE_ENABLE_SUPPLYVOLTDET
#define XGATE_ENABLE_TIME
#define XGATE_ENABLE_SWI0
#define XGATE_ENABLE_SWI1
#define XGATE_ENABLE_SWI2
#define XGATE_ENABLE_SWI3
#define XGATE_ENABLE_SWI4
#define XGATE_ENABLE_SWI5
#define XGATE_ENABLE_SWI6

/* semaphore define */
#define XGATE_ACCP_SEMAPHORE 0
#define XGATE_EXTFLASH_SEMAPHORE 1
#define XGATE_REALTIME_SEMAPHORE 2

/* define for xgate base timer counter */
#define X_TM_MS(v) (v / 1)


#endif
