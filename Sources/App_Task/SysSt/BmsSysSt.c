#include "BmsSysSt.h"
//#include "Ecu.h"
#include "SysConf.h"
#include "EE.h"

#include "ErrorAlarm.h"

#include "ClockIf.h"

#ifdef ECU_M_ENABLE_XGATE
#include "Xgate_RealTime.h"
#endif


#if(ECU_RAM_USE_MODE)
#ifdef ECU_RAM_USE_GPGAE
#pragma DATA_SEG __GPAGE_SEG PAGED_RAM
#endif
#ifdef ECU_RAM_USE_RPGAE
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM
#endif
#else
#pragma DATA_SEG DEFAULT
#endif

sBms_SysStInfoType  Bms_SysStInfo;
const uByte gby_DayPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void Bms_SysStInit(void)
{
    uByte *RAMPTR pByte;
    uWord wcnt;

    pByte = (uByte *RAMPTR)&Bms_SysStInfo;
    for(wcnt = 0; wcnt < sizeof(Bms_SysStInfo); wcnt++)
    {
        *(pByte++) = 0;
    }

    (void)SysConf_InitParaTable(SYSTEMST_POWEROFF_RESET_INFO, (uLWord)(&(Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff)), sizeof(Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff));

    (void)EE_LoadVar(SYSTEMST_POWEROFF_RESET_INFO);

    (void)SysConf_InitParaTable(SYSTEMST_BMS_TOTAL_RUNTIME, (uLWord)(&(Bms_SysStInfo.Bms_OperationSt.ServiceTime)), sizeof(Bms_SysStInfo.Bms_OperationSt.ServiceTime));
    (void)EE_LoadVar(SYSTEMST_BMS_TOTAL_RUNTIME);

}

uByte BmsSys_GetWorkSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.WorkSt;
}

void BmsSys_UpdateWorkSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.WorkSt = status;
}

uByte BmsSys_GetSynchSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.SynchSt;
}

void BmsSys_UpdateSynchSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.SynchSt = status;
}

uByte BmsSys_GetProtocalCompatibSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.ProtocalCompatibSt;
}

void BmsSys_UpdateProtocalCompatibSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.ProtocalCompatibSt = status;
}

uByte BmsSys_GetSoftWareCompatibSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.SoftWareCompatibSt;
}

void BmsSys_UpdateSoftWareCompatibSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.SoftWareCompatibSt = status;
}


Word BmsSys_GetBmuAi1St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuAiSt[bmu_no].Ai1St;
}

void BmsSys_UpdateBmuAi1St(uByte bmu_no, uWord status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuAiSt[bmu_no].Ai1St = status;
}


uByte BmsSys_GetBmuDi1St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuDiSt[bmu_no].Di1St;
}

void BmsSys_UpdateBmuDi1St(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuDiSt[bmu_no].Di1St = status;
}

uByte BmsSys_GetBmuDi2St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuDiSt[bmu_no].Di2St;
}

void BmsSys_UpdateBmuDi2St(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuDiSt[bmu_no].Di2St = status;
}

uByte BmsSys_GetBmuDi3St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuDiSt[bmu_no].Di3St;
}

void BmsSys_UpdateBmuDi3St(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuDiSt[bmu_no].Di3St = status;
}


uByte BmsSys_GetBmuDi4St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuDiSt[bmu_no].Di4St;
}

void BmsSys_UpdateBmuDi4St(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuDiSt[bmu_no].Di4St = status;
}


uByte BmsSys_GetBmuDo1St(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
   return Bms_SysStInfo.BmuDoSt[bmu_no].Do1St;
}

void BmsSys_UpdateBmuDo1St(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    Bms_SysStInfo.BmuDoSt[bmu_no].Do1St = status;
}


uByte BmsSys_GetAwakeSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.AwakeSt;
}

void BmsSys_UpdateAwakeSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.AwakeSt = status;
}

uByte BmsSys_GetK15St(void)
{
    return Bms_SysStInfo.Bms_OperationSt.K15St;
}

void BmsSys_UpdateK15St(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.K15St = status;
}

