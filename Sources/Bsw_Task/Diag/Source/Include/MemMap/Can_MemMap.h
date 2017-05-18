/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Can_MemMap.h>
 *  @brief      <Briefly describe file(one line)>
 *  
 *  <Compiler: TASKING    MCU:TC17XX>
 *  
 *  @author     <chen xue hua>
 *  @date       <2013-02-27>
 */
/*============================================================================*/
/* @req MEMMAP020 @req MEMMAP001 @req MEMMAP002 @req MEMMAP005 @req MEMMAP015 */
/* @req MEMMAP016 */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130227  chenxuehua  Initial version
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define CAN_MEMMAP_VENDOR_ID  62
#define CAN_MEMMAP_MODULE_ID  0
#define CAN_MEMMAP_AR_MAJOR_VERSION  1
#define CAN_MEMMAP_AR_MINOR_VERSION  1
#define CAN_MEMMAP_AR_PATCH_VERSION  2
#define CAN_MEMMAP_SW_MAJOR_VERSION  1
#define CAN_MEMMAP_SW_MINOR_VERSION  0
#define CAN_MEMMAP_SW_PATCH_VERSION  0
#define CAN_MEMMAP_VENDOR_API_INFIX  0

/* 
 * <SIZE> -- BOOLEAN/8BIT/16BIT/32BIT/UNSPECIFIED 
 */

/* 
 * <MSN>_START_SEC_CODE
 * <MSN>_STOP_SEC_CODE
 * To be used for mapping code to applicaiton block,boot block,external flash etc. 
 */

/* 
 * <MSN>_START_SEC_CODE_FAST
 * <MSN>_STOP_SEC_CODE_FAST
 * To be used for mapping code to local block,boot block of ISR API. 
 */

/* 
 * <MSN>_START_SEC_VAR_NOINIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_NOINIT_<SIZE>
 * To be used for all global or static variables that are never initialized. 
 */

/* 
 * <MSN>_START_SEC_VAR_POWER_ON_INIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_POWER_ON_INIT_<SIZE>
 * To be used for all global or static variables that are initialized only after power on reset. 
 */

/*
 * <MSN>_START_SEC_VAR_<SIZE>
 * <MSN>_STOP_SEC_VAR_<SIZE>
 * To be used for all global or static variables that are initialized after every reset(the normal case).
 */

/* 
 * <MSN>_START_SEC_VAR_FAST_NOINIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_NOINIT_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime. 
 */

/* 
 * <MSN>_START_SEC_VAR_FAST_POWER_ON_INIT_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_POWER_ON_INIT_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime. 
 */

/* 
 * <MSN>_START_SEC_VAR_FAST_<SIZE>
 * <MSN>_STOP_SEC_VAR_FAST_<SIZE>
 * To be used for all global or static variables that have at least one of the following properties:
 *      *accessed bitwise
 *      *frequently used
 *      *high number of accesses in source code
 *  Some platforms allow the use of bit instructions for variables located in this specific RAM area as
 *  well as shorter addressing instructions.This saves code and runtime. 
 */

/*
 * <MSN>_START_SEC_CONST_<SIZE>
 * <MSN>_STOP_SEC_CONST_<SIZE>
 * To be used for all global or static constants.
 */

/*
 * <MSN>_START_SEC_CONST_FAST_<SIZE>
 * <MSN>_STOP_SEC_CONST_FAST_<SIZE>
 * To be used for all global or static constants in fast segment block.
 */

/*
 * <MSN>_START_CONST_PBCFG
 * <MSN>_STOP_CONST_PBCFG
 * Constants with attributes that show that they reside in one segment for module configuration.
 */

/*
 * <MSN>_START_CONST_PBCFG_ROOT
 * <MSN>_STOP_CONST_PBCFG_ROOT
 * Constants with attributes that show that they reside in one segment for module configuration for root structure.
 */

