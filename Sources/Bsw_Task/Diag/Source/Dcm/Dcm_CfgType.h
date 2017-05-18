/*============================================================================*/
/*  Copyright (C) 2009-2011, iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *
 *  All rights reserved. This software is iSOFT property. Duplication
 *  or disclosure without iSOFT written authorization is prohibited.
 *
 *  @file        <Dcm_CfgType.h>
 *  @brief       <>
 *
 *  <Compiler: CodeWarrior    MCU:XXX>
 *
 *  @author     <chen maosen>
 *  @date       <2013-03-20>
 */
/*============================================================================*/

#ifndef DCM_CFGTYPE_H
#define DCM_CFGTYPE_H

/****************************** references *********************************/
#include "Dcm_Types.h"

/****@req DCM-FUNR-901****/
/**************************************************************************
 ******************************DcmGeneral Container************************
 **************************************************************************/
/****@req DCM_CFGR_087[DCM075]****/
/****@req DCM_CFGR_088[DCM076]****/
/****@req DCM_CFGR_089[DCM600]****/
typedef  struct
{
    const  boolean   DcmDevErrorDetect;          /*Preprocessor switch to enable or disable the Development Error Detection (DET) mechanism*/
    const  boolean   DcmRequestIndicationEnabled;/*Allows to enable or disable the requested indication mechanism.*/
    const  boolean   DcmRespondAllRequest;       /*=FALSE:the DCM will not respond to diagnostic request that contains a service ID,
                                                  which is in the range from 0x40 to 0x7F or in the range from 0xC0 to 0xFF*/
    const  boolean   DcmVersionInfoApi;          /*enable or disable the output Version info*/
    const  uint32    DcmTaskTime;                /*the time (in ms)for the periodic cyclic task*/
}Dcm_GeneralCfgType;

/**************************************************************************
 ************************DcmPageBufferCfg Container************************
 **************************************************************************/
#if(STD_ON == DCM_PAGEDBUFFER_ENABLED)
/****@req DCM_CFGR_090[DCM068]****/
typedef  struct
{
    const  boolean   DcmPagedBufferEnabled;		/*true = Page buffer handling enabled ;false = Page Buffer handling disabled*/
    const  uint32    DcmPagedBufferTimeout;		/*the Timeout (in ms) towards the application for filling the next page*/
}Dcm_PageBufferCfgType;
#endif

/*************************************************************************
 ***********************************DcmDsp Container**********************
 *************************************************************************/
/****@req DCM_CFGR_074[DCM073]****/
/*DcmDspSecurity Container*/
/*SecurityAccess_<Level>*/
typedef  struct
{
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,CompareKey)(P2VAR(uint8,AUTOMATIC,DCM_VAR)key);
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,GetSeed)   (P2VAR(uint8,AUTOMATIC,DCM_VAR)SecurityAccessRecord,
    		                                        P2VAR(uint8,AUTOMATIC,DCM_VAR)Seed,
    		                                        P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
}Dcm_SecurityAccessType;

/*Multiplicity=0..31*/
typedef  struct
{
    const  uint8  DcmDspSecurityLevel;       /*Range:0~63*/
    const  uint8  DcmDspSecuritySeedSize;    /*size of the security seed (in Bytes),Range:1~255*/
    const  uint8  DcmDspSecurityKeySize;     /*size of the security key (in Bytes),Range:1~255*/
    const  uint8  DcmDspSecurityADRSize;     /*Size of the AccessDataRecord used in GetSeed,Range:1~255*/
    const  uint8  DcmDspSecurityNumAttDelay; /*Number of failed security accesses after which the delay time is activated,Range:1~255*/
    const  uint8  DcmDspSecurityNumAttLock;  /*Number of failed security accesses after which security access is locked.*/
    const  uint32 DcmDspSecurityDelayTime;   /*Delay time after failed security access(unit:ms)*/
    const  uint32 DcmDspSecurityDelayTimeOnBoot;/*Start delay timer on power on(unit:ms)*/
    const  Dcm_SecurityAccessType  Dcm_SecurityAccess;
}Dcm_DspSecurityRowType;

/*Multiplicity = 1*/
typedef struct
{
   P2CONST(Dcm_DspSecurityRowType,TYPEDEF,DCM_CONST)pDcm_DspSecurityRow;/*reference DspSecurityRow container*/
   const  uint8  DcmDspSecurityRow_Num;   /*Number of DspSecurityRow*/
}Dcm_DspSecurityType;


/****@req DCM_CFGR_075[DCM072]****/
/*DcmDspSession */
/*Multiplicity=0..31*/
typedef struct
{
   const  uint8  DcmDspSessionLevel;          /*hex value of the Session contro;Range:0~255*/
   const  uint32 DcmDspSessionP2ServerMax;    /*This is the session value for P2ServerMax(in ms);*/
   const  uint32 DcmDspSessionP2StarServerMax;/*This is the session value for P2*ServerMax(in ms);*/
}Dcm_DspSessionRowType;