uWord BmsSys_GetK15Volt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.K15Volt;
}

void BmsSys_UpdateK15Volt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.K15Volt = volt;
}

uByte BmsSys_GetK30St(void)
{
    return Bms_SysStInfo.Bms_OperationSt.K30St;
}

void BmsSys_UpdateK30St(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.K30St = status;
}

uWord BmsSys_GetK30Volt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.K30Volt;
}

void BmsSys_UpdateK30Volt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.K30Volt = volt;
}

uWord BmsSys_GetOnBDChgVolt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.OnBdChgVolt;
}

void BmsSys_UpdateOnBDChgVolt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.OnBdChgVolt = volt;
}

uByte BmsSys_GetOnBDChgSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.OnBdChgSt;
}

void BmsSys_UpdateOnBDChgSt(uByte St)
{
    Bms_SysStInfo.Bms_OperationSt.OnBdChgSt = St;
}

uWord BmsSys_GetOffBDChgVolt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.OffBdChgVolt;
}

void BmsSys_UpdateOffBDChgVolt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.OffBdChgVolt = volt;
}

uByte BmsSys_GetOffBDChgSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.OffBdChgSt;
}

void BmsSys_UpdateOffBDChgSt(uByte St)
{
    Bms_SysStInfo.Bms_OperationSt.OffBdChgSt = St;
}

uWord BmsSys_GetAlarmVolt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.AlarmVolt;
}

void BmsSys_UpdateAlarmVolt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.AlarmVolt = volt;
}

uByte BmsSys_GetAlarmSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.AlarmSt;
}

void BmsSys_UpdateAlarmSt(uByte st)
{
    Bms_SysStInfo.Bms_OperationSt.AlarmSt = st;
}

uWord BmsSys_GetCanInhVolt(void)
{
   return Bms_SysStInfo.Bms_OperationSt.CanInhVolt;
}

void BmsSys_UpdateCanInhVolt(uWord volt)
{
    Bms_SysStInfo.Bms_OperationSt.CanInhVolt = volt;
}


GeneralReturnType BmsSys_GetSystemTime(uByte* p_time)
{

#ifdef ECU_M_ENABLE_XGATE
    sRealTimeType  time;
    GeneralReturnType rslt;
    
    Xgate_CpuGetRealTime(&time, &rslt);  
    *p_time++ = time.year;
    *p_time++ = time.month;
    *p_time++ = time.day;
    *p_time++ = time.hour;
    *p_time++ = time.minute;
    *p_time++ = time.second;

    Bms_SysStInfo.Bms_OperationSt.SystemTime[0] = time.year;
    Bms_SysStInfo.Bms_OperationSt.SystemTime[1] = time.month;
    Bms_SysStInfo.Bms_OperationSt.SystemTime[2] = time.day;
    Bms_SysStInfo.Bms_OperationSt.SystemTime[3] = time.hour;
    Bms_SysStInfo.Bms_OperationSt.SystemTime[4] = time.minute;
    Bms_SysStInfo.Bms_OperationSt.SystemTime[5] = time.second;
    
    (void)Err_UpdateHwErrLevel(ERR_HW_CLOCK_EXT, rslt);

    return rslt;
#else 
    sClockIf_TimeType time;
    if(!ClockIf_ReadTime(&time))
    {
        *p_time++ = time.year;
        *p_time++ = time.month;
        *p_time++ = time.day;
        *p_time++ = time.hour;
        *p_time++ = time.minute;
        *p_time++ = time.second;

        Bms_SysStInfo.Bms_OperationSt.SystemTime[0] = time.year;
        Bms_SysStInfo.Bms_OperationSt.SystemTime[1] = time.month;
        Bms_SysStInfo.Bms_OperationSt.SystemTime[2] = time.day;
        Bms_SysStInfo.Bms_OperationSt.SystemTime[3] = time.hour;
        Bms_SysStInfo.Bms_OperationSt.SystemTime[4] = time.minute;
        Bms_SysStInfo.Bms_OperationSt.SystemTime[5] = time.second;

        return ERR_OK;
    }
    else
    {
        return (GeneralReturnType)BMSSYSST_ERR_GETTIME;
    }
#endif        
}

