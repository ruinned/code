// BmsSysSt.h
#ifndef BMSSYSST_H
#define BMSSYSST_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

typedef struct
{
    uByte  PowerOffTime[6];
    uWord  ErrPowerOffTimes;
    uByte  ResetOrPowoffType;
} sBms_PowerOffType;

typedef struct
{
    uByte  WorkSt;
    uByte  SynchSt;
    uByte  AwakeSt;

    uByte  DiagnoseProtectSt;

    uByte  SystemTime[6];
    uLWord StandByTime;
    uLWord ServiceTime;
    sBms_PowerOffType Bms_PowerOff;
  
    uByte  K30St;
    uWord  K30Volt;
    uByte  K15St;
    uWord  K15Volt;
    uByte  OnBdChgSt;
    uWord  OnBdChgVolt;
    uByte  OffBdChgSt;
    uWord  OffBdChgVolt;
    uWord  AlarmVolt;
    uByte  AlarmSt;
    uWord  CanInhVolt;

    uByte  ProtocalCompatibSt;
    uByte  SoftWareCompatibSt;
    uByte  ParameterConfigSt;
    uByte  SoftWareUpdateSt;

} sBms_SysOperationStType;

typedef struct
{
    uByte  VoltDetModuleSt;
    uByte  DchBalModuleSt;
    uByte  ChgBalModuleSt;
    uByte  TemperSensorType;
    uByte  TemperDetModuleSt;
    uByte  ExtSumvDetModuleSt;
    uByte  IntSumvDetModueleSt;
    uByte  CurrentSensorType;
    uByte  CurrentDetModuleSt;
    uByte  IsoDetModuleSt;
    uByte  ClockSt;
    uByte  CpuEESt;
    uByte  ExtEESt;
    uByte  ExtFlashSt;
} sBms_HardWareModuleStType;

typedef struct
{
    uByte  RecPageMode;
    uByte  ErrRecFullSt;
    uByte  HisRecFullSt;
    uWord  ErrRecCurrentPage;
    uWord  HisRecCurrentPage;
    
    uWord  HisRecStartPage;
    uWord  HisRecEndPage;
    uWord  ErrRecStartPage;
    uWord  ErrRecEndPage;
} sBms_DataRecordStType;

typedef struct
{
    uByte  IntCanBusSt;
    uByte  ChgCanBusSt;
    uByte  CarCanBusSt;
    uByte  IntCanProtocalVer[32];
    uWord  BusLoadRate;
} sBms_CanBusStType;

typedef struct
{
   uWord Ai1St ;
}sBmsAiStType;

typedef struct
{
   uByte Di1St  : 2; 
   uByte Di2St  : 2;
   uByte Di3St  : 2; 
   uByte Di4St  : 2;
}sBmsDiStType;
  
typedef struct
{
   uByte Do1St  ;    
}sBmsDoStType;   

typedef struct
{
    sBms_SysOperationStType     Bms_OperationSt;
    sBms_HardWareModuleStType   Bms_HardWareModuleSt;
    sBms_DataRecordStType       Bms_DataRecordSt;
    sBms_CanBusStType           Bms_CanBusSt;

    sBmsAiStType BmuAiSt[MAX_BMU_NUM];
    sBmsDiStType BmuDiSt[MAX_BMU_NUM];
    sBmsDoStType BmuDoSt[MAX_BMU_NUM];
} sBms_SysStInfoType;

typedef enum
{
    BMSSYSST_ERR_UPDATE  =  ERR_DATA_SYSTEMST_BASE + 15,
    BMSSYSST_ERR_GET,
    BMSSYSST_ERR_GETTIME,
} eBmsSt_ErrCodeType;


extern void Bms_SysStInit(void);

GeneralReturnType BmsSys_CalcStandbyTime(uLWord * standby_time);

extern uByte BmsSys_GetWorkSt(void);
extern void BmsSys_UpdateWorkSt(uByte status);

