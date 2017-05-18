
#include "BatterySysSt.h"
#include "BatterySt.h"
#include "SysConf.h"
#include "EE.h"
#include "AppRelay.h"

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

sBatSys_OperationSt  BatSys_StInfo;

#define BAT_CUR_POINT              12
#define BAT_VOLT_POINT              1
#define BAT_RLY_MAX_LIFE      10000000
#define BAT_CUR_200A             2000

typedef struct
{
	uWord  Cur;           //0.1A
  uLWord Counter;       //0.01
} sBatSys_RlyAgeType;

const sBatSys_RlyAgeType BatSys_RlyAge[BAT_VOLT_POINT][BAT_CUR_POINT] = 
{	
  {    
   	{120, 1000},   {200, 1389},  {300, 2000},   {400, 2778},
  	{500, 3333},   {600, 4348},  {700, 4878},   {800, 5556},
  	{900, 6667},   {1000, 7692}, {1300, 10000}, {2000, 43480}    // Cur: 0.1A unit, offset 0 Counter 0.01
  }
};

void BatSys_StInit(void)
{
    uByte *RAMPTR pByte;
    uWord wcnt;
    GeneralReturnType rslt;

    pByte = (uByte *RAMPTR)&BatSys_StInfo;
    for(wcnt = 0; wcnt < sizeof(BatSys_StInfo); wcnt++)
    {
        *(pByte++) = 0;
    }

    (void)SysConf_InitParaTable(SYSTEMST_CONTACTOR_LIFE, (uLWord)(BatSys_StInfo.RelayLife.PosRlyLifeTime), sizeof(BatSys_StInfo.RelayLife));
    rslt = EE_LoadVar(SYSTEMST_CONTACTOR_LIFE);

    (void)SysConf_InitParaTable(SYSTEMST_VEHICLE_VIN, (uLWord)(BatSys_StInfo.VehicleNumSt.VinNum), sizeof(BatSys_StInfo.VehicleNumSt.VinNum ));
    rslt = EE_LoadVar(SYSTEMST_VEHICLE_VIN);

    (void)SysConf_InitParaTable(SYSTEMST_VEHICLE_PLATE, (uLWord)(BatSys_StInfo.VehicleNumSt.PlateNum), sizeof(BatSys_StInfo.VehicleNumSt.PlateNum ));
    rslt = EE_LoadVar(SYSTEMST_VEHICLE_PLATE);

    // prevent C5917
    (void)rslt;
}


uByte BatSys_GetRunMode(void)
{
    return  BatSys_StInfo.OperationSt.RunMode;
} 

void BatSys_UpdateRunMode(uByte mode)
{
    BatSys_StInfo.OperationSt.RunMode = mode;
}


uByte BatSys_GetWorkSt(void)
{
    return  BatSys_StInfo.OperationSt.WorkSt;
}

void BatSys_UpdateWorkSt(uByte status)
{
    BatSys_StInfo.OperationSt.WorkSt = status;
}

uByte BatSys_GetOpenCircuitSt(void)
{
    return  BatSys_StInfo.OperationSt.OpenCircuitSt;
}

void BatSys_UpdateOpenCircuitSt(uByte status)
{
    BatSys_StInfo.OperationSt.OpenCircuitSt = status;
}

uByte BatSys_GetIsoSt(void)
{
    return  BatSys_StInfo.IsoSt.IsoSt;
}

void BatSys_UpdateIsoSt(uByte status)
{
    BatSys_StInfo.IsoSt.IsoSt = status;
}

uByte BatSys_GetIsoRank(void)
{
    return  BatSys_StInfo.IsoSt.IsoRank;
}

void BatSys_UpdateIsoRank(uByte rank)
{
    BatSys_StInfo.IsoSt.IsoRank = rank;
}

uWord BatSys_GetIsoPosResistor(void)
{
    return  BatSys_StInfo.IsoSt.IsoPosResistor;
}

void BatSys_UpdateIsoPosResistor(uWord res)
{
    BatSys_StInfo.IsoSt.IsoPosResistor = res;
}