void BmsSys_GetPowerOffTime(uByte* p_time)
{
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[0];
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[1];
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[2];
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[3];
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[4];
    *p_time++ = Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[5];
}

GeneralReturnType BmsSys_UpdatePowerOffTime(void)
{   
#ifdef ECU_M_ENABLE_XGATE
    sRealTimeType  time;
    GeneralReturnType rslt;
    
    Xgate_CpuGetRealTime(&time, &rslt);  
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[0] = time.year;
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[1] = time.month;
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[2] = time.day;
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[3] = time.hour;
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[4] = time.minute;
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[5] = time.second; 
     
     (void)Err_UpdateHwErrLevel(ERR_HW_CLOCK_EXT, rslt);   
     return rslt;
#else
    sClockIf_TimeType time;  
    if(!ClockIf_ReadTime(&time))
    {
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[0] = time.year;
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[1] = time.month;
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[2] = time.day;
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[3] = time.hour;
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[4] = time.minute;
        Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.PowerOffTime[5] = time.second;

        return ERR_OK;
    }
    else
    {
        return (GeneralReturnType)BMSSYSST_ERR_GETTIME;
    }
#endif    
}

uLWord BmsSys_GetServiceTime(void)
{
    return Bms_SysStInfo.Bms_OperationSt.ServiceTime;
}

void BmsSys_UpdateServiceTime(uLWord time)
{
    Bms_SysStInfo.Bms_OperationSt.ServiceTime = time;
}


uWord BmsSys_GetUnusualPowerOffCounter(void)
{
    return Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.ErrPowerOffTimes;
}

void BmsSys_UpdateUnusualPowerOffCounter(uWord cnt)
{
    Bms_SysStInfo.Bms_OperationSt.Bms_PowerOff.ErrPowerOffTimes = cnt;
}

uByte BmsSys_GetParameterConfigSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.ParameterConfigSt;
}

void BmsSys_UpdateParameterConfigSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.ParameterConfigSt = status;
}

uByte BmsSys_GetDiagnoseProtectSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.DiagnoseProtectSt;
}

void BmsSys_UpdateDiagnoseProtectSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.DiagnoseProtectSt = status;
}

uByte BmsSys_GetSoftWareUpdateSt(void)
{
    return Bms_SysStInfo.Bms_OperationSt.SoftWareUpdateSt;
}

void BmsSys_UpdateSoftWareUpdateSt(uByte status)
{
    Bms_SysStInfo.Bms_OperationSt.SoftWareUpdateSt = status;
}

uByte BmsSys_GetVoltageDetectModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.VoltDetModuleSt;
}

void BmsSys_UpdateVoltageDetectModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.VoltDetModuleSt = status;
}

uByte BmsSys_GetDchgBalanceModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.DchBalModuleSt;
}

void BmsSys_UpdateDchgBalanceModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.DchBalModuleSt = status;
}

uByte BmsSys_GetChgBalanceModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.ChgBalModuleSt;
}

void BmsSys_UpdateChgBalanceModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.ChgBalModuleSt = status;
}

uByte BmsSys_GetTemperSensorType(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.TemperSensorType;
}

void BmsSys_UpdateTemperSensorType(uByte type)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.TemperSensorType = type;
}

uByte BmsSys_GetTemperDetectModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.TemperDetModuleSt;
}

void BmsSys_UpdateTemperDetectModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.TemperDetModuleSt = status;
}

uByte BmsSys_GetExternSumvDetectModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.ExtSumvDetModuleSt;
}

void BmsSys_UpdateExternSumvDetectModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.ExtSumvDetModuleSt = status;
}

uByte BmsSys_GetIntSumvDetectModueleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.IntSumvDetModueleSt;
}

void BmsSys_UpdateIntSumvDetectModueleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.IntSumvDetModueleSt = status;
}

uByte BmsSys_GetCurrentSensorType(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.CurrentSensorType;
}