/* @req MEMMAP010 @req MEMMAP004 @req MEMMAP021 @req MEMMAP003 @req MEMMAP018 */
/*=======[M A C R O S]========================================================*/
#if defined (START_WITH_IF)

/*=======[C A N]==============================================================*/

#elif defined (CAN_START_SEC_CODE)
   #undef      CAN_START_SEC_CODE
   #define START_SEC_CODE
#elif defined (CAN_STOP_SEC_CODE)
   #undef      CAN_STOP_SEC_CODE
   #define STOP_SEC_CODE

#elif defined (CAN_START_SEC_CODE_FAST)
   #undef      CAN_START_SEC_CODE_FAST
   #define START_SEC_CODE_FAST
#elif defined (CAN_STOP_SEC_CODE_FAST)
   #undef      CAN_STOP_SEC_CODE_FAST
   #define STOP_SEC_CODE_FAST

#elif defined (CAN_START_SEC_VAR_NOINIT_BOOLEAN)
   #undef      CAN_START_SEC_VAR_NOINIT_BOOLEAN
   #define START_SEC_VAR_NOINIT_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_NOINIT_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_NOINIT_BOOLEAN
   #define STOP_SEC_VAR_NOINIT_BOOLEAN

#elif defined (CAN_START_SEC_VAR_NOINIT_8BIT)
   #undef      CAN_START_SEC_VAR_NOINIT_8BIT
   #define START_SEC_VAR_NOINIT_8BIT
#elif defined (CAN_STOP_SEC_VAR_NOINIT_8BIT)
   #undef      CAN_STOP_SEC_VAR_NOINIT_8BIT
   #define STOP_SEC_VAR_NOINIT_8BIT

#elif defined (CAN_START_SEC_VAR_NOINIT_16BIT)
   #undef      CAN_START_SEC_VAR_NOINIT_16BIT
   #define START_SEC_VAR_NOINIT_16BIT
#elif defined (CAN_STOP_SEC_VAR_NOINIT_16BIT)
   #undef      CAN_STOP_SEC_VAR_NOINIT_16BIT
   #define STOP_SEC_VAR_NOINIT_16BIT

#elif defined (CAN_START_SEC_VAR_NOINIT_32BIT)
   #undef      CAN_START_SEC_VAR_NOINIT_32BIT
   #define START_SEC_VAR_NOINIT_32BIT
#elif defined (CAN_STOP_SEC_VAR_NOINIT_32BIT)
   #undef      CAN_STOP_SEC_VAR_NOINIT_32BIT
   #define STOP_SEC_VAR_NOINIT_32BIT

#elif defined (CAN_START_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_NOINIT_UNSPECIFIED
   #define START_SEC_VAR_NOINIT_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
   #define STOP_SEC_VAR_NOINIT_UNSPECIFIED

#elif defined (CAN_START_SEC_VAR_POWER_ON_INIT_BOOLEAN)
   #undef      CAN_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
   #define START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
   #define STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN

#elif defined (CAN_START_SEC_VAR_POWER_ON_INIT_8BIT)
   #undef      CAN_START_SEC_VAR_POWER_ON_INIT_8BIT
   #define START_SEC_VAR_POWER_ON_INIT_8BIT
#elif defined (CAN_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
   #undef      CAN_STOP_SEC_VAR_POWER_ON_INIT_8BIT
   #define STOP_SEC_VAR_POWER_ON_INIT_8BIT

#elif defined (CAN_START_SEC_VAR_POWER_ON_INIT_16BIT)
   #undef      CAN_START_SEC_VAR_POWER_ON_INIT_16BIT
   #define START_SEC_VAR_POWER_ON_INIT_16BIT
#elif defined (CAN_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
   #undef      CAN_STOP_SEC_VAR_POWER_ON_INIT_16BIT
   #define STOP_SEC_VAR_POWER_ON_INIT_16BIT

