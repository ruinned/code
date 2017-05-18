/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm_Dependence.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCM_DEPENDENCE_H
#define DCM_DEPENDENCE_H

/*=======[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>                         */
/*  V1.0.0       2013-3-20  chenms      Initial version                       */
/*============================================================================*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define DCM_DEPENDENCE_H_VENDOR_ID  0
#define DCM_DEPENDENCE_H_MODULE_ID  0
#define DCM_DEPENDENCE_H_AR_MAJOR_VERSION  3
#define DCM_DEPENDENCE_H_AR_MINOR_VERSION  3
#define DCM_DEPENDENCE_H_AR_PATCH_VERSION  0
#define DCM_DEPENDENCE_H_SW_MAJOR_VERSION  1
#define DCM_DEPENDENCE_H_SW_MINOR_VERSION  0
#define DCM_DEPENDENCE_H_SW_PATCH_VERSION  0

/****************************** references *********************************/
#include  "Os.h"
#include  "ComM_Dcm.h"
#if (DCM_DEV_ERROR_DETECT == STD_ON)
#include  "Det.h"
#endif
#include  "SchM_Dcm.h"
#include  "PduR_Dcm.h"
#include  "Rte_Dcm.h"
#if (DCM_DSP_ECU_RESET_FUNC_ENABLED == STD_ON)
#include  "Mcu.h"
#endif

/*============================================================================*/
#endif /* DCM_DEPENDENCE_H_ */
