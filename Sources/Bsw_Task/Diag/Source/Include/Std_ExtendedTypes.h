#if 1
#include "ECU.h"

#define FAR  __far

#if(ECU_RAM_USE_MODE)      
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM 
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM 
#endif      
#else
#pragma DATA_SEG DEFAULT 
#endif

#endif