/*Multiplicity=1*/
typedef struct
{
   P2CONST(Dcm_DspSessionRowType,TYPEDEF,DCM_CONST)pDcmDspSessionRow;/*reference DspSessionRow container*/
   const  uint8  DcmDspSessionRow_Num;   	 /*Number of DspSessionRow*/
}Dcm_DspSessionType;

/*****************************************************************/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
/*DcmDspDidControlRecordSizes container,Multiplicity=0..* */
typedef  struct
{
   const  uint8  DcmDspDidControlEnableMaskRecordSize; /*Multiplicity=1,This defines the size of the controlEnableMaskRecord as defined in ISO14229-1 for UDS Service 0x2F*/
   const  uint8  DcmDspDidControlOptionRecordSize;     /*Multiplicity=1,This defines the size of the controlOptionRecord
                                                         (as defined in ISO14229-1 for service UDS 0x2F) without the InputOutputControlParamete*/
   const  uint8  DcmDspDidControlStatusRecordSize;     /*Multiplicity=1,This defines the size of the controlStatusRecord
                                                         (as defined in ISO14229-1 for UDS Service 0x2F) WITHOUT the InputOutputControlParameter*/
}Dcm_DspDidControlRecordSizesType;

typedef  struct
{
   const  uint8  DcmDspDidControlSecurityLevelRefNum;  /*Number Of DcmDspSecurityRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidControlSecurityLevelRow;/*DcmDspSecurityRow Referenced*/
   const  uint8  DcmDspDidControlSessionRefNum;        /*Number Of DcmDspSessionRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidControlSessionRow;/*DcmDspSessionRow Referenced*/
   P2CONST(Dcm_DspDidControlRecordSizesType,TYPEDEF,DCM_CONST)pDcmDspDidFreezeCurrentState;/*Multiplicity=0..1; This indicates the presence of "FreezeCurrentState" and
                                                                                             refers to a container defining the sizes of the parameters*/
   P2CONST(Dcm_DspDidControlRecordSizesType,TYPEDEF,DCM_CONST)pDcmDspDidResetToDefault;    /*Multiplicity=0..1; This indicates the presence of "ResetToDefault" and
                                                                                             refers to a container defining the sizes of the parameters*/
   P2CONST(Dcm_DspDidControlRecordSizesType,TYPEDEF,DCM_CONST)pDcmDspDidReturnControlToEcu;/*Multiplicity=0..1; This indicates the presence of "ReturnControlToEcu" and
																							 refers to a container defining the sizes of the parameters*/
   P2CONST(Dcm_DspDidControlRecordSizesType,TYPEDEF,DCM_CONST)pDcmDspDidShortTermAdjustement;/*Multiplicity=0..1;This indicates the presence of "ShortTermAdjustment" and
																							   refers to a container defining the sizes of the parameters*/
}Dcm_DspDidControlType;

typedef  struct
{
   const  uint8  DcmDspDidReadSecurityLevelRefNum; /*Number Of DcmDspSecurityRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidReadSecurityLevelRow; /*DcmDspSecurityRow Referenced*/
   const  uint8  DcmDspDidReadSessionRefNum;       /*Number Of DcmDspSessionRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidReadSessionRow;/*DcmDspSessionRow Referenced*/
}Dcm_DspDidReadType;

typedef  struct
{
   const  uint8   DcmDspDidWriteSecurityLevelRefNum;/*Number Of DcmDspSecurityRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidWriteSecurityLevelRow;/*DcmDspSecurityRow Referenced*/
   const  uint8   DcmDspDidWriteSessionRefNum;      /*Number Of DcmDspSessionRow Referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDidWriteSessionRow;/*DcmDspSessionRow Referenced*/
}Dcm_DspDidWriteType;

typedef  struct
{
   P2CONST(Dcm_DspDidControlType,TYPEDEF,DCM_CONST)pDcmDspDidControl;/*Multiplicity=0..1, the configuration (parameters)of the DID control*/
   P2CONST(Dcm_DspDidReadType,TYPEDEF,DCM_CONST)pDcmDspDidRead;		 /*Multiplicity=0..1, configuration (parameters)of the DID read*/
   P2CONST(Dcm_DspDidWriteType,TYPEDEF,DCM_CONST)pDcmDspDidWrite;    /*Multiplicity=0..1, configuration (parameters)of the DID write*/
}Dcm_DspDidAccessType;

/*DcmDspDidInfo,Multiplicity=0..* */
typedef  struct
{
   const  boolean  DcmDspDidDynamicallyDefined; /*Multiplicity=1,true = DID can be dynamically defined false = DID can not bedynamically defined */
   const  boolean  DcmDspDidFixedLength;        /*Multiplicity=1,true = datalength of the DID is fixed false = datalength of the DID is variable */
   const  uint8    DcmDspDidScalingInfoSize;    /*Multiplicity=0..1,If Scaling information service is available for this DID, it provides the size of the scaling information. */
   P2CONST(Dcm_DspDidAccessType,TYPEDEF,DCM_CONST)pDcmDspDidAccess;
}Dcm_DspDidInfoType;


