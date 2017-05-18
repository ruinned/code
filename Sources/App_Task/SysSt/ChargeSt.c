
#include "ChargeSt.h"
#include "Ecu.h"
#include "SysConf.h"
#include "EE.h"
#include "ErrorAlarm.h"

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

sChg_InfoType  Chg_Info;

void ChgInfo_Init(void)
{
    uByte *RAMPTR p_data;
    uWord wcnt;

    /* initial global variable */
    p_data = (uByte *RAMPTR)(&Chg_Info);
    for(wcnt = 0; wcnt < sizeof(Chg_Info); wcnt++)
    {
        *(p_data++) = 0;
    }

    (void)SysConf_InitParaTable(SYSTEMST_CHG_CTRL_PARA, (uLWord)(&Chg_Info.Chg_Parameter), sizeof(Chg_Info.Chg_Parameter));

}

uByte Chg_GetAcLinkSt(void)
{
    return Chg_Info.Chg_Interface.AC_LinkSt;
}

void Chg_UpdateAcLinkSt(uByte status)
{
    Chg_Info.Chg_Interface.AC_LinkSt = status;
}

uWord Chg_GetCableCap(void)
{
    return Chg_Info.Chg_Interface.CableCap;
}

void Chg_UpdateCableCap(uWord cap)
{
    Chg_Info.Chg_Interface.CableCap = cap;
}

uWord Chg_GetCcVolt(void)
{
    return Chg_Info.Chg_Interface.CC_Volt;
}

void Chg_UpdateCCVolt(uWord volt)
{
    Chg_Info.Chg_Interface.CC_Volt = volt;
}

uByte Chg_GetCcSt(void)
{
    return Chg_Info.Chg_Interface.CC_St;
}

void Chg_UpdateCcSt(uByte status)
{
    Chg_Info.Chg_Interface.CC_St = status;
}

uWord Chg_GetCpPeriod(void)
{
    return Chg_Info.Chg_Interface.CP_Period;
}

void Chg_UpdateCpPeriod(uWord period)
{
    Chg_Info.Chg_Interface.CP_Period = period;
}

uByte Chg_GetCpDutyCycle(void)
{
    return Chg_Info.Chg_Interface.CP_DutyCycle;
}

void Chg_UpdateCpDutyCycle(uByte duty)
{
    Chg_Info.Chg_Interface.CP_DutyCycle = duty;
}

uWord Chg_GetCpPmtCur(void)
{
    return Chg_Info.Chg_Interface.CP_PmtCur;
}

void Chg_UpdateCpPmtCur(uWord cur)
{
    Chg_Info.Chg_Interface.CP_PmtCur = cur;
}

uByte Chg_GetDcLinkSt(void)
{
    return Chg_Info.Chg_Interface.DC_LinkSt;
}

void Chg_UpdateDcLinkSt(uByte status)
{
    Chg_Info.Chg_Interface.DC_LinkSt = status;
}

uWord Chg_GetCc2Volt(void)
{
    return Chg_Info.Chg_Interface.CC2_Volt;
}

void Chg_UpdateCc2Volt(uWord volt)
{
    Chg_Info.Chg_Interface.CC2_Volt = volt;
}

uByte Chg_GetCc2St(void)
{
    return Chg_Info.Chg_Interface.CC2_St;
}

void Chg_UpdateCc2St(uByte status)
{
    Chg_Info.Chg_Interface.CC2_St = status;
}

uByte Chg_GetPermitMaxT(void)
{
    return Chg_Info.Chg_Parameter.PermitMaxT;
}

void Chg_UpdatePermitMaxT(uByte max_t)
{
    Chg_Info.Chg_Parameter.PermitMaxT = max_t;
}

uByte Chg_GetPermitMinT(void)
{
    return Chg_Info.Chg_Parameter.PermitMinT;
}

void Chg_UpdatePermitMinT(uByte min_t)
{
    Chg_Info.Chg_Parameter.PermitMinT = min_t;
}

uWord Chg_GetPermitMaxSumV(void)
{
    return Chg_Info.Chg_Parameter.PermitMaxSumV;
}

void Chg_UpdatePermitMaxSumV(uWord sum_v)
{
    Chg_Info.Chg_Parameter.PermitMaxSumV = sum_v;
}

uWord Chg_GetPermitMaxCellV(void)
{
    return Chg_Info.Chg_Parameter.PermitMaxCellV;
}

void Chg_UpdatePermitMaxCellV(uWord max_v)
{
    Chg_Info.Chg_Parameter.PermitMaxCellV = max_v;
}

