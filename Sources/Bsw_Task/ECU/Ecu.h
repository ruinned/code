// Ecu.h
#ifndef ECU_H
#define ECU_H

#include "EcuConf.h"
#include "Cpu.h"

#define ECU_DELAYMS(ms) Cpu_Delay100US((ms) * 10) // max timer is 6553 ms
#define GET_ECU_RESET_SOURCE()  Cpu_GetResetSource()  //byte Cpu_GetResetSource(void),the return value is the reset source
#define ECU_DELAY100US(us) Cpu_Delay100US(us) //

/*
 * Define for deal Banked RAM In S12X
 */
#define ECU_RAM_USE_DEFAULT 0 
#define ECU_RAM_USE_BANKED  1
//#if ((ECU_CATEGORY == M_BCU_D) || (ECU_CATEGORY == M_BCU_C))
// bcu_d and bcu_c use S12X
/*  Use case:    
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
    
    Place code here
    .........
    
    #pragma DATA_SEG DEFAULT    
 */
#define ECU_RAM_USE_MODE ECU_RAM_USE_BANKED
#define ECU_RAM_USE_GPGAE
//#define ECU_RAM_USE_RPGAE
#ifdef ECU_RAM_USE_RPGAE 
  #define RAMPTR  __rptr
#else 
  #ifdef ECU_RAM_USE_GPGAE
  #define RAMPTR __far
  #endif
#endif /* #ifdef ECU_RAM_USE_RPGAE  */

#if ((defined(ECU_RAM_USE_GPGAE)) && (defined(ECU_RAM_USE_RPGAE) ))
  #error "Cannot Use Both kind of page mode, Just can select one, ECU_RAM_USE_RPGAE or ECU_RAM_USE_GPGAE"
#else 
  #if ((!defined(ECU_RAM_USE_GPGAE)) && (!defined(ECU_RAM_USE_RPGAE) ))
  #error "Need define ECU_RAM_USE_RPGAE or ECU_RAM_USE_GPGAE!!"
  #endif
#endif
  
//#else
// for bmu and bmu_r
//#define ECU_RAM_USE_MODE ECU_RAM_USE_DEFAULT
//#define RAMPTR /* */
//#endif 
 
 

#endif // ECU_H