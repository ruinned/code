/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/
#ifndef MCU_H_
#define MCU_H_

#include "CanSM.h"
#include "Can.h"
#include "ComM.h"
#include "EcuM.h"
#include "CanIfDiag.h" 
#include "Can_Irq.h"
#include "Dem.h"
#include "Dcm.h"
#include "CanTpDiag.h"
#include "CanSM_SchM.h"
#include "Dem_EventMemory.h"
#include "IO_Map.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*=======[M A C R O S]========================================================*/

/****************************** declarations *********************************/
extern  void Mcu_Init(void);
extern  void Mcu_PerformReset(void);
extern void Diag_Init();
extern void Diag_Sch();
extern void Diag_CountCbk(void);
/****************************** definitions *********************************/

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* MCU_DCM_H_ */
