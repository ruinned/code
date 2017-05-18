/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dem_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:41:01> 
 */
/*============================================================================*/


/********[V E R S I O N  I N F O R M A T I O N]********************************/
#define DEM_CFG_C_AR_MAJOR_VERSION   3U
#define DEM_CFG_C_AR_MINOR_VERSION   1U
#define DEM_CFG_C_AR_PATCH_VERSION   0U
#define DEM_CFG_C_SW_MAJOR_VERSION   1U
#define DEM_CFG_C_SW_MINOR_VERSION   0U
#define DEM_CFG_C_SW_PATCH_VERSION   0U


/********[I N C L U D E S]*****************************************************/
#include "Dem_CfgTypes.h"
#include "Rte_Dem.h"
#include "Std_ExtendedTypes.h"


/********[V E R S I O N  C H E C K]********************************************/
#if (DEM_CFG_C_AR_MAJOR_VERSION != DEM_CFG_H_AR_MAJOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_AR_MINOR_VERSION != DEM_CFG_H_AR_MINOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_AR_PATCH_VERSION != DEM_CFG_H_AR_PATCH_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_SW_MAJOR_VERSION != DEM_CFG_H_SW_MAJOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_SW_MINOR_VERSION != DEM_CFG_H_SW_MINOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif

#if (DEM_CFG_C_AR_MAJOR_VERSION != DEM_CFGTYPES_H_AR_MAJOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_AR_MINOR_VERSION != DEM_CFGTYPES_H_AR_MINOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_AR_PATCH_VERSION != DEM_CFGTYPES_H_AR_PATCH_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_SW_MAJOR_VERSION != DEM_CFGTYPES_H_SW_MAJOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif
#if (DEM_CFG_C_SW_MINOR_VERSION != DEM_CFGTYPES_H_SW_MINOR_VERSION)
  #error "Dem_Cfg.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************
*                          General Configuration
*******************************************************************************/