extern uByte BmsSys_GetSynchSt(void);
extern void BmsSys_UpdateSynchSt(uByte status);

extern uByte BmsSys_GetProtocalCompatibSt(void);
extern void BmsSys_UpdateProtocalCompatibSt(uByte status);

extern uByte BmsSys_GetSoftWareCompatibSt(void);
extern void BmsSys_UpdateSoftWareCompatibSt(uByte status);

extern uByte BmsSys_GetAwakeSt(void);
extern void BmsSys_UpdateAwakeSt(uByte status);

extern uByte BmsSys_GetK15St(void);
extern void BmsSys_UpdateK15St(uByte status);

extern uWord BmsSys_GetK15Volt(void);
extern void BmsSys_UpdateK15Volt(uWord volt);

extern uByte BmsSys_GetK30St(void);
extern void BmsSys_UpdateK30St(uByte status);

extern uWord BmsSys_GetK30Volt(void);
extern void BmsSys_UpdateK30Volt(uWord volt);

extern uWord BmsSys_GetOnBDChgVolt(void);
extern void BmsSys_UpdateOnBDChgVolt(uWord volt);

extern uByte BmsSys_GetOnBDChgSt(void);
extern void BmsSys_UpdateOnBDChgSt(uByte St);

extern uWord BmsSys_GetOffBDChgVolt(void);
extern void BmsSys_UpdateOffBDChgVolt(uWord volt);

extern uByte BmsSys_GetOffBDChgSt(void);
extern void BmsSys_UpdateOffBDChgSt(uByte St);


extern uWord BmsSys_GetAlarmVolt(void);
extern void BmsSys_UpdateAlarmVolt(uWord volt);

extern uByte BmsSys_GetAlarmSt(void);
extern void BmsSys_UpdateAlarmSt(uByte St);

extern uWord BmsSys_GetCanInhVolt(void);
extern void BmsSys_UpdateCanInhVolt(uWord volt);

extern Word BmsSys_GetBmuAi1St(uByte bmu_no);
extern void BmsSys_UpdateBmuAi1St(uByte bmu_no, uWord status);


extern uByte BmsSys_GetBmuDi1St(uByte bmu_no);
extern void BmsSys_UpdateBmuDi1St(uByte bmu_no,uByte status);

extern uByte BmsSys_GetBmuDi2St(uByte bmu_no);
extern void BmsSys_UpdateBmuDi2St(uByte bmu_no,uByte status);

extern uByte BmsSys_GetBmuDi3St(uByte bmu_no);
extern void BmsSys_UpdateBmuDi3St(uByte bmu_no,uByte status);

extern uByte BmsSys_GetBmuDi4St(uByte bmu_no);
extern void BmsSys_UpdateBmuDi4St(uByte bmu_no,uByte status);

extern uByte BmsSys_GetBmuDo1St(uByte bmu_no);
extern void BmsSys_UpdateBmuDo1St(uByte bmu_no,uByte status);

extern uWord BmsSys_GetSystemTime(uByte* p_time); 

extern void BmsSys_GetPowerOffTime(uByte* p_time);
extern GeneralReturnType BmsSys_UpdatePowerOffTime(void);

extern uLWord BmsSys_GetServiceTime(void);
extern void BmsSys_UpdateServiceTime(uLWord time);

extern uWord BmsSys_GetUnusualPowerOffCounter(void);
extern void BmsSys_UpdateUnusualPowerOffCounter(uWord cnt);

extern uByte BmsSys_GetParameterConfigSt(void);
extern void BmsSys_UpdateParameterConfigSt(uByte status);

extern uByte BmsSys_GetDiagnoseProtectSt(void);
extern void BmsSys_UpdateDiagnoseProtectSt(uByte status);

extern uByte BmsSys_GetSoftWareUpdateSt(void);
extern void BmsSys_UpdateSoftWareUpdateSt(uByte status);