uWord Chg_GetPermitMinCellV(void)
{
    return Chg_Info.Chg_Parameter.PermitMinCellV;
}

void Chg_UpdatePermitMinCellV(uWord min_v)
{
    Chg_Info.Chg_Parameter.PermitMinCellV = min_v;
}

uWord Chg_GetPermitMaxCurrent(void)
{
    return Chg_Info.Chg_Parameter.PermitMaxCurrent;
}

void Chg_UpdatePermitMaxCurrent(uWord cur)
{
    Chg_Info.Chg_Parameter.PermitMaxCurrent = cur;
}

uWord Chg_GetPermitMinCurrent(void)
{
    return Chg_Info.Chg_Parameter.PermitMinCurrent;
}

void Chg_UpdatePermitMinCurrent(uWord cur)
{
    Chg_Info.Chg_Parameter.PermitMinCurrent = cur;
}

uWord Chg_GetDownCurrentStep(void)
{
    return Chg_Info.Chg_Parameter.DownCurrentStep;
}

void Chg_UpdateDownCurrentStep(uWord cur_step)
{
    Chg_Info.Chg_Parameter.DownCurrentStep = cur_step;
}

uByte Chg_GetPermitChgExtSt(void)
{
    return Chg_Info.Chg_Ctrl.PermitChgSt.PmtChgExtSt;
}

void Chg_UpdatePermitChgExtSt(uByte status)
{
    Chg_Info.Chg_Ctrl.PermitChgSt.PmtChgExtSt = status;
}

uByte Chg_GetPermitChgOnSt(void)
{
    return Chg_Info.Chg_Ctrl.PermitChgSt.PmtChgOnSt;
}

void Chg_UpdatePermitChgOnSt(uByte status)
{
    Chg_Info.Chg_Ctrl.PermitChgSt.PmtChgOnSt = status;
}

uWord Chg_GetPermitChgCurOn(void)
{
    return Chg_Info.Chg_Ctrl.PermitChgCur.AcPermitChgCur;
}

void Chg_UpdatePermitChgCurOn(uWord cur)
{
    Chg_Info.Chg_Ctrl.PermitChgCur.AcPermitChgCur = cur;
}

uWord Chg_GetPermitChgCurExt(void)
{
    return Chg_Info.Chg_Ctrl.PermitChgCur.DcPermitChgCur;
}

void Chg_UpdatePermitChgCurExt(uWord cur)
{
    Chg_Info.Chg_Ctrl.PermitChgCur.DcPermitChgCur = cur;
}

uByte Chg_GetChgSt(void)
{
    return Chg_Info.Chg_Ctrl.ChgSt;
}

void Chg_UpdateChgSt(uByte status)
{
    Chg_Info.Chg_Ctrl.ChgSt = status;
}

uByte Chg_GetChgStage(void)
{
    return Chg_Info.Chg_Ctrl.ChgStage;
}

void Chg_UpdateChgStage(uByte stage)
{
    Chg_Info.Chg_Ctrl.ChgStage = stage;
}

uWord Chg_GetAccumulatedChgTime(void)
{
    return Chg_Info.Chg_Ctrl.AccumulatedChgTime;
}

void Chg_UpdateAccumulatedChgTime(uWord time)
{
    Chg_Info.Chg_Ctrl.AccumulatedChgTime = time;
}

uWord Chg_GetRemainChgTime(void)
{
    return Chg_Info.Chg_Ctrl.RemainChgTime;
}

void Chg_UpdateRemainChgTime(uWord time)
{
    Chg_Info.Chg_Ctrl.RemainChgTime = time;
}

uLWord Chg_GetBmsStopReason(void)
{
    return Chg_Info.Chg_EndReason.BmsStop.Lwd;
}

void Chg_UpdateBmsStopReason(uLWord reason)
{
    Chg_Info.Chg_EndReason.BmsStop.Lwd = reason;
}

uByte Chg_GetChargerStopReason(void)
{
    return Chg_Info.Chg_EndReason.ChargerStopMoto.By;
}