/*******************************************************************************
*                          Condition Configuration
*******************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemOperationCycle */
CONST(Dem_OperationCycleType,AUTOMATIC) DemOperationCycle[DEM_OPERATION_CYCLE_NUM] =
{
   { /* DemOperationCycle */
        FALSE,  /* 老化 */
        FALSE,
        DEM_OPCYC_IGNITION
    },
       { /* DemOperationCycle */
        FALSE,  /* 老化 */
        FALSE,
        DEM_OPCYC_OBD_DCY
    },
       { /* DemOperationCycle */
        FALSE,  /* 老化 */
        FALSE,
        DEM_OPCYC_OTHER
    },
    { /* DemOperationCycle */
        TRUE, /* 老化的时候一定是FALSE */
        TRUE,
        DEM_OPCYC_POWER
    }
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"





/*******************************************************************************
*                          FreezeFrame Configuration
*******************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
/* DemDidClass */
CONST(Dem_DidClassType,AUTOMATIC) DemDidClass[DEM_DID_CLASS_NUM] =
{
    { /* DemDidClass_1 */
        0x2101, 
        20,
        Rte_DidReadData_2101
    }
}; 
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h" 
CONST(uint16,AUTOMATIC) DemDidClassRef[DEM_DID_CLASS_REF_TOTAL_NUM] =
{
    /* DemFreezeFrameClass_1 */
    0x0,
};    
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
CONST(Dem_FreezeFrameClassType,AUTOMATIC) DemFreezeFrameClass[DEM_FREEZE_FRAME_CLASS_NUM] =
{
    { /* DemFreezeFrameClass_1 */
        20,  
        0,
        1
    }
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemFreezeFrameRecordClass */
CONST(Dem_FreezeFrameRecordClassType,AUTOMATIC) DemFreezeFrameRecordClass[DEM_FREEZE_FRAME_RECORD_CLASS_NUM] = 
{
    { /* DemFreezeFrameRecordClass_1 */
        1,                       /* DemFreezeFrameRecordNumber */
        DEM_TRIGGER_ON_TEST_FAILED,   /* DemFreezeFrameRecordTrigger */
        DEM_UPDATE_RECORD_NO       /* DemFreezeFrameRecordUpdate */
    },
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
CONST(uint8,AUTOMATIC) DemFreezeFrameRecordClassRef[DEM_FREEZE_FRAME_RECORD_CLASS_REF_TOTAL_NUM] =
{
    /* DemFreezeFrameRecNumClass_1 */
    0x0,
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"   
CONST(Dem_FreezeFrameRecNumClassType,AUTOMATIC) DemFreezeFrameRecNumClass[DEM_FREEZE_FRAME_REC_NUM_CLASS_NUM] =
{
    { /* DemFreezeFrameRecNumClass_1 */
        0,
        1
    }
};    
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/*******************************************************************************
*                          ExtendedData Configuration
*******************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
/* DemExtendedDataRecordClass */
CONST(Dem_ExtendedDataRecordClassType,AUTOMATIC) DemExtendedDataRecordClass[DEM_EXTENDED_DATA_RECORD_CLASS_NUM] =
{
    { /* DemExtendedDataRecordClass_1 */
        0x1,
        DEM_TRIGGER_ON_TEST_FAILED,
        DEM_UPDATE_RECORD_NO,
        2,
        Rte_DidReadExtendData_01
    }
}; 
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"   
CONST(uint8,AUTOMATIC) DemExtendedDataRecordClassRef[DEM_EXTENDED_DATA_RECORD_CLASS_REF_TOTAL_NUM] =
{
    /* DemExtendedDataClass_1 */
    0x0,
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"  
    
/* DemGeneral/DemExtendedDataClass */
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
CONST(Dem_ExtendedDataClassType,AUTOMATIC) DemExtendedDataClass[DEM_EXTENDED_DATA_CLASS_NUM] =
{
    { /* DemExtendedDataClass_1 */
        2,
        0,
        1
    }
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
/*******************************************************************************
*                          DTC Configuration
*******************************************************************************/

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
/* DemDTC 0-65535 */
CONST(Dem_DTCType,AUTOMATIC) DemDTC[DEM_DTC_NUM] =
{
    { /* P0A7D00 */
        0xA7D00, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16C119 */
        0x16C119, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16C11D */
        0x16C11D, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16C219 */
        0x16C219, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16C21D */
        0x16C21D, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB17 */
        0x16FB17, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB85 */
        0x16FB85, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB24 */
        0x16FB24, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB16 */
        0x16FB16, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB84 */
        0x16FB84, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FB00 */
        0x16FB00, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FC17 */
        0x16FC17, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FC85 */
        0x16FC85, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FC16 */
        0x16FC16, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FC84 */
        0x16FC84, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167E98 */
        0x167E98, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167EA4 */
        0x167EA4, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167EA5 */
        0x167EA5, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167EAF */
        0x167EAF, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16FCAF */
        0x16FCAF, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16F91C */
        0x16F91C, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P169F04 */
        0x169F04, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16C304 */
        0x16C304, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16BA04 */
        0x16BA04, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16BB04 */
        0x16BB04, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* U10C000 */
        0xD0C000, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166496 */
        0x166496, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16A22B */
        0x16A22B, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16A52B */
        0x16A52B, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* U10C087 */
        0xD0C087, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16A692 */
        0x16A692, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P160B13 */
        0x160B13, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166700 */
        0x166700, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P146E96 */
        0x146E96, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P147319 */
        0x147319, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16A72B */
        0x16A72B, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P16A82B */
        0x16A82B, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* U10C287 */
        0xD0C287, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166800 */
        0x166800, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166900 */
        0x166900, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166A00 */
        0x166A00, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166B00 */
        0x166B00, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167000 */
        0x167000, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167100 */
        0x167100, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P167200 */
        0x167200, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P146696 */
        0x146696, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P166C00 */
        0x166C00, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168300 */
        0x168300, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168400 */
        0x168400, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* U007388 */
        0xC07388, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* U007588 */
        0xC07588, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168500 */
        0x168500, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168600 */
        0x168600, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168700 */
        0x168700, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
    { /* P168900 */
        0x168900, /* DemDtcValue  */
        0, /* DemDTCAttributesRef  */
       	0x5, /* DemDTCFunctionalUnit  */
        DEM_SEVERITY_NO_SEVERITY, /* DemDTCSeverity */
    },
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
/* DemDTCAttributes */
CONST(Dem_DTCAttributesType,AUTOMATIC) DemDTCAttributes[DEM_DTC_ATTRIBUTES_NUM] =
{
    { /* DemDTCAttributes_1 */
        TRUE, /* DemAgingAllowed */
        0, /* DemAgingCycleRef */
        2, /* DemAgingCycleCounterThreshold 老化的次数*/
        0, /* DemAgingCycleCounterThresholdForTFSLC */
        5, /* DemDTCPriority */
        5, /* DemEventMemoryEntryFdcThresholdStorageValue */
        0, /* DemFreezeFrameRecNumClassRef */
        DEM_EVENT_SIGNIFICANCE_OCCURRENCE, /* DemDTCSignificance */
        0, /* DemExtendedDataClassRef  */
        0, /* DemFreezeFrameClassRef  */
		{ 0, } /* DemMemoryDestinationRef  */
    },    
    { /* DemDTCAttributes_1 */
        FALSE, /* DemAgingAllowed */
        DEM_OPERATION_CYCLE_INVALID, /* DemAgingCycleRef */
        0, /* DemAgingCycleCounterThreshold */
        0, /* DemAgingCycleCounterThresholdForTFSLC */
        5, /* DemDTCPriority */
        5, /* DemEventMemoryEntryFdcThresholdStorageValue */
        0, /* DemFreezeFrameRecNumClassRef */
        DEM_EVENT_SIGNIFICANCE_OCCURRENCE, /* DemDTCSignificance */
        0, /* DemExtendedDataClassRef  */
        0, /* DemFreezeFrameClassRef  */
		{ 0, } /* DemMemoryDestinationRef  */
    },

};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


/*******************************************************************************
*                          Indicator Configuration
*******************************************************************************/

/*******************************************************************************
*                          Debounce Configuration
*******************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
CONST(Dem_DebounceCounterBasedClassType,AUTOMATIC) DemDebounceCounterBasedClass[DEM_DEBOUNCE_COUNTER_BASED_CLASS_NUM] =
{
    
    { /* DemDebounceCounterBasedClass_1 */
        5, /* DemDebounceCounterDecrementStepSize */
        5, /* DemDebounceCounterIncrementStepSize  */
        0, /* DemDebounceCounterJumpDownValue  */
        0, /* DemDebounceCounterJumpUpValue */
        5, /* DemDebounceCounterFailedThreshold */
        -5, /* DemDebounceCounterPassedThreshold */
        FALSE, /* DemDebounceCounterJumpDown */
        FALSE, /* DemDebounceCounterJumpUp */
        FALSE, /* DemDebounceCounterStorage */
        DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
    },
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
/* DemConfigSet/DemDebounceTimeBaseClass 0-65535 */
CONST(Dem_DebounceTimeBaseClassType,AUTOMATIC) DemDebounceTimeBaseClass[DEM_DEBOUNCE_TIME_BASE_CLASS_NUM] =
{
    { /* DemDebounceTimeBaseClass_1 */
        5, /* DemDebounceTimeFailedThreshold */
        5, /* DemDebounceTimePassedThreshold */
        DEM_DEBOUNCE_FREEZE, /* DemDebounceBehavior */
    },
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"


/*******************************************************************************
*                          Event Configuration
*******************************************************************************/


#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"
CONST(Dem_EventParameterType,AUTOMATIC) DemEventParameter[DEM_EVENT_PARAMETER_NUM] = 
{
    {  /* soc_too_low_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        0, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        0, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* charge_current_too_large_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        1, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        1, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* charge_current_too_large_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        2, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        2, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* discharge_current_too_large_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        3, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        3, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* discharge_current_too_large_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        4, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        4, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        0,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_high_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        5, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        5, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        0,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_high_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        6, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        6, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_high_level_3*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        7, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        7, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_low_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        8, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        8, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_low_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        9, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        9, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_too_low_level_3*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        10, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        10, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* pack_voltage_too_high_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        11, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        11, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* pack_voltage_too_high_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        12, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        12, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* pack_voltage_too_low_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        13, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        13, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* pack_voltage_too_low_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        14, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        14, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* battery_temperature_too_high_level_1*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        15, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        15, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* battery_temperature_too_high_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        16, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        16, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* battery_temperature_too_high_level_3*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        17, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        17, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* battery_tempreture_unbalance_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        19, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        18, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* battery_cell_voltage_unbalance_level_2*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        19, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        19, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* cell_voltage_detection_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        20, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        20, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* tempreture_sensor_fault*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        21, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        21, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* current_sensor_fault*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        22, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        22, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* internal_sumvoltage_sensor_fault*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        23, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        23, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* outer__sumvoltage_sensor_fault*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        24, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        24, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_lost_internal_communication_for_1Second*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        25, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        25, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_initialization_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        26, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        26, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* positive_relay_stuck_or_cannot_close*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        27, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        27, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* negtive_relay_stuck_or_cannot_close*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        28, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        28, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_lost_car_communication_for_1Second*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        29, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        29, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* isulation_resistance_less_than_100_Ohm_V*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        30, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        30, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HVIL_open*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        31, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        31, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* precharge_fail*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        32, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        32, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_reports_charger_charge_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        33, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        33, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* Charger_charge_current_is_too_large*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        34, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        34, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* precharge_relay_stuck_or_cannot_close*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        35, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        35, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* charge_relay_stuck_or_cannot_close*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        36, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        36, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* lost_communication_with_charger_for_1_Sec*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        37, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        37, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HCU_key_signal_match_fail_with_hardware_for_3__Sec*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        38, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        38, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* emergency_line_status_mismatch_with_HVIL_status*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        39, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        39, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HV_system_isolation_fault_inside_of_BMS*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        40, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        40, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HV_system_isolation_fault_outside_of_BMS*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        41, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        41, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_12V_battery_supply_voltage_is_abnormal*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        42, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        42, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_cannot_receive_HCU_relay_open_request_after_key_off*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        43, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        43, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HV_circuit_failure*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        44, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        44, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* Crash_detected_from_SRS*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        45, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        45, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_main_fuse_open*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        46, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        46, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* Liquid_level_sensor_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        47, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        47, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* Battery_leakage_occurs*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        48, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        48, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* internal_CAN_bus_off_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        49, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        49, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* Car_communication_CAN_bus_off_error*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        50, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        50, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HVH_Fuse__failure*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        51, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        51, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* AC_Charge_fuse__failure*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        52, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        52, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* HVH_Relay__failure*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        53, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        53, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* BMS_balance_system_failure*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        54, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        54, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_SWC, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_AFTER_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* CANTP_E_COM*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        55, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* CAN_E_TIMEOUT*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0,/* AlgorithmRef = DemDebounceTimeBaseRef : Index Of DemDebounceTimeBaseClass    */
        0, /* AlgorithmIndex = AlgorithmIndex_Time++ */
        DEM_DEBOUNCE_TIME_BASE, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* CANIF_E_INVALID_DLC*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        56, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* CANIF_E_STOPPED*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        57, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* CANIF_E_FULL_TX_BUFFER*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        58, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
    {  /* COMM_E_LOWER_LIMIT*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        59, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    },
	{  /* CANSM_DEM_ERRID_0*/
        NULL_PTR,  /* DemCallbackInitMForE */
        NULL_PTR,  /* DemCallbackEventDataChanged */
        DEM_DTC_INVALID, /* DemDTCRef */
        0, /* AlgorithmRef = DemDebounceCounterBasedClassRef: Index Of DemDebounceCounterBasedClass   */
        60, /* AlgorithmIndex = AlgorithmIndex_Counter++ */
        DEM_DEBOUNCE_COUNTER_BASED, /* AlgorithmType */
        1,  /* DemEventFailureCycleCounterThreshold Range:0~255 */
        TRUE,  /* DemEventAvailable Range: true or false */
        FALSE,  /* DemFFPrestorageSupported Range: true or false */
        DEM_EVENT_KIND_BSW, /* DemEventKind = DEM_EVENT_KIND_BSW or DEM_EVENT_KIND_SWC */
        REPORT_BEFORE_INIT, /* DemReportBehavior = REPORT_AFTER_INIT or REPORT_AFTER_INIT */
        0, /* DemOperationCycleRef Reference: DemOperationCycle MULTI:1-1*/
        DEM_ENABLE_CONDITION_GROUP_INVALID, /* DemEnableConditionGroupRef: Index Of DemEnableConditionGroup */
        DEM_STORAGE_CONDITION_GROUP_INVALID, /* DemStorageConditionGroupRef: Index Of DemStorageConditionGroup */
    }
};
    
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"

/*******************************************************************************
*                          Memory Configuration
*******************************************************************************/

#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/* Non-volatile Memory */
VAR(Dem_NonVolatileMemInfoType,AUTOMATIC) DemNonVolatileMemInfo;
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"


#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemPrimaryMemory 1-1 */
VAR(Dem_EventMemEntryType,AUTOMATIC) DemPrimaryMemory[DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"

#if(DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR > 0)
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemMirrorMemory 0-1 */
VAR(Dem_EventMemEntryType,AUTOMATIC) DemMirrorMemory[DEM_MAX_NUMBER_EVENT_ENTRY_MIRROR];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

#if(DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT > 0)
#define DEM_START_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
/* DemGeneral/DemMaxNumberEventEntryPermanent [0-255]*/
VAR(Dem_EventMemEntryType,AUTOMATIC) DemPermanentMemory[DEM_MAX_NUMBER_EVENT_ENTRY_PERMANENT];
#define DEM_STOP_SEC_VAR_UNSPECIFIED
#include "Dem_MemMap.h"
#endif

/* Dem_EventMemEntryType DemUserDefinedMemory<Mem/Name>[Mem/DemMaxNumberEventEntryUserDefined]; */
#define DEM_START_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h"    
CONST(Dem_MemDestConfigType,AUTOMATIC) DemMemDestCfg[DEM_MEM_DEST_TOTAL_NUM] =  
{
    /* Primary Memory */
    { /* DemPrimaryMemory */
        DemPrimaryMemory,
        DEM_MAX_NUMBER_EVENT_ENTRY_PRIMARY,
        DEM_DTC_ORIGIN_PRIMARY_MEMORY,
    },
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "Dem_MemMap.h" 

#pragma DATA_SEG DEFAULT