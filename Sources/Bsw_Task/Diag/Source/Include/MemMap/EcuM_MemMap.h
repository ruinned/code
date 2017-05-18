/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file       <ECUM_MemMap.h>
 *  @brief      <Briefly describe file(one line)>
 *
 *  <Compiler: CodeWarriar    MCU:MC9S12 >
 *
 *  @author     <wbn>
 *  @date       <2014-09-16>
 */
/*============================================================================*/
/* @req MEMMAP020 @req MEMMAP001 @req MEMMAP002 @req MEMMAP005 @req MEMMAP015 */
/* @req MEMMAP016 */

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20140916  wbn         Initial version
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define ECUM_MEMMAP_VENDOR_ID  0
#define ECUM_MEMMAP_MODULE_ID  10
#define ECUM_MEMMAP_AR_MAJOR_VERSION  1
#define ECUM_MEMMAP_AR_MINOR_VERSION  3
#define ECUM_MEMMAP_AR_PATCH_VERSION  0
#define ECUM_MEMMAP_SW_MAJOR_VERSION  1
#define ECUM_MEMMAP_SW_MINOR_VERSION  0
#define ECUM_MEMMAP_SW_PATCH_VERSION  0
#define ECUM_MEMMAP_VENDOR_API_INFIX  0

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

/*=======[ECUM]==============================================================*/
#elif defined (ECUM_START_SEC_CODE)
   #undef      ECUM_START_SEC_CODE
   #define START_SEC_CODE
#elif defined (ECUM_STOP_SEC_CODE)
   #undef      ECUM_STOP_SEC_CODE
   #define STOP_SEC_CODE

#elif defined (ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      ECUM_START_SEC_VAR_NOINIT_UNSPECIFIED
   #define START_SEC_VAR_NOINIT_UNSPECIFIED
#elif defined (ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
   #undef      ECUM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
   #define STOP_SEC_VAR_NOINIT_UNSPECIFIED

#elif defined (ECUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      ECUM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #define START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#elif defined (ECUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
   #undef      ECUM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
   #define STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED

#elif defined (ECUM_START_SEC_CONST_UNSPECIFIED)
   #undef      ECUM_START_SEC_CONST_UNSPECIFIED
   #define START_SEC_CONST_UNSPECIFIED
#elif defined (ECUM_STOP_SEC_CONST_UNSPECIFIED)
   #undef      ECUM_STOP_SEC_CONST_UNSPECIFIED
   #define STOP_SEC_CONST_UNSPECIFIED

#endif /* START_WITH_IF */

#include "MemMap.h"


/*=======[E N D   O F   F I L E]==============================================*/