uWord BatSys_GetIsoNegResistor(void)
{
    return  BatSys_StInfo.IsoSt.IsoNegResistor;
}

void BatSys_UpdateIsoNegResistor(uWord res)
{
    BatSys_StInfo.IsoSt.IsoNegResistor = res;
}


uByte BatSys_GetPosRlySt(void)
{
    return  BatSys_StInfo.RelaySt.PosRlySt;
}

void BatSys_UpdatePosRlySt(uByte status)
{
    BatSys_StInfo.RelaySt.PosRlySt = status;
}

uByte BatSys_GetNegRlySt(void)
{
    return  BatSys_StInfo.RelaySt.NegRlySt;
}

void BatSys_UpdateNegRlySt(uByte status)
{
    BatSys_StInfo.RelaySt.NegRlySt = status;
}

uByte BatSys_GetPreRlySt(void)
{
    return  BatSys_StInfo.RelaySt.PreRlySt;
}

void BatSys_UpdatePreRlySt(uByte status)
{
    BatSys_StInfo.RelaySt.PreRlySt = status;
}

uByte BatSys_GetChgRlySt(void)
{
    return  BatSys_StInfo.RelaySt.ChgRlySt;
}

void BatSys_UpdateChgRlySt(uByte status)
{
    BatSys_StInfo.RelaySt.ChgRlySt = status;
}

uByte BatSys_GetAuxBSt(void)
{
    return  BatSys_StInfo.RelaySt.AuxBSt;
}

void BatSys_UpdateAuxBSt(uByte status)
{
    BatSys_StInfo.RelaySt.AuxBSt = status;
}

uByte BatSys_GetAuxASt(void)
{
    return  BatSys_StInfo.RelaySt.AuxASt;
}

void BatSys_UpdateAuxASt(uByte status)
{
    BatSys_StInfo.RelaySt.AuxASt = status;
}


uLWord BatSys_GetPosRlyLifeTime(void)
{
    uLWord lwtmp;
    lwtmp = BatSys_StInfo.RelayLife.PosRlyLifeTime[2];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.PosRlyLifeTime[1];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.PosRlyLifeTime[0];
    return  lwtmp;
}

void BatSys_UpdatePosRlyLifeTime(uLWord life_time)
{
    BatSys_StInfo.RelayLife.PosRlyLifeTime[0] = (uByte)(life_time & 0xff);
    BatSys_StInfo.RelayLife.PosRlyLifeTime[1] = (uByte)((life_time >> 8) & 0xff);
    BatSys_StInfo.RelayLife.PosRlyLifeTime[2] = (uByte)((life_time >> 16) & 0xff);
}

uLWord BatSys_GetNegRlyLifeTime(void)
{
    uLWord lwtmp;
    lwtmp = BatSys_StInfo.RelayLife.NegRlyLifeTime[2];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.NegRlyLifeTime[1];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.NegRlyLifeTime[0];
    return  lwtmp;
}

void BatSys_UpdateNegRlyLifeTime(uLWord life_time)
{
    BatSys_StInfo.RelayLife.NegRlyLifeTime[0] = (uByte)(life_time & 0xff);
    BatSys_StInfo.RelayLife.NegRlyLifeTime[1] = (uByte)((life_time >> 8) & 0xff);
    BatSys_StInfo.RelayLife.NegRlyLifeTime[2] = (uByte)((life_time >> 16) & 0xff);
}

uLWord BatSys_GetPreRlyLifeTime(void)
{
    uLWord lwtmp;
    lwtmp = BatSys_StInfo.RelayLife.PreRlyLifeTime[2];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.PreRlyLifeTime[1];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.PreRlyLifeTime[0];
    return  lwtmp;
}

void BatSys_UpdatePreRlyLifeTime(uLWord life_time)
{
    BatSys_StInfo.RelayLife.PreRlyLifeTime[0] = (uByte)(life_time & 0xff);
    BatSys_StInfo.RelayLife.PreRlyLifeTime[1] = (uByte)((life_time >> 8) & 0xff);
    BatSys_StInfo.RelayLife.PreRlyLifeTime[2] = (uByte)((life_time >> 16) & 0xff);
}

