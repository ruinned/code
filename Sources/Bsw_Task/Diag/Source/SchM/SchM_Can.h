/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  
 *  @file       <SchM_Can.h>
 *  @brief      <Briefly describe file(one line)>
 *  
 *  <Compiler: CodeWarrior V2.7    MCU:MPC5602D>
 *  
 *  @author     <bo.zeng> 
 *  @date       <15-07-2013>
 */
/*============================================================================*/
 #ifndef SCHM_CAN_H
 #define SCHM_CAN_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *  V1.0.0       20130715  bo.zeng     Initial version
 * 
 */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define SCHM_CAN_H_AR_MAJOR_VERSION  1
#define SCHM_CAN_H_AR_MINOR_VERSION  1
#define SCHM_CAN_H_AR_PATCH_VERSION  2

#define SCHM_CAN_H_SW_MAJOR_VERSION  3
#define SCHM_CAN_H_SW_MINOR_VERSION  1
#define SCHM_CAN_H_SW_PATCH_VERSION  0

/*=======[I N C L U D E S]====================================================*/

/*=======[M A C R O S]========================================================*/
#define INTERRUPT_PROTECTION_AREA    0U
#define EXCLUSIVE_AREA_1             1U

#define SchM_Enter_Can(Instance, Exclusive_Area)    do{}while(0)
#define SchM_Exit_Can(Instance, Exclusive_Area)     do{}while(0)

#endif /* #define SCHM_CAN_H */    

/*=======[E N D   O F   F I L E]==============================================*/

