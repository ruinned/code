/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dem.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-02-25 10:48:10>
 */
/*============================================================================*/


#ifndef RTE_DEM_H
#define RTE_DEM_H

#include "Dem_Types.h"
#include "Std_ExtendedTypes.h"

/*********************************/
extern Std_ReturnType Rte_DidReadData_2101(uint8*UDS_FAR Buffer, uint16 BufSize);
extern Std_ReturnType Rte_DidReadExtendData_01(uint8*UDS_FAR Buffer, uint16 BufSize);
extern void Rte_EnableAllDtcsRecord(void);
extern Std_ReturnType  Rte_CommunicaitonControl(uint8  controlType,uint8 communicationType);

/*********************************/



#endif /* RTE_DEM_H */