void Chg_UpdateChargerStopReason(uByte reason)
{
    Chg_Info.Chg_EndReason.ChargerStop.By = reason;
    Chg_Info.Chg_EndReason.ChargerStopMoto.Bit.FaultStop = Chg_Info.Chg_EndReason.ChargerStop.Bit.FaultStop;
    Chg_Info.Chg_EndReason.ChargerStopMoto.Bit.PeopleStop = Chg_Info.Chg_EndReason.ChargerStop.Bit.PeopleStop;
    Chg_Info.Chg_EndReason.ChargerStopMoto.Bit.ReachCondition = Chg_Info.Chg_EndReason.ChargerStop.Bit.ReachCondition;
    Chg_Info.Chg_EndReason.ChargerStopMoto.Bit.Rsv = Chg_Info.Chg_EndReason.ChargerStop.Bit.Rsv;
    
}

uWord Chg_GetChargerFault(void)
{
    return Chg_Info.Chg_EndReason.ChargerFaultMoto.Wd;
}

void Chg_UpdateChargerFault(uWord fault)
{
    Chg_Info.Chg_EndReason.ChargerFault.Wd = fault;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.TemperHigh = Chg_Info.Chg_EndReason.ChargerFault.Bit.TemperHigh;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.ConectorFault = Chg_Info.Chg_EndReason.ChargerFault.Bit.ConectorFault;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.IntTemperHigh = Chg_Info.Chg_EndReason.ChargerFault.Bit.IntTemperHigh;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.EnergyFault = Chg_Info.Chg_EndReason.ChargerFault.Bit.EnergyFault;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.TripFault = Chg_Info.Chg_EndReason.ChargerFault.Bit.TripFault;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.SumVFault = Chg_Info.Chg_EndReason.ChargerFault.Bit.SumVFault;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.CurNotMatch = Chg_Info.Chg_EndReason.ChargerFault.Bit.CurNotMatch;
    Chg_Info.Chg_EndReason.ChargerFaultMoto.Bit.OtherFault = Chg_Info.Chg_EndReason.ChargerFault.Bit.OtherFault;
}

uByte Chg_GetChargerSt(void)
{
    return Chg_Info.Chg_ChargerInfo.ChargerSt;
}

void Chg_UpdateChargerSt(uByte status)
{
    Chg_Info.Chg_ChargerInfo.ChargerSt = status;
}



uWord Chg_GetOutPutVoltage(void)
{
    return Chg_Info.Chg_ChargerInfo.OutPutVoltage;
}

void Chg_UpdateOutPutVoltage(uWord volt)
{
    Chg_Info.Chg_ChargerInfo.OutPutVoltage = volt;
}

uWord Chg_GetOutPutCurrent(void)
{
    return Chg_Info.Chg_ChargerInfo.OutPutCurrent;
}

void Chg_UpdateOutPutCurrent(uWord cur)
{
    Chg_Info.Chg_ChargerInfo.OutPutCurrent = cur;
}

uWord Chg_GetMaxOutPutVoltage(void)
{
    return Chg_Info.Chg_ChargerInfo.MaxOutPutVoltage;
}

void Chg_UpdateMaxOutPutVoltage(uWord volt)
{
    Chg_Info.Chg_ChargerInfo.MaxOutPutVoltage = volt;
}

uWord Chg_GetMinOutPutVoltage(void)
{
    return Chg_Info.Chg_ChargerInfo.MinOutPutVoltage;
}

void Chg_UpdateMinOutPutVoltage(uWord volt)
{
    Chg_Info.Chg_ChargerInfo.MinOutPutVoltage = volt;
}

uWord Chg_GetOutPutMinCurrent(void)
{
    return Chg_Info.Chg_ChargerInfo.OutPutMinCurrent;
}

void Chg_UpdateOutPutMinCurrent(uWord cur)
{
    Chg_Info.Chg_ChargerInfo.OutPutMinCurrent = cur;
}

uWord Chg_GetOutPutMaxCurrent(void)
{
    return Chg_Info.Chg_ChargerInfo.OutPutMaxCurrent;
}

void Chg_UpdateOutPutMaxCurrent(uWord cur)
{
    Chg_Info.Chg_ChargerInfo.OutPutMaxCurrent = cur;
}

uWord Chg_GetOutPutEnergy(void)
{
    return Chg_Info.Chg_ChargerInfo.OutPutEnergy;
}

void Chg_UpdateOutPutEnergy(uWord energy)
{
    Chg_Info.Chg_ChargerInfo.OutPutEnergy = energy;
}

uWord Chg_GetErrorCode(void)
{
    return Chg_Info.Chg_ChargerInfo.ErrorCode;
}

void Chg_UpdateErrorCode(uWord err_code)
{
    Chg_Info.Chg_ChargerInfo.ErrorCode = err_code;
}

#pragma DATA_SEG DEFAULT