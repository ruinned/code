/*============================================================================*/
/*  Copyright (C) 2009-2013, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <SchM_CanIf.h>
 *  @brief      <SchM>
 *  
 *  <Compiler: CodeWarrior2.8 MCU:MPC5634>
 *  
 *  @author     <Tommy>
 *  @date       <2013-05-14>
 */
/*============================================================================*/

#ifndef _SCHM_CANIF_H_ 
#define _SCHM_CANIF_H_

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130514   Tommy      Initial version
 */
/*============================================================================*/



/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define SCHM_CANIF_H_VENDOR_ID          60U
#define SCHM_CANIF_H_AR_MAJOR_VERSION   1U
#define SCHM_CANIF_H_AR_MINOR_VERSION   1U
#define SCHM_CANIF_H_AR_PATCH_VERSION   2U
#define SCHM_CANIF_H_SW_MAJOR_VERSION   1U
#define SCHM_CANIF_H_SW_MINOR_VERSION   0U
#define SCHM_CANIF_H_SW_PATCH_VERSION   0U

/*=======[M A C R O S]========================================================*/
#define CANIF_AREA_RXBUFFER     (0)
#define CANIF_AREA_TXBUFFER     (1)
#define CANIF_AREA_CAN          (2)

#define SchM_Enter_CanIf(CANIF_INSTANCE_ID, areaid)
#define SchM_Exit_CanIf(CANIF_INSTANCE_ID, areaid)


#endif 