/*DcmDspDid,Multiplicity=0..* */
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ConditionCheckReadFncType) (P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)Nrc);
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ConditionCheckWriteFncType)(P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)Nrc);
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_GetScalingInformationFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)ScalingInfo,
		                                                                       P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ReadDataFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)Data);
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ReadDataLengthFncType)(P2VAR(uint16,AUTOMATIC,DCM_VAR)DidLength);

typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_FreezeCurrentStateFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
																			P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
																			P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlStatusRecord,
																			P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);

typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ResetToDefaultFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
																		P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
																		P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlStatusRecord,
																		P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);


typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ReturnControlToECUFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
																			P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
																			P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlStatusRecord,
																			P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);


typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_ShortTermAdjustmentFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlOptionRecord,
																			 P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlEnableMaskRecord,
																			 P2VAR(uint8,AUTOMATIC,DCM_VAR)ControlStatusRecord,
																			 P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);


typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_WriteDataFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)data,
																   uint16 dataLength,
																   P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);

typedef  struct DcmDspDid
{
   const  uint16  DcmDspDidId;             /*2 byte Identifier of the DID,Multiplicity=1*/
   const  uint16  DcmDspDidSize;           /*Length of data associated to the DID. If DID has variable datalength,that corresponds to the maximum datalength.*/
   const  Dcm_ConditionCheckReadFncType    DcmDspDidConditionCheckReadFnc; /*Function name to demand application if the conditions (e.g. System state) to read the DID are correct.Multiplicity=0..1*/
   const  Dcm_ConditionCheckWriteFncType   DcmDspDidConditionCheckWriteFnc;/*Function name to demand application if the conditions (e.g. System state) to write the DID are correc,Multiplicity=0..1*/
   const  Dcm_FreezeCurrentStateFncType    DcmDspDidFreezeCurrentStateFnc; /*Function name to request to application to freeze the current state of an IOControl,Multiplicity=0..1*/
   const  Dcm_GetScalingInformationFncType DcmDspDidGetScalingInfoFnc;     /*Function name to request to application the scaling information of the DID,Multiplicity=0..1*/
   const  Dcm_ReadDataLengthFncType        DcmDspDidReadDataLengthFnc;     /*Function name to request from application the data length of a DID*/
   const  Dcm_ReadDataFncType              DcmDspDidReadDataFnc;           /*Function name to request from application the data value of a DID*/
   const  Dcm_ResetToDefaultFncType        DcmDspDidResetToDefaultFnc;     /*Function name to request to application to reset an IOControl to default value.*/
   const  Dcm_ReturnControlToECUFncType    DcmDspDidReturnControlToECUFnc; /*Function name to request to application to return control to ECU of an IOControl*/
   const  Dcm_ShortTermAdjustmentFncType   DcmDspDidShortTermAdjustmentFnc;/*Function name to request to application to adjuste the IO signal*/
   const  Dcm_WriteDataFncType             DcmDspDidWriteFnc;              /*Function name to request application to write the data value of a DID*/
   const  uint8   DcmDspDidInfoIndex;     								   /*Reference to DcmDspDidInfo containing information on this DID*/
   const  uint8   DcmDspRefDidNum;        									/*Number Of Did referenced*/
   P2CONST(uint16,TYPEDEF,DCM_CONST)pDcmDspRefDidIdArray;					/*Reference to DcmDspDid in case this DID refer to one or serveral other DID's*/
   const  uint8   DcmDspDidControlRecordSizes;								/*Number Of DcmDspDidControlRecordSizes Container*/
   P2CONST(Dcm_DspDidControlRecordSizesType,TYPEDEF,DCM_CONST)pDcmDspDidControlRecordSizes;/*This container defines the sizes of the data sent and received with the DID control functions*/
}Dcm_DspDidType;
#endif

/*****************************************************************/
#if(STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
typedef Std_ReturnType (*Dcm_EcuResetType)(uint8 ResetType,Dcm_NegativeResponseCodeType* ErrorCode);
/*DcmDspEcuReset container,Multiplicity=0..* */
typedef  struct
{
   const  uint8  DcmEcuResetType;  /*=1:hardReset;Autosar v3.1 only support hardReset*/
   const  uint8  DcmDspEcuResetSecurityLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspEcuResetSecurityLevelRow; /*Reference to DcmDspSecurityRow */
   const  uint8  DcmDspEcuResetSessionRefNum;		/*Number Of DcmDspSessionRow referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspEcuResetSessionRow;/*Reference to DcmDspSessionRow*/
}Dcm_DspEcuResetType;
#endif

