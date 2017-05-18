/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Dcm.h>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-16 09:31:00>
 */
/*============================================================================*/


#ifndef RTE_DCM_H
#define RTE_DCM_H

#include "Dcm_Types.h"
#include "Std_ExtendedTypes.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/********************* callback function for ECU Reset********************/
extern Std_ReturnType Rte_EcuReset(uint8 ResetType, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

/**********call back functions called by DCM when a service request received******/
extern Std_ReturnType Rte_Indication1(uint8 SID, uint8 *UDS_FAR RequestData, uint16 DataSize);

/*******function for Protocol Start and stop***********/
extern Std_ReturnType  StartProtocol(Dcm_ProtocolType  ProtocolID);
extern Std_ReturnType  StopProtocol(Dcm_ProtocolType  ProtocolID);

/*************functions for session control service*****************/
extern Std_ReturnType GetSesChgPermission(Dcm_SesType SesCtrlTypeActive, Dcm_SesType SesCtrlTypeNew);

extern Std_ReturnType ChangeIndication(Dcm_SesType SesCtrlTypeActive, Dcm_SesType SesCtrlTypeNew);

/*************callback functions for DID services*************/

extern Std_ReturnType Rte_DidReadData_f10d(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f18c(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f199(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f187(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f180(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f189(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f17f(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f184(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_0201(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_0200(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_0110(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_f186(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_1000(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_1400(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_1404(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_1409(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_0505(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_141e(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_141f(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_3011(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_3012(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2005(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2006(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2007(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2008(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2020(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2021(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2022(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2023(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_1001(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2024(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2025(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2026(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2027(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2000(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2009(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2010(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2011(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2012(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2013(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2014(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2015(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2016(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2017(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2018(uint8*UDS_FAR data);
extern Std_ReturnType Rte_DidReadData_2019(uint8*UDS_FAR data);


extern Std_ReturnType Rte_DidWriteData_f199(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode);
extern Std_ReturnType Rte_DidWriteData_f184(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode);
extern Std_ReturnType Rte_DidWriteData_0110(uint8*UDS_FAR data, uint16 dataLength, Dcm_NegativeResponseCodeType*UDS_FAR ErrorCode);

extern Std_ReturnType Rte_DidReturnControlToEcu_d001(uint8 *UDS_FAR ControlOptionRecord,  uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode);

extern Std_ReturnType Rte_DidShortTermAdjustment_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode);

extern Std_ReturnType Rte_DidFreezeCurrentState_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode);

extern Std_ReturnType Rte_ResetToDefault_d001(uint8 *UDS_FAR ControlOptionRecord, uint8 *UDS_FAR ControlEnableMaskRecord, uint8 *UDS_FAR ControlStatusRecord, Dcm_NegativeResponseCodeType  *UDS_FAR ErrorCode);



/******callback functions for Routine control**********/
extern Std_ReturnType RequestResults_060E(uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

extern Std_ReturnType StartRoutine_060E(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

extern Std_ReturnType StartRoutine_0203(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

extern Std_ReturnType StopRoutine_060E(uint8*UDS_FAR InBuffer, uint8*UDS_FAR OutBuffer, Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

/*******function for security access***********/
extern Std_ReturnType  Rte_GetSeed1(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);
extern Std_ReturnType  Rte_GetSeed2(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);
extern Std_ReturnType  Rte_GetSeed9(uint8 *UDS_FAR SecurityAccessRecord,uint8 *UDS_FAR Seed,Dcm_NegativeResponseCodeType *UDS_FAR ErrorCode);

extern Std_ReturnType  Rte_CompareKey1(uint8 *UDS_FAR key);
extern Std_ReturnType  Rte_CompareKey2(uint8 *UDS_FAR key);
extern Std_ReturnType  Rte_CompareKey9(uint8 *UDS_FAR key);

extern Std_ReturnType Get_FactoryMode(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* RTE_DCM_H */
