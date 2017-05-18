/*============================================================================*/
/*  Copyright (C) 2009-2014, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Dcm_Cfg.c>
 *  @brief      <>
 *  
 *  <MCU:MC9S12_XEP100>
 *  
 *  @author     <>
 *  @date       <2017-03-22 19:40:59> 
 */
/*============================================================================*/


/******************************* references ************************************/
#include "Dcm_Types.h"
#include "Dcm_CfgType.h"
#include "Rte_Dcm.h"
#include "Dcm_Cfg.h"
#include "UDS.h"
#include "Std_ExtendedTypes.h"

#define   DCM_START_SEC_CONST_UNSPECIFIED
#include  "MemMap.h"
/**********************************************************************
 ***********************DcmGeneral Container***************************
 **********************************************************************/
CONST(Dcm_GeneralCfgType,DCM_CONST)Dcm_GeneralCfg =
{
    DCM_DEV_ERROR_DETECT,
    DCM_REQUEST_INDICATION_ENABLED,
    DCM_RESPOND_ALL_REQUEST,
    DCM_VERSION_INFO_API,
    10u
};


/**********************************************************************
 ***********************DSP Container**********************************
 *********************************************************************/
/************************************************
 ****DcmDspSecurityRow container(Multiplicity=0..31)****
 ************************************************/
STATIC  CONST(Dcm_DspSecurityRowType,DCM_CONST)Dcm_DspSecurityRow[4] =
{
    { /* SecurityLevel_1 */
        1u,      /*DcmDspSecurityLevel*/
        4u,      /*DcmDspSecuritySeedSize*/
        4u,      /*DcmDspSecurityKeySize*/
        0u,      /*DcmDspSecurityADRSize*/
        3u,      /*DcmDspSecurityNumAttDelay*/
        0u,      /*DcmDspSecurityNumAttLock*/
        10000u,  /*DcmDspSecurityDelayTime,10s */
        0u,   	  /*DcmDspSecurityDelayTimeOnBoot*/
        {Rte_CompareKey1,Rte_GetSeed1}
    },
    { /* SecurityLevel_0 */
        0u,      /*DcmDspSecurityLevel*/
        4u,      /*DcmDspSecuritySeedSize*/
        4u,      /*DcmDspSecurityKeySize*/
        0u,      /*DcmDspSecurityADRSize*/
        3u,      /*DcmDspSecurityNumAttDelay*/
        0u,      /*DcmDspSecurityNumAttLock*/
        10000u,  /*DcmDspSecurityDelayTime,10s */
        0u,   	  /*DcmDspSecurityDelayTimeOnBoot*/
        {NULL_PTR,NULL_PTR}
    },
    { /* SecurityLevel_2 */
        2u,      /*DcmDspSecurityLevel*/
        4u,      /*DcmDspSecuritySeedSize*/
        4u,      /*DcmDspSecurityKeySize*/
        0u,      /*DcmDspSecurityADRSize*/
        3u,      /*DcmDspSecurityNumAttDelay*/
        0u,      /*DcmDspSecurityNumAttLock*/
        10000u,  /*DcmDspSecurityDelayTime,10s */
        0u,   	  /*DcmDspSecurityDelayTimeOnBoot*/
        {Rte_CompareKey2,Rte_GetSeed2}
    },
    { /* SecurityLevel_9 */
        9u,      /*DcmDspSecurityLevel*/
        4u,      /*DcmDspSecuritySeedSize*/
        4u,      /*DcmDspSecurityKeySize*/
        0u,      /*DcmDspSecurityADRSize*/
        3u,      /*DcmDspSecurityNumAttDelay*/
        0u,      /*DcmDspSecurityNumAttLock*/
        10000u,  /*DcmDspSecurityDelayTime,10s */
        0u,   	  /*DcmDspSecurityDelayTimeOnBoot*/
        {Rte_CompareKey9,Rte_GetSeed9}
    }
};

/************************************************
 ****DcmDspSecurity container(Multiplicity=1)****
 ************************************************/
STATIC  CONST(Dcm_DspSecurityType,DCM_CONST)Dcm_DspSecurity =
{
    &Dcm_DspSecurityRow[0],
    4u
};

/************************************************
 ****DcmDspSessionRow container(Multiplicity=0..31)
 ************************************************/
STATIC  CONST(Dcm_DspSessionRowType,DCM_CONST)Dcm_DspSessionRow[3] =
{ 
    { /* DefaultSession */
        1u,
        50u,
        2000u
    },
    { /* ProgrammingSession */
        2u,
        50u,
        2000u
    },
    { /* ExtendedDiagnosticSession */
        3u,
        50u,
        2000u
    }
};