#elif defined (CAN_START_SEC_VAR_POWER_ON_INIT_32BIT)
   #undef      CAN_START_SEC_VAR_POWER_ON_INIT_32BIT
   #define START_SEC_VAR_POWER_ON_INIT_32BIT
#elif defined (CAN_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
   #undef      CAN_STOP_SEC_VAR_POWER_ON_INIT_32BIT
   #define STOP_SEC_VAR_POWER_ON_INIT_32BIT

#elif defined (CAN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #define START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #define STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED

#elif defined (CAN_START_SEC_VAR_BOOLEAN)
   #undef      CAN_START_SEC_VAR_BOOLEAN
   #define START_SEC_VAR_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_BOOLEAN
   #define STOP_SEC_VAR_BOOLEAN

#elif defined (CAN_START_SEC_VAR_8BIT)
   #undef      CAN_START_SEC_VAR_8BIT
   #define START_SEC_VAR_8BIT
#elif defined (CAN_STOP_SEC_VAR_8BIT)
   #undef      CAN_STOP_SEC_VAR_8BIT
   #define STOP_SEC_VAR_8BIT

#elif defined (CAN_START_SEC_VAR_16BIT)
   #undef      CAN_START_SEC_VAR_16BIT
   #define START_SEC_VAR_16BIT
#elif defined (CAN_STOP_SEC_VAR_16BIT)
   #undef      CAN_STOP_SEC_VAR_16BIT
   #define STOP_SEC_VAR_16BIT

#elif defined (CAN_START_SEC_VAR_32BIT)
   #undef      CAN_START_SEC_VAR_32BIT
   #define START_SEC_VAR_32BIT
#elif defined (CAN_STOP_SEC_VAR_32BIT)
   #undef      CAN_STOP_SEC_VAR_32BIT
   #define STOP_SEC_VAR_32BIT

#elif defined (CAN_START_SEC_VAR_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_UNSPECIFIED
   #define START_SEC_VAR_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_UNSPECIFIED
   #define STOP_SEC_VAR_UNSPECIFIED


#elif defined (CAN_START_SEC_VAR_FAST_NOINIT_BOOLEAN)
   #undef      CAN_START_SEC_VAR_FAST_NOINIT_BOOLEAN
   #define START_SEC_VAR_FAST_NOINIT_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_FAST_NOINIT_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_FAST_NOINIT_BOOLEAN
   #define STOP_SEC_VAR_FAST_NOINIT_BOOLEAN

#elif defined (CAN_START_SEC_VAR_FAST_NOINIT_8BIT)
   #undef      CAN_START_SEC_VAR_FAST_NOINIT_8BIT
   #define START_SEC_VAR_FAST_NOINIT_8BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_NOINIT_8BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_NOINIT_8BIT
   #define STOP_SEC_VAR_FAST_NOINIT_8BIT

#elif defined (CAN_START_SEC_VAR_FAST_NOINIT_16BIT)
   #undef      CAN_START_SEC_VAR_FAST_NOINIT_16BIT
   #define START_SEC_VAR_FAST_NOINIT_16BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_NOINIT_16BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_NOINIT_16BIT
   #define STOP_SEC_VAR_FAST_NOINIT_16BIT

#elif defined (CAN_START_SEC_VAR_FAST_NOINIT_32BIT)
   #undef      CAN_START_SEC_VAR_FAST_NOINIT_32BIT
   #define START_SEC_VAR_FAST_NOINIT_32BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_NOINIT_32BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_NOINIT_32BIT
   #define STOP_SEC_VAR_FAST_NOINIT_32BIT

#elif defined (CAN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
   #define START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
   #define STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED

#elif defined (CAN_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
   #undef      CAN_START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
   #define START_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN
   #define STOP_SEC_VAR_FAST_POWER_ON_INIT_BOOLEAN

#elif defined (CAN_START_SEC_VAR_FAST_POWER_ON_INIT_8BIT)
   #undef      CAN_START_SEC_VAR_FAST_POWER_ON_INIT_8BIT
   #define START_SEC_VAR_FAST_POWER_ON_INIT_8BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT
   #define STOP_SEC_VAR_FAST_POWER_ON_INIT_8BIT