/*****************************************************************/
#if(STD_ON == DCM_DSP_PID_FUNC_ENABLED)
/*DcmDspPid container,Multiplicity=0..* */
typedef  P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_GetPidValueFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)DataValueBuffer);
typedef  struct
{
   const  uint8  DcmDspPidId;  /*2 bytes Identifier of the PID*/
   const  uint8  DcmDspPidSize;/*Length of data associated to the PID.*/
   const  Dcm_GetPidValueFncType  DcmGetPidValueFnc;/*Function name for reading value by PID,This is only relevant if DcmDspPidUsePort==FALSE.*/
}Dcm_DspPidType;
#endif

/*****************************************************************/
/*DcmDspReadDTCRow container,Multiplicity=0..* */
typedef  struct
{
   const  uint8   DcmDspDTCInfoSubFunc;        /*hex value of the sub-function.Range=0..255*/
   const  boolean DcmDspDTCInfoSubFuncSupp;    /*TRUE =sub-function supported. FALSE = sub-function not supported*/
   const  uint8   DcmDspDTCInfoSecLevelRefNum; /*Number Of DcmDspSecurityRow referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspDTCSecurityLevelRow;/*Reference to DcmDspSecurityRow,Multiplicity = 1..**/
}Dcm_DspReadDTCRowType;

/*DcmDspReadDTC container,Multiplicity=1 */
typedef  struct
{
   const  uint8  DcmDspReadDTCRowNum;     /*Number Of DcmDspReadDTCRow container*/
   P2CONST(Dcm_DspReadDTCRowType,TYPEDEF,DCM_CONST)pDcmDspReadDTCRow;/*Reference to DcmDspReadDTCRow*/
}Dcm_DspReadDTCType;

/*****************************************************************/
#if(STD_ON == DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
/*DcmDspRequestControl container,Multiplicity=0..* */
typedef  struct
{
   const  uint8  DcmDspRequestControlInBufferSize;
   const  uint8  DcmDspRequestControlOutBufferSize;
   const  uint8  DcmDspRequestControlTestId; /*Test Id for OBD Service $08*/
}Dcm_DspRequestControlType;
#endif

/*****************************************************************/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
/*DcmDspRoutine,Multiplicity=0..* */
typedef  struct
{
   const uint8   DcmDspStartRoutineCtrlOptRecSize; /*Size of optional record in the Routine Start request*/
   const uint8   DcmDspStartRoutineStsOptRecSize;  /*Size of optional record in the Routine Start response*/
}Dcm_DspStartRoutineType;

typedef  struct
{
   const uint8   DcmDspStopRoutineCtrlOptRecSize; /*Size of optional record in the Stop request*/
   const uint8   DcmDspStopRoutineStsOptRecSize;  /*Size of optional record in the Stop response*/
}Dcm_DspRoutineStopType;

typedef  struct
{
   const  uint8  DcmDspReqResRtnCtrlOptRecSize;   /*Size of optional record in the RequestResult response*/
}Dcm_DspRoutineRequestResType;

typedef  struct
{
   const  uint8  DcmDspRoutineAuthorizationSecLevelRefNum;  				/*Number Of DcmDspSecurityRow referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspRoutineAuthorizationSecLevelRow; 	/*DcmDspSecurityRow referenced*/
   const  uint8  DcmDspRoutineAuthorizationSessionRefNum;  					/*Number Of DcmDspSessionRow referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspRoutineAuthorizationSessionRow;	/*DcmDspSessionRow referenced*/
}Dcm_DspRoutineAuthorizationType;

typedef  struct
{
   P2CONST(Dcm_DspRoutineAuthorizationType,TYPEDEF,DCM_CONST)pDcmDspRoutineAuthorization;/*"reference to RoutineAuthorization"*/
   P2CONST(Dcm_DspStartRoutineType,TYPEDEF,DCM_CONST)pDcmDspStartRoutine;/*"reference to Routine Start"*/
   P2CONST(Dcm_DspRoutineStopType,TYPEDEF,DCM_CONST)pDcmDspRoutineStop;  /*"reference to Routine Stop"*/
   P2CONST(Dcm_DspRoutineRequestResType,TYPEDEF,DCM_CONST)pDcmDspRoutineRequestRes;/*"reference to RequestResult"*/
}Dcm_DspRoutineInfoType;

typedef P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_StartRoutineFncType)(P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
																	 P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
																	 P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);

typedef P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_StopRoutineFncType) (P2VAR(uint8,AUTOMATIC,DCM_VAR)InBuffer,
																	 P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
																	 P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);

typedef P2FUNC(Std_ReturnType,DCM_APPL_CODE,Dcm_RequestResultsRoutineFncType) (P2VAR(uint8,AUTOMATIC,DCM_VAR)OutBuffer,
																			   P2VAR(Dcm_NegativeResponseCodeType,AUTOMATIC,DCM_VAR)ErrorCode);