/************************************************
 *******Dcm_DspSession container(Multiplicity=1)*
 ************************************************/
STATIC  CONST(Dcm_DspSessionType,DCM_CONST)Dcm_DspSession =
{
    &Dcm_DspSessionRow[0],
    3u,
};

/************************************************
 ******DcmDspDid container(Multiplicity=0..*)****
 ***********************************************/
/******************************************
 *DcmDspDidRead container configration
 *****************************************/
/*DID = DidInfo_Read*/
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Read_Read_SecRefCfg[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Read_Read_SesRefCfg[3] = {1,2,3};
STATIC  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_Read_ReadContainerCfg =
{
    4u,
    &Dcm_DidInfo_Read_Read_SecRefCfg[0],
    3u,
    &Dcm_DidInfo_Read_Read_SesRefCfg[0],
};
/*DID = DidInfo_ReadWrite*/
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_ReadWrite_Read_SecRefCfg[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_ReadWrite_Read_SesRefCfg[3] = {1,2,3};
STATIC  CONST(Dcm_DspDidReadType,DCM_CONST)Dcm_DidInfo_ReadWrite_ReadContainerCfg =
{
    4u,
    &Dcm_DidInfo_ReadWrite_Read_SecRefCfg[0],
    3u,
    &Dcm_DidInfo_ReadWrite_Read_SesRefCfg[0],
};

/*******************************************
 *DcmDspDidWrite container configuration,
 which is in the DcmDspDidInfo container
 ******************************************/
/*DID = DidInfo_ReadWrite*/
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_ReadWrite_Write_SecRefCfg[1] = {1};
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_ReadWrite_Write_SesRefCfg[2] = {2,3};
STATIC  CONST(Dcm_DspDidWriteType,DCM_CONST)Dcm_DidInfo_ReadWrite_WriteContainerCfg=
{
    1u,
    &Dcm_DidInfo_ReadWrite_Write_SecRefCfg[0],
    2u,
    &Dcm_DidInfo_ReadWrite_Write_SesRefCfg[0],
};
/*DID = DidInfo_Write*/
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Write_Write_SecRefCfg[1] = {1};
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Write_Write_SesRefCfg[2] = {2,3};
STATIC  CONST(Dcm_DspDidWriteType,DCM_CONST)Dcm_DidInfo_Write_WriteContainerCfg=
{
    1u,
    &Dcm_DidInfo_Write_Write_SecRefCfg[0],
    2u,
    &Dcm_DidInfo_Write_Write_SesRefCfg[0],
};

/*******************************************
 *DcmDspDidControl container configuration,
 which is in the DcmDspDidInfo container
 ******************************************/ 
/*DID = DidInfo_Control*/
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Control_Control_SecRefCfg[1]= {1};
STATIC  CONST(uint8,DCM_CONST)Dcm_DidInfo_Control_Control_SesRefCfg[1]={3};

CONST( Dcm_DspDidControlRecordSizesType,DCM_CONST)
Dcm_DidInfo_Control_ControlRecordSizesCfg[2]=
{
    {0,0,0},
    {0,0,0},
};

STATIC  CONST(Dcm_DspDidControlType,DCM_CONST)
Dcm_DidInfo_Control_ControlContainerCfg =
{
    1u,
    &Dcm_DidInfo_Control_Control_SecRefCfg[0],
    1u,
    &Dcm_DidInfo_Control_Control_SesRefCfg[0],
    &Dcm_DidInfo_Control_ControlRecordSizesCfg[0], /*FreezeCurrentState*/
    &Dcm_DidInfo_Control_ControlRecordSizesCfg[1], /*ResetToDefault*/
    NULL_PTR, /*ReturnControlToEcu*/
    NULL_PTR, /*ShortTermAdjustement*/
};



/*******************************************
 **DcmDspDidAccess container configration**
 ******************************************/ 
/*DID = DidInfo_Read*/
STATIC  CONST(Dcm_DspDidAccessType,DCM_CONST)Dcm_DidInfo_Read_AccessCfg =
{
    NULL_PTR,
    &Dcm_DidInfo_Read_ReadContainerCfg,
    NULL_PTR
}; 
/*DID = DidInfo_ReadWrite*/
STATIC  CONST(Dcm_DspDidAccessType,DCM_CONST)Dcm_DidInfo_ReadWrite_AccessCfg =
{
    NULL_PTR,
    &Dcm_DidInfo_ReadWrite_ReadContainerCfg,
    &Dcm_DidInfo_ReadWrite_WriteContainerCfg
}; 
/*DID = DidInfo_Write*/
STATIC  CONST(Dcm_DspDidAccessType,DCM_CONST)Dcm_DidInfo_Write_AccessCfg =
{
    NULL_PTR,
    NULL_PTR,
    &Dcm_DidInfo_Write_WriteContainerCfg
}; 
/*DID = DidInfo_Control*/
STATIC  CONST(Dcm_DspDidAccessType,DCM_CONST)Dcm_DidInfo_Control_AccessCfg =
{
    &Dcm_DidInfo_Control_ControlContainerCfg,
    NULL_PTR,
    NULL_PTR
};

/******************************************
 *DcmDspDidInfo container Configuration *****
 ******************************************/
STATIC  CONST(Dcm_DspDidInfoType,DCM_CONST)Dcm_DspDidInfoCfg[4] =
{
    { /* DidInfo_Read */
        FALSE,	/*true = DID can be dynamically defined, false = DID can not bedynamically defined*/
        TRUE,    /*true = datalength of the DID is fixed, false = datalength of the DID is variable*/
        0u,      /*If Scaling information service is available for this DID, it provides the size of the scaling information.*/
        &Dcm_DidInfo_Read_AccessCfg
    },
    { /* DidInfo_ReadWrite */
        FALSE,	/*true = DID can be dynamically defined, false = DID can not bedynamically defined*/
        TRUE,    /*true = datalength of the DID is fixed, false = datalength of the DID is variable*/
        0u,      /*If Scaling information service is available for this DID, it provides the size of the scaling information.*/
        &Dcm_DidInfo_ReadWrite_AccessCfg
    },
    { /* DidInfo_Write */
        FALSE,	/*true = DID can be dynamically defined, false = DID can not bedynamically defined*/
        TRUE,    /*true = datalength of the DID is fixed, false = datalength of the DID is variable*/
        0u,      /*If Scaling information service is available for this DID, it provides the size of the scaling information.*/
        &Dcm_DidInfo_Write_AccessCfg
    },
    { /* DidInfo_Control */
        FALSE,	/*true = DID can be dynamically defined, false = DID can not bedynamically defined*/
        FALSE,    /*true = datalength of the DID is fixed, false = datalength of the DID is variable*/
        0u,      /*If Scaling information service is available for this DID, it provides the size of the scaling information.*/
        &Dcm_DidInfo_Control_AccessCfg
    }
};

/**********************************************
 *DcmDspDid container configration*************
 **********************************************/
STATIC  CONST(Dcm_DspDidType,DCM_CONST)Dcm_DspDidCfg[47] =
{ 
    { /* Did_01_f10d */
        0xF10D,         /*ECU Software Number of Supplier*/
        3u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f10d,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_02_f18c */
        0xF18C,         /*ECU Software Number of Supplier*/
        17u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f18c,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_03_f199 */
        0xF199,         /*ECU Software Number of Supplier*/
        4u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f199,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidWriteData_f199,
        1u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_04_f187 */
        0xF187,         /*ECU Software Number of Supplier*/
        14u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f187,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_05_f180 */
        0xF180,         /*ECU Software Number of Supplier*/
        17u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f180,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        1u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_06_f189 */
        0xF189,         /*ECU Software Number of Supplier*/
        17u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f189,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_07_f17f */
        0xF17F,         /*ECU Software Number of Supplier*/
        17u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f17f,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_08_f184 */
        0xF184,         /*ECU Software Number of Supplier*/
        10u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f184,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidWriteData_f184,
        1u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_09_0201 */
        0x201,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_0201,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_10_0200 */
        0x200,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_0200,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_11_0110 */
        0x110,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_0110,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidWriteData_0110,
        1u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_12_f186 */
        0xF186,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_f186,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_13_1000 */
        0x1000,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_1000,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_14_1400 */
        0x1400,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_1400,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_15_1404 */
        0x1404,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_1404,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_16_1409 */
        0x1409,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_1409,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_17_0505 */
        0x505,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_0505,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_18_141e */
        0x141E,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_141e,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_19_141f */
        0x141F,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_141f,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_20_3011 */
        0x3011,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_3011,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_21_3012 */
        0x3012,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_3012,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_22_2005 */
        0x2005,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2005,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_23_2006 */
        0x2006,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2006,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_24_2007 */
        0x2007,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2007,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_25_2008 */
        0x2008,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2008,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_26_2020 */
        0x2020,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2020,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_27_2021 */
        0x2021,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2021,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_28_2022 */
        0x2022,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2022,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_29_2023 */
        0x2023,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2023,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_30_1001 */
        0x1001,         /*ECU Software Number of Supplier*/
        104u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_1001,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_31_2024 */
        0x2024,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2024,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_32_2025 */
        0x2025,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2025,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_33_2026 */
        0x2026,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2026,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_34_2027 */
        0x2027,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2027,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_35_2000 */
        0x2000,         /*ECU Software Number of Supplier*/
        24u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2000,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_36_2009 */
        0x2009,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2009,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_37_2010 */
        0x2010,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2010,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_38_2011 */
        0x2011,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2011,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_39_2012 */
        0x2012,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2012,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_40_2013 */
        0x2013,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2013,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_41_2014 */
        0x2014,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2014,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_42_2015 */
        0x2015,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2015,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_43_2016 */
        0x2016,         /*ECU Software Number of Supplier*/
        2u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2016,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_44_2017 */
        0x2017,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2017,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_45_2018 */
        0x2018,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2018,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_46_2019 */
        0x2019,         /*ECU Software Number of Supplier*/
        1u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_DidReadData_2019,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        0u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    },
    { /* Did_00_control */
        0xD001,         /*ECU Software Number of Supplier*/
        0u,            /*DcmDspDidSize*/
        NULL_PTR,
        NULL_PTR,
        Rte_DidFreezeCurrentState_d001,
        NULL_PTR,
        NULL_PTR,
        NULL_PTR,
        Rte_ResetToDefault_d001,
        Rte_DidReturnControlToEcu_d001,
        Rte_DidShortTermAdjustment_d001,
        NULL_PTR,
        3u,            /*DcmDspDidInfo array subscript*/
        0u,
        NULL_PTR,
        0u,
        NULL_PTR
    }      
};

/***********************************************
 ***DcmDspEcuReset container configration*******
 ***********************************************/
STATIC  CONST(Dcm_EcuResetType,DCM_CONST)Dcm_EcuResetPort[1]=
{
    Rte_EcuReset,
};

STATIC  CONST(uint8,DCM_CONST)Dcm_DspEcuReset_EcuReset_Hard_RefSecCfg[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_DspEcuReset_EcuReset_Hard_RefSesCfg[2] = {2,3};
STATIC  CONST(Dcm_DspEcuResetType,DCM_CONST)Dcm_DspEcuResetCfg[1]=
{
    { /* EcuReset_Hard */
        1u,    /*ResetType=hard reset */
        4u,
        &Dcm_DspEcuReset_EcuReset_Hard_RefSecCfg[0],
        2u,
        &Dcm_DspEcuReset_EcuReset_Hard_RefSesCfg[0],
    }
};

/*************************************************
 ************DcmDspReadDTC container configration
 *************************************************/
CONST(uint8,DCM_CONST)Dcm_DspReadDTC_ReadDTC_1_RefSecCfg[4] = {0,1,2,9};
CONST(uint8,DCM_CONST)Dcm_DspReadDTC_ReadDTC_2_RefSecCfg[4] = {0,1,2,9};
CONST(uint8,DCM_CONST)Dcm_DspReadDTC_ReadDTC_4_RefSecCfg[4] = {0,1,2,9};
CONST(uint8,DCM_CONST)Dcm_DspReadDTC_ReadDTC_6_RefSecCfg[4] = {0,1,2,9};

CONST(Dcm_DspReadDTCRowType,DCM_CONST)Dcm_DspReadDTCRowCfg[4] =
{
    { /* ReadDTC_1 */
        0x01,  /*subfunction: reportNumberOfDTCByStatusMask */
        TRUE,  /*TRUE =sub-function supported. FALSE = sub-function not supported*/
        4u,
        &Dcm_DspReadDTC_ReadDTC_1_RefSecCfg[0],
    },
    { /* ReadDTC_2 */
        0x02,  /*subfunction: reportNumberOfDTCByStatusMask */
        TRUE,  /*TRUE =sub-function supported. FALSE = sub-function not supported*/
        4u,
        &Dcm_DspReadDTC_ReadDTC_2_RefSecCfg[0],
    },
    { /* ReadDTC_4 */
        0x04,  /*subfunction: reportNumberOfDTCByStatusMask */
        TRUE,  /*TRUE =sub-function supported. FALSE = sub-function not supported*/
        4u,
        &Dcm_DspReadDTC_ReadDTC_4_RefSecCfg[0],
    },
    { /* ReadDTC_6 */
        0x06,  /*subfunction: reportNumberOfDTCByStatusMask */
        TRUE,  /*TRUE =sub-function supported. FALSE = sub-function not supported*/
        4u,
        &Dcm_DspReadDTC_ReadDTC_6_RefSecCfg[0],
    }
};

CONST(Dcm_DspReadDTCType,DCM_CONST)Dcm_DspReadDTCCfg =
{
    4u,
    &Dcm_DspReadDTCRowCfg[0]
};

/*************************************************
 *****DcmDspRoutine container configration********
 *************************************************/
/***********************************
 *DcmDspRoutineAuthorization container
 **********************************/
STATIC  CONST(uint8,DCM_CONST)Dcm_RoutineInfo_1_Authorization_RefSecCfg[1] = {1};
STATIC  CONST(uint8,DCM_CONST)Dcm_RoutineInfo_1_Authorization_RefSesCfg[2] = {2,3};
STATIC  CONST(uint8,DCM_CONST)Dcm_RoutineInfo_2_Authorization_RefSecCfg[4] = {1,0,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_RoutineInfo_2_Authorization_RefSesCfg[2] = {2,3};

STATIC  CONST(Dcm_DspRoutineAuthorizationType,DCM_CONST)Dcm_RoutineInfo_1_AuthorizationCfg =
{
    1u,
    &Dcm_RoutineInfo_1_Authorization_RefSecCfg[0],
    2u,
    &Dcm_RoutineInfo_1_Authorization_RefSesCfg[0],
};
STATIC  CONST(Dcm_DspRoutineAuthorizationType,DCM_CONST)Dcm_RoutineInfo_2_AuthorizationCfg =
{
    4u,
    &Dcm_RoutineInfo_2_Authorization_RefSecCfg[0],
    2u,
    &Dcm_RoutineInfo_2_Authorization_RefSesCfg[0],
};
/***********************************
 *DcmDspRoutineStart container
 **********************************/
STATIC CONST(Dcm_DspStartRoutineType,DCM_CONST)Dcm_RoutineInfo_1_StartCfg =   
{
     1u,  /*Size of optional record in the Routine Start request*/
     1u   /*Size of optional record in the Routine Start response*/
};
STATIC CONST(Dcm_DspStartRoutineType,DCM_CONST)Dcm_RoutineInfo_2_StartCfg =   
{
     0u,  /*Size of optional record in the Routine Start request*/
     0u   /*Size of optional record in the Routine Start response*/
};

/***********************************
 *DcmDspRoutineStop container
 **********************************/
STATIC CONST(Dcm_DspRoutineStopType,DCM_CONST)Dcm_RoutineInfo_1_StopCfg =    
{
    1u, /*Size of optional record in the stop request*/
    1u, /*Size of optional record in the stop response*/
}; 

/***********************************
 *DcmDspRoutineRequestRes container configration
 **********************************/
/*In the the response of RequestResult,the size of the number of bytes for Optional record  */
STATIC CONST(Dcm_DspRoutineRequestResType,DCM_CONST)Dcm_RoutineInfo_1_RequestResCfg = {1};

/***********************************
 *DcmDspRoutineInfo container configration
 **********************************/
STATIC  CONST(Dcm_DspRoutineInfoType,DCM_CONST)Dcm_DspRoutineInfoCfg[2] =
{
    { /* RoutineInfo_1 */
        &Dcm_RoutineInfo_1_AuthorizationCfg,
        &Dcm_RoutineInfo_1_StartCfg,
        &Dcm_RoutineInfo_1_StopCfg,
        &Dcm_RoutineInfo_1_RequestResCfg,
    },
    { /* RoutineInfo_2 */
        &Dcm_RoutineInfo_2_AuthorizationCfg,
        &Dcm_RoutineInfo_2_StartCfg,
        NULL_PTR,
        NULL_PTR,
    }
};

/***********************************
 *DcmDspRoutine container configration
 **********************************/
STATIC  CONST(Dcm_DspRoutineType,DCM_CONST)Dcm_DspRoutineCfg[2] =
{
    { /* Routine_1 */
        0x60E,  /*Routine ID*/
        StartRoutine_060E,   /*Start Routine Fnc*/
        StopRoutine_060E,   /*Stop Routine Fnc*/
        RequestResults_060E,   /*Request Results Routine Fnc*/
        0u,  					 /*Dcm_DspRoutineInfoCfg[] array subscript*/
    },
    { /* Routine_2 */
        0x203,  /*Routine ID*/
        StartRoutine_0203,   /*Start Routine Fnc*/
        NULL_PTR,   /*Stop Routine Fnc*/
        NULL_PTR,   /*Request Results Routine Fnc*/
        1u,  					 /*Dcm_DspRoutineInfoCfg[] array subscript*/
    }

};

/************************************************
 *******DcmDsp container configration(Multiplicity=1)**
 ************************************************/
CONST(Dcm_DspCfgType,DCM_CONST)Dcm_DspCfg =
{
    0, /*Indicates the maximum allowed DIDs in a single "ReadDataByIdentifier" request. If set to 0, then no limitation is applied. */
    47u,
    &Dcm_DspDidCfg[0],    /* Dids */
    4u,
    &Dcm_DspDidInfoCfg[0], /* Did Infos*/
    1u,
    &Dcm_DspEcuResetCfg[0],/* Resets */ 
    1u,
    &Dcm_EcuResetPort[0],   /* Resets Callback*/
    &Dcm_DspReadDTCCfg,        /* Read DTCs */
    2u,
    &Dcm_DspRoutineCfg[0],    /* Routines */
    2u,
    &Dcm_DspRoutineInfoCfg[0], /* Routine Infos */
    &Dcm_DspSecurity,  /* Security levels */
    &Dcm_DspSession,  /* Sessions */
};
/*****************************************************************************************
 ********************************* DSD container configration*****************************
 *****************************************************************************************/
/**********************************************************************/
/******UDS Service session and security configration******/
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ClearDiagnosticInformation_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ClearDiagnosticInformation_SesRef[2] = {1,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ReadDTCInformation_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ReadDTCInformation_SesRef[2] = {1,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_DiagnosticSessionControl_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_DiagnosticSessionControl_SesRef[3] = {1,2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ECU_Reset_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ECU_Reset_SesRef[2] = {2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_InputOutputControlByIdentifie_SecRef[1] = {1};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_InputOutputControlByIdentifie_SesRef[1] = {3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_CommunicationControl_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_CommunicationControl_SesRef[1] = {3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ControlDTCSetting_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ControlDTCSetting_SesRef[1] = {3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ReadDataByIdentifier_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_ReadDataByIdentifier_SesRef[3] = {1,2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_RoutineControl_SecRef[4] = {1,2,0,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_RoutineControl_SesRef[2] = {2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_SecurityAccess_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_SecurityAccess_SesRef[2] = {2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_TesterPresent_SecRef[4] = {0,1,2,9};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_TesterPresent_SesRef[3] = {1,2,3};  
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_WriteDataByIdentifier_SecRef[2] = {1,2};
STATIC  CONST(uint8,DCM_CONST)Dcm_UDS_WriteDataByIdentifier_SesRef[2] = {2,3};  
/**********************************************************************/
STATIC CONST(Dcm_DsdServiceCfgType,DCM_CONST)Dcm_Dsd_UDS_ServiceTable_Service[12] =
{
    { /* ClearDiagnosticInformation */
        0x14u,
        FALSE,
        4u,
        &Dcm_UDS_ClearDiagnosticInformation_SecRef[0],
        2u,
        &Dcm_UDS_ClearDiagnosticInformation_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x14,
    },
    { /* ReadDTCInformation */
        0x19u,
        TRUE,
        4u,
        &Dcm_UDS_ReadDTCInformation_SecRef[0],
        2u,
        &Dcm_UDS_ReadDTCInformation_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x19,
    },
    { /* DiagnosticSessionControl */
        0x10u,
        TRUE,
        4u,
        &Dcm_UDS_DiagnosticSessionControl_SecRef[0],
        3u,
        &Dcm_UDS_DiagnosticSessionControl_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x10,
    },
    { /* ECU_Reset */
        0x11u,
        TRUE,
        4u,
        &Dcm_UDS_ECU_Reset_SecRef[0],
        2u,
        &Dcm_UDS_ECU_Reset_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x11,
    },
    { /* InputOutputControlByIdentifie */
        0x2Fu,
        TRUE,
        1u,
        &Dcm_UDS_InputOutputControlByIdentifie_SecRef[0],
        1u,
        &Dcm_UDS_InputOutputControlByIdentifie_SesRef[0],
        DCM_ADDRESSING_PHYSICAL,
        DspInternal_UDS0x2F,
    },
    { /* CommunicationControl */
        0x28u,
        TRUE,
        4u,
        &Dcm_UDS_CommunicationControl_SecRef[0],
        1u,
        &Dcm_UDS_CommunicationControl_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x28,
    },
    { /* ControlDTCSetting */
        0x85u,
        TRUE,
        4u,
        &Dcm_UDS_ControlDTCSetting_SecRef[0],
        1u,
        &Dcm_UDS_ControlDTCSetting_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x85,
    },
    { /* ReadDataByIdentifier */
        0x22u,
        FALSE,
        4u,
        &Dcm_UDS_ReadDataByIdentifier_SecRef[0],
        3u,
        &Dcm_UDS_ReadDataByIdentifier_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x22,
    },
    { /* RoutineControl */
        0x31u,
        TRUE,
        4u,
        &Dcm_UDS_RoutineControl_SecRef[0],
        2u,
        &Dcm_UDS_RoutineControl_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC, //DCM_ADDRESSING_PHYSICAL,
        DspInternal_UDS0x31,
    },
    { /* SecurityAccess */
        0x27u,
        TRUE,
        4u,
        &Dcm_UDS_SecurityAccess_SecRef[0],
        2u,
        &Dcm_UDS_SecurityAccess_SesRef[0],
        DCM_ADDRESSING_PHYSICAL,
        DspInternal_UDS0x27,
    },
    { /* TesterPresent */
        0x3Eu,
        TRUE,
        4u,
        &Dcm_UDS_TesterPresent_SecRef[0],
        3u,
        &Dcm_UDS_TesterPresent_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x3E,
    },
    { /* WriteDataByIdentifier */
        0x2Eu,
        FALSE,
        2u,
        &Dcm_UDS_WriteDataByIdentifier_SecRef[0],
        2u,
        &Dcm_UDS_WriteDataByIdentifier_SesRef[0],
        DCM_ADDRESSING_PHYANDFUNC,
        DspInternal_UDS0x2E,
    }  
};

/**********************************************************************/
/*DCM Support Service Table(Multiplicity=1..256)*/
STATIC  CONST(Dcm_DsdServiceTableCfgType,DCM_CONST)Dcm_DsdServiceTable[DCM_SERVICE_TAB_NUM]=
{
    {
        0x0,
        &Dcm_Dsd_UDS_ServiceTable_Service[0],
        DCM_UDS_SERVICE_NUM	
    }
};

/**********************************************************************/
/*Dsd container(Multiplicity=1)*/
CONST(Dcm_DsdCfgType,DCM_CONST)Dcm_DsdCfg =
{
    &Dcm_DsdServiceTable[0],
    DCM_SERVICE_TAB_NUM
};

/*****************************************************************************************
 ********************************* DSL container configration*****************************
 *****************************************************************************************/
/*DcmDslBuffer container(Multiplicity=1..256)*/
STATIC  CONST(Dcm_DslBufferType,DCM_CONST)Dcm_DslBufferCfg[DCM_CHANNEL_NUM] =
{
    {
        0x0u, 
        255u,
        0u
    },
    {
        0x1u, 
        255u,
        255u
    }
};

/***********************************/
/*DcmDslDiagResp container(Multiplicity=1)*/
STATIC  CONST(Dcm_DslDiagRespType,DCM_CONST)Dcm_DslDiagRespCfg =
{
    DCM_DSLDIAGRESP_FORCERESPENDEN,
    3u
};

/*****************************************************
 *DcmDslCallbackDCMRequestService port configration(Multiplicity=1..*)
 *****************************************************/
STATIC  CONST(Dcm_DslCallbackDCMRequestServiceType,DCM_CONST)Dcm_DslCallbackDCMRequestServiceCfg[1] =
{
    {
        StartProtocol,
        StopProtocol
    }
};

/*****************************************************
 *DcmDslSessionControl port configration(Multiplicity=1..*)*******
 ****************************************************/
STATIC  CONST(Dcm_DslSessionControlType,DCM_CONST)Dcm_DslSessionControlCfg[1]=
{
    {
        ChangeIndication,
        GetSesChgPermission
    }
};

/*****************************************************
 *DcmDslServiceRequestIndication port configration(Multiplicity=0..*)*
 ****************************************************/
STATIC  CONST(Dcm_ServiceRequestIndicationType,DCM_CONST)Dcm_ServiceRequestIndicationCfg[1] =
{
    {
        Rte_Indication1
    }
};

/*****************************************************
 ****DcmDslProtocolTiming container(Multiplicity=1)***********
 ****************************************************/
CONST(Dcm_DslProtocolTimingRowType,DCM_CONST)Dcm_DslProtocolTimingRowCfg[1] =
{
    {
        50U,  	/*P2ServerMax*/
        0U,
        5000U,	/* P2StarServerMax */
        0U,
        5000U	/*S3Server*/
    }
};

CONST(Dcm_DslProtocolTimingType,DCM_CONST)Dcm_DslProtocolTimingCfg=
{
    &Dcm_DslProtocolTimingRowCfg[0],
    1u,
};

/******************************************************
 *************DcmDslConnection container***************
 *****************************************************/

/********************UDS protocal Connection configration*******************/
/*Connection1,Mainconnection,ProtocolRx configration(Multiplicity=1..*)*/
STATIC  CONST(Dcm_DslProtocolRxType,DCM_CONST)Dcm_Protocol_1_Connection_0_RxCfg[2]=
{
    {
        DCM_FUNCTIONAL,       /*DcmDslProtocolRxAddrType*/
        0x4u,              /*DcmDslProtocolRxPduId*/
    },
    {
        DCM_PHYSICAL,       /*DcmDslProtocolRxAddrType*/
        0x3u,              /*DcmDslProtocolRxPduId*/
    }
};

/*Connection1,Mainconnection,ProtocolTx configration(Multiplicity=1..*)*/
STATIC  CONST(Dcm_DslMainConnectionType,DCM_CONST) Dcm_Protocol_1_Connection_0Cfg =
{
    NULL_PTR,
    NULL_PTR,
    &Dcm_Protocol_1_Connection_0_RxCfg[0], /*pDcmDslProtocolRx*/
    2u,                     /*DcmDslProtocolRx_Num*/
    0x0u                  /*DcmDslProtocolTxPduId*/
};

/*Connection1 configration*/
STATIC  CONST(Dcm_DslConnectionType,DCM_CONST)Dcm_Dsl_Protocol_1_ConnectionCfg[1]=
{
    {
        &Dcm_Protocol_1_Connection_0Cfg,  /*pDcmDslMainConnection*/
        NULL_PTR,         /*pDcmDslPeriodicTransmission*/
        NULL_PTR          /*pDcmDslResponseOnEvent*/
    }
};
/*****************************************************
 ****Dcm_DslProtocolRow container configration(Multiplicity=1..*)*******
 ****************************************************/
STATIC  CONST(Dcm_DslProtocolRowType,DCM_CONST)Dcm_DslProtocolRowCfg[DCM_DSLPROTOCOLROW_NUM_MAX] =
{
    {
        DCM_UDS_ON_CAN,			/*DcmDslProtocolID*/
        FALSE,					/*DcmDslProtocolIsParallelExecutab*/
        0u,					/*DcmDslProtocolPreemptTimes*/
        0u,						/*DcmDslProtocolPriority*/
        DCM_PROTOCOL_TRAN_TYPE1,/*DcmDslProtocolTransType*/
        0x0,					/*DcmDslProtocolRxBufferID*/
        0x1,					/*DcmDslProtocolTxBufferID*/
        0x0, 					/*DcmDslServiceTableID*/
        &Dcm_DslProtocolTimingRowCfg[0],			    /*pDcmDslProtocolTimeLimit*/
        &Dcm_Dsl_Protocol_1_ConnectionCfg[0],/*DcmDslConnection*/
        1u,		/*Number of connection*/
    }
};

/*****************************************************
 *DcmDslProtocol container configration(Multiplicity=1)
 ****************************************************/
STATIC  CONST(Dcm_DslProtocolType,DCM_CONST)Dcm_DslProtocol =
{
    &Dcm_DslProtocolRowCfg[0],
    DCM_DSLPROTOCOLROW_NUM_MAX,
};

/*****************************************************
 ****************DcmDsl container configration*****
 ****************************************************/
CONST(Dcm_DslCfgType,DCM_CONST)Dcm_DslCfg =
{
    DCM_CHANNEL_NUM,				/*Number of Channel configration*/
    &Dcm_DslBufferCfg[0],

    1u,	/*Number of DslCallbackDCMRequestService port*/
    &Dcm_DslCallbackDCMRequestServiceCfg[0],
    1u,			/*Number of ServiceRequestIndication port*/
    &Dcm_ServiceRequestIndicationCfg[0],
    1u,			/*Number of SessionControl port*/
    &Dcm_DslSessionControlCfg[0], /*reference to SessionControl port configration*/

    &Dcm_DslDiagRespCfg,          /*reference to DcmDslDiagResp configration*/
    &Dcm_DslProtocol,             /*reference to DcmDslProtocol configration*/
    &Dcm_DslProtocolTimingCfg
};

#define  DCM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"