#elif defined (CAN_START_SEC_VAR_FAST_POWER_ON_INIT_16BIT)
   #undef      CAN_START_SEC_VAR_FAST_POWER_ON_INIT_16BIT
   #define START_SEC_VAR_FAST_POWER_ON_INIT_16BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT
   #define STOP_SEC_VAR_FAST_POWER_ON_INIT_16BIT

#elif defined (CAN_START_SEC_VAR_FAST_POWER_ON_INIT_32BIT)
   #undef      CAN_START_SEC_VAR_FAST_POWER_ON_INIT_32BIT
   #define START_SEC_VAR_FAST_POWER_ON_INIT_32BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT
   #define STOP_SEC_VAR_FAST_POWER_ON_INIT_32BIT

#elif defined (CAN_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
   #define START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
   #define STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED

#elif defined (CAN_START_SEC_VAR_FAST_BOOLEAN)
   #undef      CAN_START_SEC_VAR_FAST_BOOLEAN
   #define START_SEC_VAR_FAST_BOOLEAN
#elif defined (CAN_STOP_SEC_VAR_FAST_BOOLEAN)
   #undef      CAN_STOP_SEC_VAR_FAST_BOOLEAN
   #define STOP_SEC_VAR_FAST_BOOLEAN

#elif defined (CAN_START_SEC_VAR_FAST_8BIT)
   #undef      CAN_START_SEC_VAR_FAST_8BIT
   #define START_SEC_VAR_FAST_8BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_8BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_8BIT
   #define STOP_SEC_VAR_FAST_8BIT

#elif defined (CAN_START_SEC_VAR_FAST_16BIT)
   #undef      CAN_START_SEC_VAR_FAST_16BIT
   #define START_SEC_VAR_FAST_16BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_16BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_16BIT
   #define STOP_SEC_VAR_FAST_16BIT

#elif defined (CAN_START_SEC_VAR_FAST_32BIT)
   #undef      CAN_START_SEC_VAR_FAST_32BIT
   #define START_SEC_VAR_FAST_32BIT
#elif defined (CAN_STOP_SEC_VAR_FAST_32BIT)
   #undef      CAN_STOP_SEC_VAR_FAST_32BIT
   #define STOP_SEC_VAR_FAST_32BIT

#elif defined (CAN_START_SEC_VAR_FAST_UNSPECIFIED)
   #undef      CAN_START_SEC_VAR_FAST_UNSPECIFIED
   #define START_SEC_VAR_FAST_UNSPECIFIED
#elif defined (CAN_STOP_SEC_VAR_FAST_UNSPECIFIED)
   #undef      CAN_STOP_SEC_VAR_FAST_UNSPECIFIED
   #define STOP_SEC_VAR_FAST_UNSPECIFIED

#elif defined (CAN_START_SEC_CONST_BOOLEAN)
   #undef      CAN_START_SEC_CONST_BOOLEAN
   #define START_SEC_CONST_BOOLEAN
#elif defined (CAN_STOP_SEC_CONST_BOOLEAN)
   #undef      CAN_STOP_SEC_CONST_BOOLEAN
   #define STOP_SEC_CONST_BOOLEAN

#elif defined (CAN_START_SEC_CONST_8BIT)
   #undef      CAN_START_SEC_CONST_8BIT
   #define START_SEC_CONST_8BIT
#elif defined (CAN_STOP_SEC_CONST_8BIT)
   #undef      CAN_STOP_SEC_CONST_8BIT
   #define STOP_SEC_CONST_8BIT

#elif defined (CAN_START_SEC_CONST_16BIT)
   #undef      CAN_START_SEC_CONST_16BIT
   #define START_SEC_CONST_16BIT