void BmsSys_UpdateCurrentSensorType(uByte type)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.CurrentSensorType = type;
}

uByte BmsSys_GetCurrentDetectModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.CurrentDetModuleSt;
}

void BmsSys_UpdateCurrentDetectModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.CurrentDetModuleSt = status;
}

uByte BmsSys_GetIsoDetectModuleSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.IsoDetModuleSt;
}

void BmsSys_UpdateIsoDetectModuleSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.IsoDetModuleSt = status;
}

uByte BmsSys_GetIntEeSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.CpuEESt;
}

void BmsSys_UpdateIntEeSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.CpuEESt = status;
}

uByte BmsSys_GetExtEeSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.ExtEESt;
}

void BmsSys_UpdateExtEeSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.ExtEESt = status;
}

uByte BmsSys_GetExtFlashSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.ExtFlashSt;
}

void BmsSys_UpdateExtFlashSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.ExtFlashSt = status;
}

uByte BmsSys_GetClockSt(void)
{
    return Bms_SysStInfo.Bms_HardWareModuleSt.ClockSt;
}

void BmsSys_UpdateClockSt(uByte status)
{
    Bms_SysStInfo.Bms_HardWareModuleSt.ClockSt = status;
}

uByte BmsSys_GetHisRecFullSt(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.HisRecFullSt;
}

void BmsSys_UpdateRecPageMode(uByte page_mode)
{
    Bms_SysStInfo.Bms_DataRecordSt.RecPageMode = page_mode;
}

uByte BmsSys_GetRecPageMode(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.RecPageMode;
}

void BmsSys_UpdateHisRecFullSt(uByte status)
{
    Bms_SysStInfo.Bms_DataRecordSt.HisRecFullSt = status;
}

uWord BmsSys_GetHisRecCurrentPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.HisRecCurrentPage;
}

void BmsSys_UpdateHisRecCurrentPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.HisRecCurrentPage = page;
}

uWord BmsSys_GetHisRecStartPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.HisRecStartPage;
}

void BmsSys_UpdateHisRecStartPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.HisRecStartPage = page;
}

uWord BmsSys_GetHisRecEndPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.HisRecEndPage;
}

void BmsSys_UpdateHisRecEndPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.HisRecEndPage = page;
}

uByte BmsSys_GetErrRecFullSt(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.ErrRecFullSt;
}

void BmsSys_UpdateErrRecFullSt(uByte status)
{
    Bms_SysStInfo.Bms_DataRecordSt.ErrRecFullSt = status;
}

uWord BmsSys_GetErrRecStartPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.ErrRecStartPage;
}

void BmsSys_UpdateErrRecStartPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.ErrRecStartPage = page;
}

uWord BmsSys_GetErrRecCurrentPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.ErrRecCurrentPage;
}

void BmsSys_UpdateErrRecCurrentPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.ErrRecCurrentPage = page;
}

uWord BmsSys_GetErrRecEndPage(void)
{
    return Bms_SysStInfo.Bms_DataRecordSt.ErrRecEndPage;
}

void BmsSys_UpdateErrRecEndPage(uWord page)
{
    Bms_SysStInfo.Bms_DataRecordSt.ErrRecEndPage = page;
}

uByte BmsSys_GetIntCanBusSt(void)
{
    return Bms_SysStInfo.Bms_CanBusSt.IntCanBusSt;
}

void BmsSys_UpdateIntCanBusSt(uByte status)
{
    Bms_SysStInfo.Bms_CanBusSt.IntCanBusSt = status;
}

uByte BmsSys_GetChgCanBusSt(void)
{
    return Bms_SysStInfo.Bms_CanBusSt.ChgCanBusSt;
}

void BmsSys_UpdateChgCanBusSt(uByte status)
{
    Bms_SysStInfo.Bms_CanBusSt.ChgCanBusSt = status;
}

uByte BmsSys_GetCarCanBusSt(void)
{
    return Bms_SysStInfo.Bms_CanBusSt.CarCanBusSt;
}