uLWord BatSys_GetChgRlyLifeTime(void)
{
    uLWord lwtmp;
    lwtmp = BatSys_StInfo.RelayLife.ChgRlyLifeTime[2];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.ChgRlyLifeTime[1];
    lwtmp = (lwtmp << 8) + BatSys_StInfo.RelayLife.ChgRlyLifeTime[0];
    return  lwtmp;
}

void BatSys_UpdateChgRlyLifeTime(uLWord life_time)
{
    BatSys_StInfo.RelayLife.ChgRlyLifeTime[0] = (uByte)(life_time & 0xff);
    BatSys_StInfo.RelayLife.ChgRlyLifeTime[1] = (uByte)((life_time >> 8) & 0xff);
    BatSys_StInfo.RelayLife.ChgRlyLifeTime[2] = (uByte)((life_time >> 16) & 0xff);
}


uWord BatSys_GetFanSpeed(void)
{
    return  BatSys_StInfo.ThmSysSt.FanSpeed;
}

void BatSys_UpdateFanSpeed(uWord speed)
{
    BatSys_StInfo.ThmSysSt.FanSpeed = speed;
}

uByte  BatSys_GetFanSt(void)
{
    return  BatSys_StInfo.ThmSysSt.FanSt;
}

void BatSys_UpdateFanSt(uByte status)
{
    BatSys_StInfo.ThmSysSt.FanSt = status;
}

uByte  BatSys_GetHeaterSt(void)
{
    return  BatSys_StInfo.ThmSysSt.HeaterSt;
}

void BatSys_UpdateHeaterSt(uByte status)
{
    BatSys_StInfo.ThmSysSt.HeaterSt = status;
}


uWord BatSys_GetAllPackFanSpeed(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    return  BatSys_StInfo.PackThmSysSt[bmu_no].FanSpeed;
}

void BatSys_UpdateAllPackFanSpeed(uByte bmu_no, uWord speed)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    BatSys_StInfo.PackThmSysSt[bmu_no].FanSpeed = speed;
}

uByte  BatSys_GetAllPackFanSt(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    return  BatSys_StInfo.PackThmSysSt[bmu_no].FanSt;
}

void BatSys_UpdateAllPackFanSt(uByte bmu_no, uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    BatSys_StInfo.PackThmSysSt[bmu_no].FanSt = status;
}

uByte  BatSys_GetAllPackHeaterSt(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    return  BatSys_StInfo.PackThmSysSt[bmu_no].HeaterSt;
}

void BatSys_UpdateAllPackHeaterSt(uByte bmu_no,uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    BatSys_StInfo.PackThmSysSt[bmu_no].HeaterSt = status;
}


uByte BatSys_GetAllPackHeatRlySt(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    return  BatSys_StInfo.PackRelaySt[bmu_no].Heat;
}

void BatSys_UpdateAllPackHeatRlySt(uByte bmu_no, uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    BatSys_StInfo.PackRelaySt[bmu_no].Heat = status;
}

uByte BatSys_GetAllPackFanRlySt(uByte bmu_no)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    return  BatSys_StInfo.PackRelaySt[bmu_no].Fan;
}

void BatSys_UpdateAllPackFanRlySt(uByte bmu_no, uByte status)
{
    if(bmu_no > 0)
    {
       bmu_no -= 1;
    }
    BatSys_StInfo.PackRelaySt[bmu_no].Fan = status;
}


uByte  BatSys_GetHvilSt(void)
{
    return  BatSys_StInfo.HighVoltSt.HvilSt;
}

void BatSys_UpdateHvilSt(uByte status)
{
    BatSys_StInfo.HighVoltSt.HvilSt = status;
}

uByte  BatSys_GetMaintainSwitchSt(void)
{
    return  BatSys_StInfo.SwitchSt.MaintainSwitchSt;
}

void BatSys_UpdateMaintainSwitchSt(uByte status)
{
    BatSys_StInfo.SwitchSt.MaintainSwitchSt = status;
}