typedef  struct
{
   const  uint16   RoutineId;  /*2 bytes Identifier of the RID*/
   const  Dcm_StartRoutineFncType  DcmDspStartRoutineFnc;/*Function name for request to application to start a routine.*/
   const  Dcm_StopRoutineFncType   DcmDspRoutineStopFnc; /*Function name for request to application to stop a routine.*/
   const  Dcm_RequestResultsRoutineFncType  DcmDspRequestResultsRoutineFnc;/*Function name for request to application the results of a routine*/
   const  uint8    DcmDspRoutineInfoID;/*Reference to DcmDspRoutineInfo containing information on this routine*/
}Dcm_DspRoutineType;
#endif

/***********************************************************************/
#if(STD_ON == DCM_DSP_TEST_RESULT_BYOBDMID_FUNC_ENABLED)
/*DcmDspTestResultByObdmid container,Multiplicity=0..* */
typedef  struct
{
   const  uint8   DcmDspTestResultTestId; /*Test Id for OBD Service $06*/
   const  uint8   DcmDspTestResultUaSid;  /*Unit And Scaling ID for OBD Service $06 */
}Dcm_DspTestResultTidType;

typedef  struct
{
   const  uint8   DcmDspTestResultObdMid;  /*OBDMID for Service $06*/
   const  uint8   DcmDspTestResultObdmidTidRefNum;/*Number Of DcmDspTestResultTid */
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspTestResultTestIdArray;/*Reference to DcmDspTestResultTid */
}Dcm_DspTestResultObdmidTidType;

typedef  struct
{
   const  uint8   DcmDspTestResultObdmidTidNum;    /*Number Of ObdMid referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspTestResultObdMidTidArray;
   const  uint8   DcmDspTestResultTidNum;          /*Number Of TestId referenced*/
   P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDspTestResultTidArray;
}Dcm_DspTestResultByObdmidType;
#endif

/*********************************************************************/
#if(STD_ON == DCM_DSP_VEHINFO_FUNC_ENABLED)
/*DcmDspVehInfo Container,Multiplicity=0..* */
typedef  Std_ReturnType (*Dcm_GetInfoTypeValueFncType)(uint8 *DataValueBuffer);
typedef struct
{
   const  uint8  DcmDspVehInfoInfoType;/*INFOTYPE for Service $09 */
   const  uint8  DcmDspVehInfoSize;    /*Length of data of associated INFOTYPE.*/
   const  Dcm_GetInfoTypeValueFncType  DcmGetInfoTypeValueFnc;/*Function name for reading the associated INFOTYPE.*/
}Dcm_DspVehInfoType;
#endif

/***************************************************
 ********DcmDsp Container(Multiplicity=1)***********
 ***************************************************/
typedef  struct
{
   const  uint8  DcmDspMaxDidToRead;  /*Indicates the maximum allowed DIDs in a single "ReadDataByIdentifier" request. If set to 0, then no limitation is applied. */
   /*DcmDspDid and DcmDspDidInfo configeration(Multiplicity=0..*)*/
#if(STD_ON == DCM_DSP_DID_FUNC_ENABLED)
   const  uint8  DcmDspDidNum;       /*Number Of Did*/
   P2CONST(Dcm_DspDidType,TYPEDEF,DCM_CONST)pDcmDspDid;
   const  uint8  DcmDspDidInfoNum;   /*Number Of DidInfo*/
   P2CONST(Dcm_DspDidInfoType,TYPEDEF,DCM_CONST)pDcmDspDidInfo;
#endif

   /*EcuReset configeration(Multiplicity=0..*)*/
#if(STD_ON == DCM_DSP_ECU_RESET_FUNC_ENABLED)
   const  uint8  DcmDspEcuResetNum;
   P2CONST(Dcm_DspEcuResetType,TYPEDEF,DCM_CONST)pDcmDspEcuReset;
   const  uint8  DcmEcuResetPortNum;
   P2CONST(Dcm_EcuResetType,TYPEDEF,DCM_CONST)pDcmEcuResetPort;
#endif

   /*DcmDspPid configeration(Multiplicity=0..*)*/
#if(STD_ON == DCM_DSP_PID_FUNC_ENABLED)
   const  uint8  DcmDspPidNum;
   P2CONST(Dcm_DspPidType,TYPEDEF,DCM_CONST)pDcmDspPid;
#endif

   /*DcmDspReadDTC configeration(Multiplicity=1)*/
   P2CONST(Dcm_DspReadDTCType,TYPEDEF,DCM_CONST)pDcmDspReadDTC;
   /*DcmDspRequestControl configeration(Multiplicity=0..*)*/
#if(STD_ON ==DCM_DSP_REQUESTCONTROL_FUNC_ENABLED)
   const  uint8  DcmDspRequestControlNum;
   P2CONST(Dcm_DspRequestControlType,TYPEDEF,DCM_CONST)pDcmDspResetControl;
#endif

   /*DcmDspRoutine and DcmDspRoutineInfo configeration(Multiplicity=0..*)*/
#if(STD_ON == DCM_DSP_ROUTINE_FUNC_ENABLED)
   const  uint8  DcmDspRoutineNum;
   P2CONST(Dcm_DspRoutineType,TYPEDEF,DCM_CONST)pDcmDspRoutine;
   const  uint8  DcmDspRoutineInfoNum;
   P2CONST(Dcm_DspRoutineInfoType,TYPEDEF,DCM_CONST)pDcmDspRoutineInfo;
#endif
   /*DcmDspSecurity configeration(Multiplicity=1)*/
   P2CONST(Dcm_DspSecurityType,TYPEDEF,DCM_CONST)pDcm_DspSecurity;
   /*DcmDspSession configeration(Multiplicity=1)*/
   P2CONST(Dcm_DspSessionType,TYPEDEF,DCM_CONST)pDcm_DspSession;

   /*DcmDspTestResultByObdmid configeration(Multiplicity=0..*)*/
#if(STD_ON == DCM_DSP_TEST_RESULT_BYOBDMID_FUNC_ENABLED)
   const  uint8  DcmDspTestResultByObdmidNum;
   P2CONST(Dcm_DspTestResultByObdmidType,TYPEDEF,DCM_CONST)pDcmDspTestResultByObdmid;
#endif
   /*DcmDspVehInfo configeration*/
#if(STD_ON == DCM_DSP_VEHINFO_FUNC_ENABLED)
   const  uint8  DcmDspVehInfoNum;
   P2CONST(Dcm_DspVehInfoType,TYPEDEF,DCM_CONST)pDcmDspVehInfo;
#endif
}Dcm_DspCfgType;

