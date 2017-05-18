// BatterySyst.h
#ifndef BATTERYSYSST_H
#define BATTERYSYSST_H

#include "GeneralTypeDef.h"
#include "Ecu.h"

typedef struct
{
    uByte RunMode;
    uByte WorkSt;
    uByte OpenCircuitSt;
} sBatSys_OperationStType;

typedef struct
{
    uByte IsoSt;
    uByte IsoRank;
    uWord IsoPosResistor;
    uWord IsoNegResistor;
} sBatSys_IsoStType;

typedef struct
{
    uByte  PosRlyLifeTime[3];
    uByte  NegRlyLifeTime[3];
    uByte  PreRlyLifeTime[3];
    uByte  ChgRlyLifeTime[3];
} BatSys_RelayLifeType;

typedef struct
{
    uByte PosRlySt;
    uByte NegRlySt;
    uByte PreRlySt;
    uByte ChgRlySt;
    uByte AuxASt;
    uByte AuxBSt; 

} sBatSys_RelayStType;

typedef struct
{
    uByte Heat;
    uByte Fan;
} sBatSys_PackRelayStType;


typedef struct
{
    uWord  FanSpeed;
    uByte  FanSt;
    uByte  HeaterSt;
} sBatSys_ThmSysStType;

typedef struct
{
    uByte  MaintainSwitchSt;
    uByte  LatchingMechanismSt;
    uByte  CollisionSwitchSt;
    uByte  ManualMaintenanceSwitchSt;
    uByte  FaultDryContactSt;

} sBatSys_SwitchStType;

typedef struct
{
    uByte  SmokeDetectorSt;
    uByte  HumiditySensorSt;
    uByte  PressureSensorSt;
    uByte  LiquidLevelSensorSt;
} sBatSys_SensorStType;

typedef struct
{
    uByte  HvilSt;
    uWord  PreChgTime;
} sBatSys_HighVoltStType;

typedef struct
{
    uByte VinNum[17];
    uByte PlateNum[8];
} sBatSys_VehicleNumType;

typedef struct
{
    sBatSys_RelayStType      RelaySt;  
    sBatSys_ThmSysStType     ThmSysSt;      /* thermal management system  */
    sBatSys_OperationStType  OperationSt;
    sBatSys_IsoStType        IsoSt;    
    sBatSys_SwitchStType     SwitchSt;
    sBatSys_SensorStType     SensorSt;
    sBatSys_HighVoltStType   HighVoltSt;
    sBatSys_VehicleNumType   VehicleNumSt;
    BatSys_RelayLifeType     RelayLife;
    sBatSys_PackRelayStType      PackRelaySt[MAX_BMU_NUM];
    sBatSys_ThmSysStType     PackThmSysSt[MAX_BMU_NUM];   /* thermal management system  */
} sBatSys_OperationSt;


extern void BatSys_StInit(void);

extern GeneralReturnType BatSys_RlyLifeCalc(uByte RelayId, uWord Cur);

extern uByte BatSys_GetWorkSt(void);
extern void BatSys_UpdateWorkSt(uByte status);

extern uByte BatSys_GetOpenCircuitSt(void);
extern  void BatSys_UpdateOpenCircuitSt(uByte status);

extern uByte BatSys_GetIsoSt(void);
extern void BatSys_UpdateIsoSt(uByte status);

extern uByte BatSys_GetIsoRank(void);
extern void BatSys_UpdateIsoRank(uByte rank);

extern uWord BatSys_GetIsoPosResistor(void);
extern void BatSys_UpdateIsoPosResistor(uWord res);

extern uWord BatSys_GetIsoNegResistor(void);
extern void BatSys_UpdateIsoNegResistor(uWord res);

extern uByte BatSys_GetRunMode(void);
extern void BatSys_UpdateRunMode(uByte mode);

extern uByte BatSys_GetPosRlySt(void);
extern void BatSys_UpdatePosRlySt(uByte status);

extern uByte BatSys_GetNegRlySt(void);
extern void BatSys_UpdateNegRlySt(uByte status);

extern uByte BatSys_GetPreRlySt(void);
extern void BatSys_UpdatePreRlySt(uByte status);

extern uByte BatSys_GetChgRlySt(void);
extern void BatSys_UpdateChgRlySt(uByte status);

extern uByte BatSys_GetAuxASt(void);
extern void BatSys_UpdateAuxASt(uByte status);