uByte  BatSys_GetSmokeDetectorSt(void)
{
    return  BatSys_StInfo.SensorSt.SmokeDetectorSt;
}

void BatSys_UpdateSmokeDetectorSt(uByte status)
{
    BatSys_StInfo.SensorSt.SmokeDetectorSt = status;
}

uByte  BatSys_GetLatchingMechanismSt(void)
{
    return  BatSys_StInfo.SwitchSt.LatchingMechanismSt;
}

void BatSys_UpdateLatchingMechanismSt(uByte status)
{
    BatSys_StInfo.SwitchSt.LatchingMechanismSt = status;
}

uByte  BatSys_GetCollisionSwitchSt(void)
{
    return  BatSys_StInfo.SwitchSt.CollisionSwitchSt;
}

void BatSys_UpdateCollisionSwitchSt(uByte status)
{
    BatSys_StInfo.SwitchSt.CollisionSwitchSt = status;
}

uByte  BatSys_GetManualMaintenanceSwitchSt(void)
{
    return  BatSys_StInfo.SwitchSt.ManualMaintenanceSwitchSt;
}

void BatSys_UpdateManualMaintenanceSwitchSt(uByte status)
{
    BatSys_StInfo.SwitchSt.ManualMaintenanceSwitchSt = status;
}

uByte  BatSys_GetHumiditySensorSt(void)
{
    return  BatSys_StInfo.SensorSt.HumiditySensorSt;
}

void BatSys_UpdateHumiditySensorSt(uByte status)
{
    BatSys_StInfo.SensorSt.HumiditySensorSt = status;
}

uByte  BatSys_GetPressureSensorSt(void)
{
    return  BatSys_StInfo.SensorSt.PressureSensorSt;
}

void BatSys_UpdatePressureSensorSt(uByte status)
{
    BatSys_StInfo.SensorSt.PressureSensorSt = status;
}

uByte  BatSys_GetLiquidLevelSensorSt(void)
{
    return  BatSys_StInfo.SensorSt.LiquidLevelSensorSt;
}

void BatSys_UpdateLiquidLevelSensorSt(uByte status)
{
    BatSys_StInfo.SensorSt.LiquidLevelSensorSt = status;
}

uByte  BatSys_GetFaultDryContactSt(void)
{
    return  BatSys_StInfo.SwitchSt.FaultDryContactSt;
}

void BatSys_UpdateFaultDryContactSt(uByte status)
{
    BatSys_StInfo.SwitchSt.FaultDryContactSt = status;
}

uWord  BatSys_GetPreChgTime(void)
{
    return  BatSys_StInfo.HighVoltSt.PreChgTime;
}

void BatSys_UpdatePreChgTime(uWord time)
{
    BatSys_StInfo.HighVoltSt.PreChgTime = time;
}

void BatSys_GetCarVinNum(uByte * data)
{
    uByte i;
    for(i = 0; i < sizeof(BatSys_StInfo.VehicleNumSt.VinNum); i++)
    {
        *(data++) = BatSys_StInfo.VehicleNumSt.VinNum[i];
    }
}

void BatSys_UpdateCarVinNum(uByte * data)
{
    uByte i;
    for(i = 0; i < sizeof(BatSys_StInfo.VehicleNumSt.VinNum); i++)
    {
        BatSys_StInfo.VehicleNumSt.VinNum[i] = *(data++);
    }
}

void BatSys_GetCarPlateNum(uByte * data)
{
    uByte i;
    for(i = 0; i < sizeof(BatSys_StInfo.VehicleNumSt.PlateNum); i++)
    {
        *(data++) = BatSys_StInfo.VehicleNumSt.PlateNum[i];
    }
}

void BatSys_UpdateCarPlateNum(uByte * data)
{
    uByte i;
    for(i = 0; i < sizeof(BatSys_StInfo.VehicleNumSt.PlateNum); i++)
    {
        BatSys_StInfo.VehicleNumSt.PlateNum[i] = *(data++);
    }
}