/**************************************************************************
 ************************DcmDsd container********************
 ************************************************************************/
typedef  enum
{
   DCM_ADDRESSING_PHYSICAL   = 0u,	/*physical addressing*/
   DCM_ADDRESSING_FUNCTION 	 = 1u,  /*functional addressing*/
   DCM_ADDRESSING_PHYANDFUNC = 2u   /*physical and functional*/
}Dcm_DsdServiceAddressingFormatType;

/****@req DCM_CFGR_004[DCM071]****/
/*DcmDsdService container,Multiplicity=1..*,*/
typedef  struct
{
    const  uint8     DcmDsdSidTabServiceId;         /*Id of the Service identifier in hex*/
    const  boolean   DcmDsdSidTabSubfuncAvial;      /*Information whether the DcmDsdSidTabServiceId includes Subfunctions or not*/
    const  uint8     DcmDsdSidTabSecurityLevel_Num; /*Number Of Security Access Levels*/
    P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDsdSidTabSecurityLevelRef;/*Reference to DcmDspSecurityRow*/
    const  uint8     DcmDsdSidTabSessionLevel_Num;  /*Number Of  the Session Control*/
    P2CONST(uint8,TYPEDEF,DCM_CONST)pDcmDsdSidTabSessionLevelRef;
    Dcm_DsdServiceAddressingFormatType AddressingFormat;
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,Function)(uint8  ProtocolCtrlId);
}Dcm_DsdServiceCfgType;

/*DcmDsdServiceTable container,Multiplicity=1..256*/
typedef  struct
{
    const  uint8     DcmDsdSidTabId; /*service table id*/
    P2CONST(Dcm_DsdServiceCfgType,TYPEDEF,DCM_CONST)pDcmDsdService;/* reference to DcmDsdService*/
    const  uint8     DcmDsdSidTab_ServiceNum;      /*Number of the service*/
}Dcm_DsdServiceTableCfgType;

/*DcmDsd container,Multiplicity=1*/
typedef  struct
{
    P2CONST(Dcm_DsdServiceTableCfgType,TYPEDEF,DCM_CONST)pDcmDsdServiceTable;/*pointer to Service table array*/
    const  uint16     DcmDsdServiceTable_Num;  /*Number of the service table*/
}Dcm_DsdCfgType;

/**************************************************************************
 ************************DcmDsl container********************
 ************************************************************************/
/****@req DCM_CFGR_005[DCM032]****/
/*DcmDslBuffer container,Multiplicity=1..256*/
typedef  struct
{
    const  uint8    Dcm_DslBufferId;   /*buffer Id */
    const  uint16   Dcm_DslBufferSize; /*length of the buffer,Range=8~4095*/
    const  uint16   offset;
}Dcm_DslBufferType;

/*********************************/
/****@req DCM_CFGR_006[DCM679]****/
/*DslCallbackDCMRequestService container,Multiplicity=1..*,*/
typedef  struct
{
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,StartProtocol)(Dcm_ProtocolType  ProtocolID);
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,StopProtocol) (Dcm_ProtocolType  ProtocolID);
}Dcm_DslCallbackDCMRequestServiceType;