extern uByte BmsSys_GetVoltageDetectModuleSt(void);
extern void BmsSys_UpdateVoltageDetectModuleSt(uByte status);

extern uByte BmsSys_GetDchgBalanceModuleSt(void);
extern void BmsSys_UpdateDchgBalanceModuleSt(uByte status);

extern uByte BmsSys_GetChgBalanceModuleSt(void);
extern void BmsSys_UpdateChgBalanceModuleSt(uByte status);

extern uByte BmsSys_GetTemperSensorType(void);
extern void BmsSys_UpdateTemperSensorType(uByte type);

extern uByte BmsSys_GetTemperDetectModuleSt(void);
extern void BmsSys_UpdateTemperDetectModuleSt(uByte status);

extern uByte BmsSys_GetExternSumvDetectModuleSt(void);
extern void BmsSys_UpdateExternSumvDetectModuleSt(uByte status);

extern uByte BmsSys_GetIntSumvDetectModueleSt(void);
extern void BmsSys_UpdateIntSumvDetectModueleSt(uByte status);

extern uByte BmsSys_GetCurrentSensorType(void);
extern void BmsSys_UpdateCurrentSensorType(uByte type);

extern uByte BmsSys_GetCurrentDetectModuleSt(void);
extern void BmsSys_UpdateCurrentDetectModuleSt(uByte status);

extern uByte BmsSys_GetIsoDetectModuleSt(void);
extern void BmsSys_UpdateIsoDetectModuleSt(uByte status);

extern uByte BmsSys_GetIntEeSt(void);
extern void BmsSys_UpdateIntEeSt(uByte status);

extern uByte BmsSys_GetExtEeSt(void);
extern void BmsSys_UpdateExtEeSt(uByte status);

extern uByte BmsSys_GetExtFlashSt(void);
extern void BmsSys_UpdateExtFlashSt(uByte status);

extern uByte BmsSys_GetClockSt(void);
extern void BmsSys_UpdateClockSt(uByte status);

extern void BmsSys_UpdateRecPageMode(uByte page_mode);
extern uByte BmsSys_GetRecPageMode(void);

extern uByte BmsSys_GetHisRecFullSt(void);
extern void BmsSys_UpdateHisRecFullSt(uByte status);

extern uWord BmsSys_GetHisRecCurrentPage(void);
extern void BmsSys_UpdateHisRecCurrentPage(uWord page);

extern uWord BmsSys_GetHisRecStartPage(void);
extern void BmsSys_UpdateHisRecStartPage(uWord page);

extern uWord BmsSys_GetHisRecEndPage(void);
extern void BmsSys_UpdateHisRecEndPage(uWord page);

extern uByte BmsSys_GetErrRecFullSt(void);
extern void BmsSys_UpdateErrRecFullSt(uByte status);

extern uWord BmsSys_GetErrRecStartPage(void);
extern void BmsSys_UpdateErrRecStartPage(uWord page);

extern uWord BmsSys_GetErrRecCurrentPage(void);
extern void BmsSys_UpdateErrRecCurrentPage(uWord page);

extern uWord BmsSys_GetErrRecEndPage(void);
extern void BmsSys_UpdateErrRecEndPage(uWord page);

extern uByte BmsSys_GetIntCanBusSt(void);
extern void BmsSys_UpdateIntCanBusSt(uByte status);

extern uByte BmsSys_GetChgCanBusSt(void);
extern void BmsSys_UpdateChgCanBusSt(uByte status);

extern uByte BmsSys_GetCarCanBusSt(void);
extern void BmsSys_UpdateCarCanBusSt(uByte status);

extern uByte BmsSys_GetIntCanProtocalVer(uByte bcnt);
extern void BmsSys_UpdateIntCanProtocalVer(uByte * RAMPTR version);

extern uWord BmsSys_GetBusLoadRate(void);
extern void BmsSys_UpdateBusLoadRate(uByte rate);

#endif  /* BMSSYSST_H */