#elif defined (CAN_STOP_SEC_CONST_16BIT)
   #undef      CAN_STOP_SEC_CONST_16BIT
   #define STOP_SEC_CONST_16BIT

#elif defined (CAN_START_SEC_CONST_32BIT)
   #undef      CAN_START_SEC_CONST_32BIT
   #define START_SEC_CONST_32BIT
#elif defined (CAN_STOP_SEC_CONST_32BIT)
   #undef      CAN_STOP_SEC_CONST_32BIT
   #define STOP_SEC_CONST_32BIT

#elif defined (CAN_START_SEC_CONST_UNSPECIFIED)
   #undef      CAN_START_SEC_CONST_UNSPECIFIED
   #define START_SEC_CONST_UNSPECIFIED
#elif defined (CAN_STOP_SEC_CONST_UNSPECIFIED)
   #undef      CAN_STOP_SEC_CONST_UNSPECIFIED
   #define STOP_SEC_CONST_UNSPECIFIED

#elif defined (CAN_START_SEC_CONST_FAST_BOOLEAN)
   #undef      CAN_START_SEC_CONST_FAST_BOOLEAN
   #define START_SEC_CONST_FAST_BOOLEAN
#elif defined (CAN_STOP_SEC_CONST_FAST_BOOLEAN)
   #undef      CAN_STOP_SEC_CONST_FAST_BOOLEAN
   #define STOP_SEC_CONST_FAST_BOOLEAN

#elif defined (CAN_START_SEC_CONST_FAST_8BIT)
   #undef      CAN_START_SEC_CONST_FAST_8BIT
   #define START_SEC_CONST_FAST_8BIT
#elif defined (CAN_STOP_SEC_CONST_FAST_8BIT)
   #undef      CAN_STOP_SEC_CONST_FAST_8BIT
   #define STOP_SEC_CONST_FAST_8BIT

#elif defined (CAN_START_SEC_CONST_FAST_16BIT)
   #undef      CAN_START_SEC_CONST_FAST_16BIT
   #define START_SEC_CONST_FAST_16BIT
#elif defined (CAN_STOP_SEC_CONST_FAST_16BIT)
   #undef      CAN_STOP_SEC_CONST_FAST_16BIT
   #define STOP_SEC_CONST_FAST_16BIT

#elif defined (CAN_START_SEC_CONST_FAST_32BIT)
   #undef      CAN_START_SEC_CONST_FAST_32BIT
   #define START_SEC_CONST_FAST_32BIT
#elif defined (CAN_STOP_SEC_CONST_FAST_32BIT)
   #undef      CAN_STOP_SEC_CONST_FAST_32BIT
   #define STOP_SEC_CONST_FAST_32BIT

#elif defined (CAN_START_SEC_CONST_FAST_UNSPECIFIED)
   #undef      CAN_START_SEC_CONST_FAST_UNSPECIFIED
   #define START_SEC_CONST_FAST_UNSPECIFIED
#elif defined (CAN_STOP_SEC_CONST_FAST_UNSPECIFIED)
   #undef      CAN_STOP_SEC_CONST_FAST_UNSPECIFIED
   #define STOP_SEC_CONST_FAST_UNSPECIFIED

#elif defined (CAN_START_CONST_PBCFG)
   #undef      CAN_START_CONST_PBCFG
   #define START_CONST_PBCFG
#elif defined (CAN_STOP_CONST_PBCFG)
   #undef      CAN_STOP_CONST_PBCFG
   #define STOP_CONST_PBCFG

#elif defined (CAN_START_CONST_PBCFG_ROOT)
   #undef      CAN_START_CONST_PBCFG_ROOT
   #define START_CONST_PBCFG_ROOT
#elif defined (CAN_STOP_CONST_PBCFG_ROOT)
   #undef      CAN_STOP_CONST_PBCFG_ROOT
   #define STOP_CONST_PBCFG_ROOT

#endif /* START_WITH_IF */

#include "MemMap.h"


/*=======[E N D   O F   F I L E]==============================================*/
