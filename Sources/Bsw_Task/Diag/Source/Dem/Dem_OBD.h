/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_OBD.h>
 *  @brief      <Dem_OBD>
 *  
 *  <Compiler: All>
 *  
 *  @author     <wbn>
 *  @date       <Dec 26, 2014 - 10:23:31 AM>
 */
/*============================================================================*/

#ifndef DEM_OBD_H_
#define DEM_OBD_H_

/********[R E V I S I O N   H I S T O R Y]*************************************/
/*  <VERSION>    <DATE>    	<AUTHOR>    	<REVISION LOG>
 *  V1.0.0       Dec 26, 2014    wbn    		Initial version
 * 
 */
/******************************************************************************/

/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_OBD_H_MODULE_ID            54U
#define DEM_OBD_H_VENDOR_ID          0U
#define DEM_OBD_H_AR_MAJOR_VERSION   0U
#define DEM_OBD_H_AR_MINOR_VERSION   0U
#define DEM_OBD_H_AR_PATCH_VERSION   0U
#define DEM_OBD_H_SW_MAJOR_VERSION   1U
#define DEM_OBD_H_SW_MINOR_VERSION   0U
#define DEM_OBD_H_SW_PATCH_VERSION   0U

/********[I N C L U D E S]*****************************************************/
#include "Dem_Types.h"

/********[M A C R O S]*********************************************************/
#if(DEM_OBD_ENABLE == STD_ON)
extern Std_ReturnType Dem_SetEventDisabled(
    Dem_EventIdType EventId
    );

extern Std_ReturnType Dem_RepIUMPRFaultDetect(
    Dem_RatioIdType RatioID
    );

extern Std_ReturnType Dem_SetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    Dem_IumprDenomCondStatusType ConditionStatus
    );   

extern Std_ReturnType Dem_GetIUMPRDenCondition(
    Dem_IumprDenomCondIdType ConditionId,
    Dem_IumprDenomCondStatusType* ConditionStatus
    );

extern Std_ReturnType Dem_RepIUMPRDenLock(
    Dem_RatioIdType RatioID
    );    

extern Std_ReturnType Dem_RepIUMPRDenRelease(
    Dem_RatioIdType RatioID
    );

extern Std_ReturnType Dem_SetPtoStatus(
    boolean PtoStatus
    );

extern Std_ReturnType Dem_ReadDataOfPID01(
    uint8* PID01value
    );    

extern Std_ReturnType Dem_GetDataOfPID21(
    uint8* PID21value
    );    

extern Std_ReturnType Dem_SetDataOfPID21(
    uint8* PID21value
    );    

extern Std_ReturnType Dem_SetDataOfPID31(
    uint8* PID31value
    );

extern Std_ReturnType Dem_SetDataOfPID4D(
    uint8* PID4Dvalue
    );    

extern Std_ReturnType Dem_SetDataOfPID4E(
    uint8* PID4Evalue
    );

extern Std_ReturnType Dem_SetPfcCycleQualified(void);    

extern Std_ReturnType Dem_GetPfcCycleQualified(boolean* isqualified);

extern Std_ReturnType Dem_SetClearDTC(
    uint32 DTC,
    Dem_DTCFormatType DTCFormat,
    Dem_DTCOriginType DTCOrigin
    );

extern Std_ReturnType Dem_DcmGetDTCSeverityAvailabilityMask(
    Dem_DTCSeverityType* DTCSeverityMask
    );  

extern Std_ReturnType Dem_GetB1Counter(
    uint16* B1Counter
    );  

extern Std_ReturnType Dem_SetDTR(
    uint16 DTRId,
    sint32 TestResult,
    sint32 LowerLimit,
    sint32 UpperLimit,
    Dem_DTRControlType Ctrlval
    );    
#endif
/********[E X T E R N A L   D A T A]*******************************************/

/********[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]*********/

#endif /* DEM_OBD_H_ */

/********[E N D   O F   F I L E]***********************************************/