void BmsSys_UpdateCarCanBusSt(uByte status)
{
    Bms_SysStInfo.Bms_CanBusSt.CarCanBusSt = status;
}

uByte BmsSys_GetIntCanProtocalVer(uByte bcnt)
{
    return Bms_SysStInfo.Bms_CanBusSt.IntCanProtocalVer[bcnt];
}

void BmsSys_UpdateIntCanProtocalVer(uByte *RAMPTR version)
{
    uByte i;
    for(i = 0; i < sizeof(Bms_SysStInfo.Bms_CanBusSt.IntCanProtocalVer); i++)
    {
        Bms_SysStInfo.Bms_CanBusSt.IntCanProtocalVer[i] = *(version++);
    }
}

uWord BmsSys_GetBusLoadRate(void)
{
    return Bms_SysStInfo.Bms_CanBusSt.BusLoadRate;
}

void BmsSys_UpdateBusLoadRate(uByte rate)
{
    Bms_SysStInfo.Bms_CanBusSt.BusLoadRate = rate;
}

GeneralReturnType BmsSys_CalcStandbyTime(uLWord * standby_time) // unit 1s
{
    uByte i;
    uLWord llwtmp;
    uLWord llwtmp_bak;    
    sClockIf_TimeType pow_off_time;

#ifdef ECU_M_ENABLE_XGATE
    sRealTimeType  pow_on_time;
    GeneralReturnType rslt = 0;  

    Xgate_CpuGetRealTime(&pow_on_time, &rslt);
    if(rslt)
    {
      (void)Err_UpdateHwErrLevel(ERR_HW_CLOCK_EXT, rslt);
       return (GeneralReturnType)BMSSYSST_ERR_GETTIME;
    }  
#else    
    sClockIf_TimeType pow_on_time;

    if(ClockIf_ReadTime(&pow_on_time))
    {
        return (GeneralReturnType)BMSSYSST_ERR_GETTIME;
    }

#endif    
    BmsSys_GetPowerOffTime((uByte*)(&pow_off_time));

    if((pow_off_time.year >= 10) && (pow_on_time.year >= 10)) // year must large than 2010
    {
        // calc	all day of end time
        llwtmp = pow_off_time.year - 1;
        llwtmp = llwtmp * 365;

        for(i = 0; i < (pow_off_time.month - 1); i++)
        {
            llwtmp = llwtmp + gby_DayPerMonth[i];
        }

        if( ( 0 == (pow_off_time.year % 4) )  // only calc leap year once
                && (pow_off_time.month >= 3) )
        {
            llwtmp ++;
        }

        llwtmp += pow_off_time.day;
        // end of calc	all day of end time

        // calc	all day of start time
        llwtmp_bak = pow_on_time.year - 1;
        llwtmp_bak = llwtmp_bak * 365;

        for(i = 0; i < (pow_on_time.month - 1); i++)
        {
            llwtmp_bak = llwtmp_bak + gby_DayPerMonth[i];
        }

        if( ( 0 == (pow_on_time.year % 4))  // only calc leap year once
                && (pow_on_time.month >= 3) )
        {
            llwtmp_bak ++;
        }

        llwtmp_bak += pow_on_time.day;
        // end of cal	all day of start time

        llwtmp_bak *= 24;
        llwtmp_bak += pow_on_time.hour;
        llwtmp_bak *= 60;
        llwtmp_bak += pow_on_time.minute;
        llwtmp_bak *= 60;
        llwtmp_bak += pow_on_time.second;

        llwtmp *= 24;
        llwtmp += pow_off_time.hour;
        llwtmp *= 60;
        llwtmp += pow_off_time.minute;
        llwtmp *= 60;
        llwtmp += pow_off_time.second;

        if(llwtmp_bak > llwtmp)
        {
            llwtmp_bak -= llwtmp;		//calc time space unit:S
        }
        else
        {
            llwtmp_bak = 0;
        }

        *standby_time = llwtmp_bak;
    }
    else
    {
        return (GeneralReturnType)BMSSYSST_ERR_GETTIME;
    }

    return ERR_OK;
    
}

#pragma DATA_SEG DEFAULT
