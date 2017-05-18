/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <EcuM_Types.h>
 *  @brief      <EcuM_Types>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Sep 17, 2014 - 9:43:13 AM>
 */
/*============================================================================*/

#ifndef ECUM_TYPES_H_ 
#define ECUM_TYPES_H_

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Sep 17, 2014    wbn    		Initial version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define ECUM_TYPES_H_MODULE_ID            0U
#define ECUM_TYPES_H_VENDOR_ID          0U
#define ECUM_TYPES_H_AR_MAJOR_VERSION   0U
#define ECUM_TYPES_H_AR_MINOR_VERSION   0U
#define ECUM_TYPES_H_AR_PATCH_VERSION   0U
#define ECUM_TYPES_H_SW_MAJOR_VERSION   1U
#define ECUM_TYPES_H_SW_MINOR_VERSION   0U
#define ECUM_TYPES_H_SW_PATCH_VERSION   0U

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "EcuM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/
typedef uint32 EcuM_WakeupSourceType;

typedef struct
{
    EcuM_WakeupSourceType Id;
    NetworkHandleType ComMChannelIdRef;
}EcuM_WksCfgInfoType;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* ECUM_TYPES_H_ */

/*=======[E N D   O F   F I L E]==============================================*/