static void BatSys_CalcRlyAgeCnt(uWord Cur, uLWord *Life)
{
  	uByte i,volt;
  	uLWord AgeCnt;
  	uLWord llwd_k;
  	uLWord Cnt;
  	
  	volt = 0;
  	AgeCnt = 0;
  	
    if(Cur > OFFSET_CUR)
    {
        Cur = (Cur - OFFSET_CUR);
    }
    else
    {
        Cur = (OFFSET_CUR - Cur);
    }

  	if(Cur >= BAT_CUR_200A)
  	{
    		AgeCnt = (uLWord)Cur * 2175; 
  	}
    else
    {          
        for(i = 0; i < BAT_CUR_POINT; i++)
        {
            if(Cur == BatSys_RlyAge[volt][i].Cur)
            {
                AgeCnt = BatSys_RlyAge[volt][i].Counter;
                break;
            }
            else
            {
                if (Cur < BatSys_RlyAge[volt][i].Cur)
                {
                  	if(i == 0)
                  	{
                  	    AgeCnt = BatSys_RlyAge[volt][0].Counter;
                  	    break;
                  	}
                  	else
            	      {
            		       llwd_k = (BatSys_RlyAge[volt][i].Counter - BatSys_RlyAge[volt][i-1].Counter) 
            				           / (BatSys_RlyAge[volt][i].Cur - BatSys_RlyAge[volt][i-1].Cur);
                       AgeCnt = BatSys_RlyAge[volt][i-1].Counter 
                               + ((Cur - BatSys_RlyAge[volt][i - 1].Cur) * llwd_k);
                       break ;
            	     }		
                }
            }
        }
    }
    if(*Life < BAT_RLY_MAX_LIFE)
    {
        Cnt = (BAT_RLY_MAX_LIFE - *Life) + AgeCnt;
        if(Cnt < BAT_RLY_MAX_LIFE) 
        {
           *Life = BAT_RLY_MAX_LIFE - Cnt;  // max 1,000,000  // 0.01
        }
        else
        {       
           *Life = 0;
        }
    }
    else
    {
       *Life = 0; 
    }
}

GeneralReturnType BatSys_RlyLifeCalc(uByte RelayId, uWord Cur)
{
    GeneralReturnType rslt; 
    uLWord RemainLife;
    rslt = ERR_OK;
    
    switch(RelayId)
    {    
      case POS_RELAY_ID: /* Pos Relay */
      {
          RemainLife = BatSys_GetPosRlyLifeTime();
          BatSys_CalcRlyAgeCnt(Cur, &RemainLife);
          BatSys_UpdatePosRlyLifeTime(RemainLife);
          rslt = EE_SaveVar(BOTHEE, SYSTEMST_CONTACTOR_LIFE);
          break;
      }
      case NEG_RELAY_ID: /* Neg Relay */
      {
          RemainLife = BatSys_GetNegRlyLifeTime();
          BatSys_CalcRlyAgeCnt(Cur, &RemainLife);
          BatSys_UpdateNegRlyLifeTime(RemainLife);
          rslt = EE_SaveVar(BOTHEE, SYSTEMST_CONTACTOR_LIFE);
          break;
      } 
      case PRE_RELAY_ID: /* Pre Relay */
      {
          RemainLife = BatSys_GetPreRlyLifeTime();
          BatSys_CalcRlyAgeCnt(Cur, &RemainLife);
          BatSys_UpdatePreRlyLifeTime(RemainLife);
          rslt = EE_SaveVar(BOTHEE, SYSTEMST_CONTACTOR_LIFE);
          break;

      }
      case CHG_RELAY_ID: /* Charge Relay */
      {
          RemainLife = BatSys_GetChgRlyLifeTime();
          BatSys_CalcRlyAgeCnt(Cur, &RemainLife);
          BatSys_UpdateChgRlyLifeTime(RemainLife);
          rslt = EE_SaveVar(BOTHEE, SYSTEMST_CONTACTOR_LIFE);
          break;
      }
      default:
      {
          break;
      }

    }
    
    return rslt;

}

#pragma DATA_SEG DEFAULT