extern uByte BatSys_GetAuxBSt(void);
extern void BatSys_UpdateAuxBSt(uByte status);

extern uLWord BatSys_GetPosRlyLifeTime(void);
extern void BatSys_UpdatePosRlyLifeTime(uLWord life_time);

extern uLWord BatSys_GetNegRlyLifeTime(void);
extern void BatSys_UpdateNegRlyLifeTime(uLWord life_time);

extern uLWord BatSys_GetPreRlyLifeTime(void);
extern void BatSys_UpdatePreRlyLifeTime(uLWord life_time);

extern uLWord BatSys_GetChgRlyLifeTime(void);
extern void BatSys_UpdateChgRlyLifeTime(uLWord life_time);

extern uWord BatSys_GetFanSpeed(void);
extern void BatSys_UpdateFanSpeed(uWord speed);

extern uByte BatSys_GetFanSt(void);
extern void BatSys_UpdateFanSt(uByte status);

extern uByte BatSys_GetHeaterSt(void);
extern void BatSys_UpdateHeaterSt(uByte status);

extern uWord BatSys_GetAllPackFanSpeed(uByte bmu_no);
extern void BatSys_UpdateAllPackFanSpeed(uByte bmu_no, uWord speed);

extern uByte  BatSys_GetAllPackFanSt(uByte bmu_no);
extern void BatSys_UpdateAllPackFanSt(uByte bmu_no, uByte status);

extern uByte  BatSys_GetAllPackHeaterSt(uByte bmu_no);
extern void BatSys_UpdateAllPackHeaterSt(uByte bmu_no, uByte status);


extern uByte BatSys_GetAllPackHeatRlySt(uByte bmu_no);
extern void BatSys_UpdateAllPackHeatRlySt(uByte bmu_no, uByte status);

extern uByte BatSys_GetAllPackFanRlySt(uByte bmu_no);
extern void BatSys_UpdateAllPackFanRlySt(uByte bmu_no, uByte status);

/*
extern uByte BatSys_GetAllPackPreRlySt(uByte bmu_no);
extern void BatSys_UpdateAllPackPreRlySt(uByte bmu_no, uByte status);

extern uByte BatSys_GetAllPackChgRlySt(uByte bmu_no);
extern void BatSys_UpdateAllPackChgRlySt(uByte bmu_no, uByte status);

extern uByte BatSys_GetAllPackAuxBSt(uByte bmu_no);
extern void BatSys_UpdateAllPackAuxBSt(uByte bmu_no, uByte status);

extern uByte BatSys_GetAllPackAuxASt(uByte bmu_no);
extern void BatSys_UpdateAllPackAuxASt(uByte bmu_no, uByte status);
*/
extern uByte BatSys_GetHvilSt(void);
extern void BatSys_UpdateHvilSt(uByte status);

extern uByte BatSys_GetMaintainSwitchSt(void);
extern void BatSys_UpdateMaintainSwitchSt(uByte status);

extern uByte BatSys_GetSmokeDetectorSt(void);
extern void BatSys_UpdateSmokeDetectorSt(uByte status);

extern uByte BatSys_GetLatchingMechanismSt(void);
extern void BatSys_UpdateLatchingMechanismSt(uByte status);

extern uByte BatSys_GetCollisionSwitchSt(void);
extern void BatSys_UpdateCollisionSwitchSt(uByte status);

extern uByte BatSys_GetManualMaintenanceSwitchSt(void);
extern void BatSys_UpdateManualMaintenanceSwitchSt(uByte status);

extern uByte BatSys_GetHumiditySensorSt(void);
extern void BatSys_UpdateHumiditySensorSt(uByte status);

extern uByte BatSys_GetPressureSensorSt(void);
extern void BatSys_UpdatePressureSensorSt(uByte status);

extern uByte BatSys_GetLiquidLevelSensorSt(void);
extern void BatSys_UpdateLiquidLevelSensorSt(uByte status);

extern uByte BatSys_GetFaultDryContactSt(void);
extern void BatSys_UpdateFaultDryContactSt(uByte status);

extern uWord BatSys_GetPreChgTime(void);
extern void BatSys_UpdatePreChgTime(uWord time);

extern void BatSys_GetCarVinNum(uByte * data);
extern void BatSys_UpdateCarVinNum(uByte * data);

extern void BatSys_GetCarPlateNum(uByte * data);
extern void BatSys_UpdateCarPlateNum(uByte * data);

#endif  /* BMSSYSST_H */