/*********************************/
/*DcmDslDiagResp container,Multiplicity =1,*/
typedef  struct
{
    const boolean  DcmDslDiagRespForceRespPendEn; /*enable (TRUE) or disable (FALSE) the mechanism of directly triggering of ResponsePending by application,Multiplicity=1*/
    const uint8    DcmDslDiagRespMaxNumRespPend;  /*Maximum number of negative responses with response code 0x78 per request,Multiplicity=1*/
}Dcm_DslDiagRespType;

/*********************************/
/****@req DCM_CFGR_010[DCM031]****/
/*Dcm_DslProtocolTimingRow container,Multiplicity=0..*,*/
typedef  struct
{
    const  uint32  DcmTimStrP2ServerMax;    	/*unit:ms*/
    const  uint32  DcmTimStrP2ServerMin;    	/*unit:ms*/
    const  uint32  DcmTimStrP2StarServerMax;	/*unit:ms*/
    const  uint32  DcmTimStrP2StarServerMin;	/*unit:ms*/
    const  uint32  DcmTimStrS3Server;   		/*unit:ms*/
}Dcm_DslProtocolTimingRowType;

/*Dcm_DslProtocolTiming container,Multiplicity=1*/
typedef  struct
{
    P2CONST(Dcm_DslProtocolTimingRowType,TYPEDEF,DCM_CONST)pDcm_DslProtocolTimingRow;/*Multiplicity=0..*,*/
    const  uint8    Dcm_DslProtocolTimingRowNum; /*Number Of DcmDslProtocolTimingRow*/
}Dcm_DslProtocolTimingType;

/*********************************/
/****@req DCM_CFGR_011[DCM681]****/
/*DcmDslServiceRequestIndication container,Multiplicity=0..*,*/
typedef  struct
{
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,Indication)(uint8 SID,
    		                                        P2VAR(uint8,AUTOMATIC,DCM_VAR)RequestData,
    		                                        uint16 DataSize);
}Dcm_ServiceRequestIndicationType;

/*********************************/
/****@req DCM_CFGR_012[DCM680]****/
/*Dcm_DslSessionControl container,Multiplicity=1..*,*/
typedef  struct
{
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,ChangeIndication)(Dcm_SesType SesCtrlTypeOld, Dcm_SesType SesCtrlTypeNew);
    P2FUNC(Std_ReturnType,DCM_APPL_CODE,GetSesChgPermission)(Dcm_SesType SesCtrlTypeActive, Dcm_SesType SesCtrlTypeNew);
}Dcm_DslSessionControlType;

/*********************************/
/*DcmDslPeriodicTransmission container,Multiplicity=0..1*/
typedef  struct
{
    P2CONST(PduIdType,TYPEDEF,DCM_CONST)pDcmPeriodicTxPduId;
    const  uint8  DcmPeriodicTxPduIdNum;
}Dcm_DslPeriodicTransmissionType;

/*DcmDslResponseOnEvent container,Multiplicity=0..1*/
typedef  struct
{
    const  PduIdType  DcmROETxPduId;
    const  uint8  DcmROETxPduIdNum;
}Dcm_DslResponseOnEventType;

typedef  enum
{
    DCM_PHYSICAL    = 0,
    DCM_FUNCTIONAL  = 1
}Dcm_DslProtocolRxAddrType;

/*Dcm_DslProtocolRx container,Multiplicity=1..*,*/
typedef  struct
{
    const Dcm_DslProtocolRxAddrType  DcmDslProtocolRxAddrType;
    const PduIdType  DcmDslProtocolRxPduId;
}Dcm_DslProtocolRxType;

/*DcmDslMainConnection container,Multiplicity=1*/
typedef  struct
{
    P2CONST(Dcm_DslPeriodicTransmissionType,TYPEDEF,DCM_CONST)pDcmDslPeriodicTranmissionConRef;/*Reference to DcmDslPeriodicTransmission,Multiplicity=0..1,*/
    P2CONST(Dcm_DslResponseOnEventType,TYPEDEF,DCM_CONST)pDcmDslROEConnectionRef;/*Reference to DcmDslResponseOnEvent,Multiplicity=0..1,*/
    P2CONST(Dcm_DslProtocolRxType,TYPEDEF,DCM_CONST)pDcmDslProtocolRx;			 /*Reference to DcmDslProtocolRx,Multiplicity=1..*,*/
    const  uint8  	  DcmDslProtocolRx_Num;             					     /*Number Of DcmDslProtocolRx*/
    /****@req DCM_CFGR_007[DCM683]****/
    const  PduIdType  DcmDslProtocolTxPduId;            						 /*Use to transmission of responses,Multiplicity=1,*/
}Dcm_DslMainConnectionType;

/*Dcm_DslConnection,Multiplicity=1..*,*/
typedef  struct
{
    P2CONST(Dcm_DslMainConnectionType,TYPEDEF,DCM_CONST)pDcmDslMainConnection;/*Reference to DcmDslMainConnection,Multiplicity=1,*/
    /****@req DCM_CFGR_008[DCM070]****/
    P2CONST(Dcm_DslPeriodicTransmissionType,TYPEDEF,DCM_CONST)pDcmDslPeriodicTransmission;/*Reference to DcmDslPeriodicTransmission,Multiplicity=0..1,*/
    /****@req DCM_CFGR_009[DCM069]****/
    P2CONST(Dcm_DslResponseOnEventType,TYPEDEF,DCM_CONST)pDcmDslResponseOnEvent; /*Reference to DcmDslResponseOnEvent,Multiplicity=0..1*/
}Dcm_DslConnectionType;

typedef  enum
{
    DCM_PROTOCOL_TRAN_TYPE1 = 0x00,  /*Messages on the DcmTxPduId already used for normal diagnostic responses*/
    DCM_PROTOCOL_TRAN_TYPE2 = 0x01   /*Messages on a separate DcmTxPduId.*/
}Dcm_DslProtocolTransType;

/*DcmDslProtocolRow container,Multiplicity=1..*,*/
typedef  struct
{
    const Dcm_ProtocolType  DcmDslProtocolID;                /*Protocol Id,Multiplicity=1*/
    const boolean           DcmDslProtocolIsParallelExecutab;/*Enables the parallel processing of ROE or Periodic Transmission protocol,Multiplicity=1*/
    const uint32            DcmDslProtocolPreemptTime;       /*This is the value for the timeout (in ms) of preempting protocol until protocol needs to be started,Multiplicity=1,*/
    const uint8             DcmDslProtocolPriority;          /*Protocol Priority,Multiplicity=1*/
    const Dcm_DslProtocolTransType  DcmDslProtocolTransType; /*the transmission type of ROE or Periodic,Multiplicity=1,*/
    const uint8             DcmDslProtocolRxBufferID;        /*Link to buffer configuration (DcmDslBufferID),Multiplicity=1,*/
    const uint8             DcmDslProtocolTxBufferID;        /*TxbufferID,Multiplicity=1,*/
    /****@req DCM-FUNR-040[DCM035]****/
    const uint8             DcmDslServiceTableID;            /*Reference to DcmDsdServiceTable,Multiplicity=1,*/
    P2CONST(Dcm_DslProtocolTimingRowType,TYPEDEF,DCM_CONST)pDcmDslProtocolTimeLimit;/*Link to the extended / limit timing structure. Note: limits are checked when using AccessTimingParameter service.Multiplicity=0..1,*/
    P2CONST(Dcm_DslConnectionType,TYPEDEF,DCM_CONST)pDcmDslConnection;
    const uint8             DcmDslConnection_Num;  /*Number of connection*/
}Dcm_DslProtocolRowType;

/*Dcm_DslProtocol container,Multiplicity=1,*/
typedef  struct
{
    P2CONST(Dcm_DslProtocolRowType,TYPEDEF,DCM_CONST)pDcmDslProtocolRow;
    const uint8  DcmDslProtocolRow_Num;/*Number of Protocol*/
}Dcm_DslProtocolType;

/*****************************/
/*DcmDsl container,Multiplicity=1*/
typedef  struct
{
  const  uint8  DcmChannelCfg_Num; 					/*Number of Channel configration*/
  P2CONST(Dcm_DslBufferType,TYPEDEF,DCM_CONST)pDcmChannelCfg;

  const  uint8  DCMCallBackDcmRequest_PortNum; 		/*Number of DslCallbackDCMRequestService port*/
  P2CONST(Dcm_DslCallbackDCMRequestServiceType,TYPEDEF,DCM_CONST)pDcmDslCallback_DCMRequestService;

  const  uint8  DcmServiceRequestIndication_PortNum;/*Number of ServiceRequestIndication port*/
  P2CONST(Dcm_ServiceRequestIndicationType,TYPEDEF,DCM_CONST)pDcmServiceRequestIndication;

  const  uint8  DcmDslSessionControl_PortNum;		/*Number of SessionControl port*/
  P2CONST(Dcm_DslSessionControlType,TYPEDEF,DCM_CONST)pDcmDslSessionControl;

  P2CONST(Dcm_DslDiagRespType,TYPEDEF,DCM_CONST)pDcmDslDiagRespCfg;/*reference to DcmDslDiagResp configration*/
  P2CONST(Dcm_DslProtocolType,TYPEDEF,DCM_CONST)pDcmDslProtocol;   /*reference to DcmDslProtocol configration*/
  P2CONST(Dcm_DslProtocolTimingType,TYPEDEF,DCM_CONST)pDcmDslProtocolTiming;
}Dcm_DslCfgType;


#endif /* DCM_CFGTYPE_H_ */
